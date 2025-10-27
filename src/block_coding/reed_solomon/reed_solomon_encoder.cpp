#include "reed_solomon_encoder.h"

namespace reed_solomon
{

static bool make_sequential_root_generator_polynomial(const galois::field& field,
		const uint32_t initial_index,
		const uint32_t num_elements,
		galois::field_polynomial& generator_polynomial)
{
	if ((initial_index >= field.size()) ||
			((initial_index + num_elements) >  field.size()))
	{
		return false;
	}

	const galois::field_symbol alpha = 2;
	const galois::field_symbol xgfe[2] = { 0, 1};
	const galois::field_polynomial X(field, 1, xgfe);
	generator_polynomial = galois::field_polynomial(1, field);

	for (uint32_t i = initial_index; i < (initial_index + num_elements); ++i)
	{
		galois::field_polynomial Y(X);
		Y += field.exp(alpha, static_cast<galois::field_symbol>(i));
		generator_polynomial *= Y;
	}

	return true;
}


Encoder::Encoder(uint32_t code_len, uint32_t fec_len, const galois::field& gfield,
				 uint32_t generator_polynomial_index,
				 uint32_t generator_polynomial_root_count):
	code_length(code_len),
	fec_length(fec_len),
	data_length(code_len - fec_len),
	natural_length(255),
	padding_length(natural_length - data_length - fec_len),
	field_(gfield),
	generator_(gfield)
{
	DIE_IF(natural_length != gfield.size());

	if ( !make_sequential_root_generator_polynomial(
				field_,
				generator_polynomial_index,
				generator_polynomial_root_count,
				generator_))
	{
		DPRINTF("Error - Failed to create sequential root generator!");
		D_LD;
	}

	rsblock_ = new uint8_t[natural_length]; //galois::field_symbol
}

Encoder::~Encoder()
{

	delete[] rsblock_;
}

void Encoder::encode(const uint8_t* data, uint8_t* rsblock_out)// (block_type& rsblock) const
{
	memset(&rsblock_[0], 0, padding_length * sizeof(rsblock_[0]));

	for (uint32_t i = 0; i < data_length; ++i)
	{
		rsblock_[padding_length + i] = data[i];
	}

	const galois::field_symbol  mask = field_.size();

	galois::field_polynomial parities(field_, natural_length);

	for (uint32_t i = fec_length; i < natural_length; ++i)
	{
		parities[i] = rsblock_[natural_length - 1 - i] & mask;
	}

	parities %= generator_;

	for (uint32_t i = 0; i < data_length; ++i)
	{
		rsblock_out[i] = rsblock_[padding_length + i];
	}

	for (uint32_t i = 0; i < fec_length; ++i)
	{
		rsblock_out[data_length + i] =  parities[fec_length - 1 - i] & mask;
	}
}

} // namespace reed_solomon
