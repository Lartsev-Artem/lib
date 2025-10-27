#ifndef REED_SOLOMON_DECODER_H
#define REED_SOLOMON_DECODER_H

#include "galois_field_polynomial.h"

namespace reed_solomon
{

struct state_t
{
	enum error_t
	{
		e_no_error       = 0,
		e_decoder_error0 = 1,
		e_decoder_error1 = 2,
		e_decoder_error2 = 3,
		e_decoder_error3 = 4
	};

	void reset()
	{
		errors_detected  = 0;
		errors_corrected = 0;
		zero_numerators  = 0;
		unrecoverable    = false;
		error            = e_no_error;
	}

	bool unrecoverable;
	uint8_t errors_detected;
	uint8_t errors_corrected;
	uint8_t zero_numerators;
	uint8_t error;
};


class Decoder
{
private:
	Decoder();
	Decoder(const Decoder& dec);
	Decoder& operator=(const Decoder& dec);

public:

	Decoder(uint32_t code_len, uint32_t fec_len, const galois::field& field, const unsigned int gen_initial_index = 0);
	~Decoder();

	bool decode(const uint8_t* rsblock, uint8_t* decode_data);

private:
	void create_lookup_tables();

	int compute_syndrome(const galois::field_polynomial& received,
						 galois::field_polynomial& syndrome) const;

	void find_roots(const galois::field_polynomial& poly, galois::symbol_vector<uint16_t>& root_list) const;

	void compute_discrepancy(galois::field_symbol&          discrepancy,
							 const galois::field_polynomial& lambda,
							 const galois::field_polynomial& syndrome,
							 const uint32_t&              l,
							 const uint32_t&              round) const;


	void modified_berlekamp_massey_algorithm(galois::field_polynomial& lambda,
			const galois::field_polynomial& syndrome) const;


	bool forney_algorithm(const galois::symbol_vector<uint16_t>& error_locations,
						  const galois::field_polynomial& lambda,
						  const galois::field_polynomial& syndrome);

public:
	inline const char* error_as_string() const
	{
		switch (st.error)
		{
			case state_t::e_no_error:
				return "No Error";

			case state_t::e_decoder_error0:
				return "Decoder Failure - Non-zero Syndrome";

			case state_t::e_decoder_error1:
				return "Decoder Failure - Too Many Errors/Erasures";

			case state_t::e_decoder_error2:
				return "Decoder Failure - Invalid Symbol Correction";

			case state_t::e_decoder_error3:
				return "Decoder Failure - Invalid Codeword Correction";

			default:
				return "Invalid Error Code";
		}
	}

private:
	const uint32_t code_length;
	const uint32_t fec_length;
	const uint32_t data_length;
	const uint32_t natural_length;
	const uint32_t padding_length;
	const unsigned int                    gen_initial_index_;
	const galois::field&                  field_;

	state_t st;
	galois::symbol_vector<galois::field_symbol>     root_exponent_table_;
	galois::symbol_vector<galois::field_symbol>     syndrome_exponent_table_;
	uint8_t* rsblock_;
};

} // namespace reed_solomon

#endif  ///REED_SOLOMON_DECODER_H
