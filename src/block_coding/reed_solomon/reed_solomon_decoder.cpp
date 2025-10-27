#include "reed_solomon_decoder.h"

namespace reed_solomon
{

Decoder::Decoder(uint32_t code_len, uint32_t fec_len,  const galois::field& field,
				 const unsigned int gen_initial_index ):
	code_length(code_len),
	fec_length(fec_len),
	data_length(code_len - fec_len),
	natural_length(255),
	padding_length(natural_length - data_length - fec_len),
	field_(field),
	gen_initial_index_(gen_initial_index)
{
	DIE_IF(field.size() != natural_length);

	//Note: natural_length and field size can be used interchangeably
	create_lookup_tables();
	rsblock_ = new uint8_t[natural_length];
}

Decoder::~Decoder()
{
	delete[] rsblock_;
}


bool Decoder::decode(const uint8_t* rsblock, uint8_t* decode_data)
{
	memcpy(decode_data, rsblock, data_length);
	st.reset();

	memset(&rsblock_[0], 0, padding_length * sizeof(rsblock_[0]));

	for (uint32_t i = 0; i < code_length; ++i)
	{
		rsblock_[padding_length + i] = rsblock[i];
	}

	galois::field_symbol mask = field_.size();
	galois::field_polynomial received(field_, natural_length - 1);

	for (uint32_t i = 0; i < natural_length; ++i)
	{
		received[natural_length - 1 - i] = rsblock_[i] & mask;
	}

	galois::field_polynomial syndrome(field_);

	if (compute_syndrome(received, syndrome) == 0)
	{
		return true;	/// нет ошибок в блоке
	}

	galois::field_polynomial lambda(1, field_);
	modified_berlekamp_massey_algorithm(lambda, syndrome);

	galois::symbol_vector<uint16_t> error_locations;
	find_roots(lambda, error_locations);

	if (0 == error_locations.size())
	{
		/*
		 Syndrome is non-zero yet no error locations have
		 been obtained, conclusion:
		 It is possible that there are MORE errrors in the
		 message than can be detected and corrected for this
		 particular code.
		*/

		st.errors_detected  = 0;
		st.errors_corrected = 0;
		st.zero_numerators  = 0;
		st.unrecoverable    = true;
		st.error            = state_t::e_decoder_error0;
		return false;
	}
	else
		if (((2 * error_locations.size())) > fec_length)
		{
			/*
			  Too many errors\erasures! 2E + S <= fec_length
			   L =  E + S
			   E =  L - S
			  2E = 2L - 2S
			  2E + S = 2L - 2S + S
					 = 2L - S
			 Where:
			  L : Error Locations
			  E : Errors
			  S : Erasures

			*/

			st.errors_detected  = error_locations.size();
			st.errors_corrected = 0;
			st.zero_numerators  = 0;
			st.unrecoverable    = true;
			st.error            = state_t::e_decoder_error1;

			return false;
		}
		else
		{
			st.errors_detected  = error_locations.size();
		}

	bool err = forney_algorithm(error_locations, lambda, syndrome);

	for (uint32_t i = 0; i < data_length; ++i)
	{
		decode_data[i] = rsblock_[padding_length + i];
	}

	return err;
}

void Decoder::create_lookup_tables()
{
	root_exponent_table_.resize(field_.size() + 1);

	for (int i = 0; i < static_cast<int>(field_.size() + 1); ++i)
	{
		root_exponent_table_[i] = (field_.exp(field_.alpha(natural_length - i), (1 - gen_initial_index_)));
	}

	syndrome_exponent_table_.resize(fec_length);

	for (int i = 0; i < static_cast<int>(fec_length); ++i)
	{
		syndrome_exponent_table_[i] = (field_.alpha(gen_initial_index_ + i));
	}
}


int Decoder::compute_syndrome(const galois::field_polynomial& received,
							  galois::field_polynomial& syndrome) const
{
	int error_flag = 0;
	syndrome = galois::field_polynomial(field_, fec_length - 1);

	galois::field_symbol mask = field_.size();

	for (uint32_t i = 0; i < fec_length; ++i)
	{
		syndrome[i]  = received(syndrome_exponent_table_[i]) & mask;
		error_flag  |= syndrome[i];
	}

	return error_flag;
}


void Decoder::find_roots(const galois::field_polynomial& poly, galois::symbol_vector<uint16_t>& root_list) const
{
	/*
	  Chien Search: Find the roots of the error locator polynomial
	  via an exhaustive search over all non-zero elements in the
	  given finite field.
	*/
	const uint32_t polynomial_degree = poly.deg();

	root_list.resize(polynomial_degree, 0);
	uint32_t err_count = 0;

	for (uint16_t i = 1; i <= natural_length; ++i)
	{
		if (0 == poly(field_.alpha(i)))
		{
			root_list[err_count++] = i;

			if (polynomial_degree == err_count)
			{
				root_list.resize(err_count, 0);
				break;
			}
		}
	}

	root_list.resize(err_count, 0);
}

void Decoder::compute_discrepancy(galois::field_symbol&          discrepancy,
								  const galois::field_polynomial& lambda,
								  const galois::field_polynomial& syndrome,
								  const uint32_t&              l,
								  const uint32_t&              round) const
{
	/*
	   Compute the lambda discrepancy at the current round of BMA
	*/
	const uint32_t upper_bound = (l < lambda.deg()) ? l : lambda.deg();

	discrepancy = 0;

	for (uint32_t i = 0; i <= upper_bound; ++i)
	{
		discrepancy ^= (field_.mul(lambda[i], syndrome[round - i]));
	}
}

void Decoder::modified_berlekamp_massey_algorithm(galois::field_polynomial& lambda,
		const galois::field_polynomial& syndrome) const
{
	/*
	   Modified Berlekamp-Massey Algorithm
	   Identify the shortest length linear feed-back shift register (LFSR)
	   that will generate the sequence equivalent to the syndrome.
	*/

	int i = -1;
	uint32_t l = 0;

	galois::field_symbol discrepancy(0);
	galois::field_polynomial previous_lambda(lambda);
	previous_lambda <<= 1;

	for (uint32_t round = 0; round < fec_length; ++round)
	{
		compute_discrepancy(discrepancy, lambda, syndrome, l, round);

		if (discrepancy != 0)
		{
			galois::field_polynomial tau = previous_lambda;

			if (static_cast<int>(l) < (static_cast<int>(round) - i))
			{
				const uint32_t tmp = round - i;
				i = static_cast<int>(round - l);
				l = tmp;
				previous_lambda = lambda;
				previous_lambda /= discrepancy;
			}

			tau *= discrepancy;
			lambda += tau;
		}

		previous_lambda <<= 1;
	}
}

bool Decoder::forney_algorithm(const galois::symbol_vector<uint16_t>& error_locations,
							   const galois::field_polynomial& lambda,
							   const galois::field_polynomial& syndrome)
{
	/*
	   The Forney algorithm for computing the error magnitudes
	*/
	galois::field_polynomial omega(lambda);
	omega *= syndrome;
	omega %= fec_length;
	galois::field_polynomial lambda_derivative(field_);
	lambda.derivative(lambda_derivative);

	st.errors_corrected = 0;
	st.zero_numerators  = 0;

	for (uint32_t i = 0; i < error_locations.size(); ++i)
	{
		const unsigned int         error_location = error_locations[i];
		const galois::field_symbol alpha_inverse  = field_.alpha(error_location);
		galois::field_symbol numerator  = field_.mul(omega(alpha_inverse), root_exponent_table_[error_location]);
		const galois::field_symbol denominator  = lambda_derivative(alpha_inverse);

		if (0 != numerator)
		{
			if (0 != denominator)
			{
				rsblock_[error_location - 1] ^= field_.div(numerator, denominator);
				st.errors_corrected++;
			}
			else
			{
				st.unrecoverable = true;
				st.error         = state_t::e_decoder_error2;
				return false;
			}
		}
		else
		{
			++st.zero_numerators;
		}
	}

	if (lambda.deg() == static_cast<int>(st.errors_detected))
	{
		return true;
	}
	else
	{
		st.unrecoverable = true;
		st.error         = state_t::e_decoder_error3;
		return false;
	}
}

} // namespace reed_solomon
