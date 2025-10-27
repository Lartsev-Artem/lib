#include "reed_solomon.h"

namespace reed_solomon
{
/* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 0x^4 + 0x^5 + 1x^6 */
static constexpr unsigned int primitive_polynomial[]    = {1, 1, 0, 0, 0, 0, 1};
static constexpr unsigned int primitive_polynomial_size = 7;

RsCoder_t::RsCoder_t(bool encoder_enable, bool decoder_enable) :
	encoder(nullptr),
	decoder(nullptr),
	field_(primitive_polynomial_size + 1, primitive_polynomial)
{
	if (encoder_enable)
	{
		encoder = new Encoder(code_length, fec_length, field_);
	}

	if (decoder_enable)
	{
		decoder = new Decoder(code_length, fec_length, field_);
	}
}

RsCoder_t::~RsCoder_t()
{
	if (encoder)
	{
		delete encoder;
	}

	if (decoder)
	{
		delete decoder;
	}
}

uint8_t* RsCoder_t::Encode(const uint8_t* data,  uint32_t& size)
{
	if (!encoder)
	{
		size = 0;
		return nullptr;
	}

	uint32_t num_blocks = (size + data_length - 1) / data_length;
	uint32_t full_blocks = size / data_length;
	uint32_t last_block = size - full_blocks * data_length;

	uint8_t* encode_data = new uint8_t[num_blocks * code_length];
	uint8_t* encode_ptr = encode_data;

	for (uint32_t i = 0; i < full_blocks; i++)
	{
		encoder->encode(data, encode_ptr);
		data += data_length;
		encode_ptr += code_length;
	}

	if (last_block)
	{
		uint8_t tmp[data_length];
		memcpy(tmp, data, last_block);
		memset(&tmp[last_block], 0, data_length - last_block);
		encoder->encode(tmp, encode_ptr);
		encode_ptr += code_length;
	}

	size = (uint32_t)(encode_ptr - encode_data);
	return encode_data;
}

uint8_t* RsCoder_t::Decode(const uint8_t* data, uint32_t& size)
{
	if (!decoder)
	{
		size = 0;
		return nullptr;
	}

	uint32_t full_blocks = size / code_length;
	uint8_t* decode_data = new uint8_t[full_blocks * data_length];
	uint8_t* decode_ptr = decode_data;

	for (uint32_t i = 0; i < full_blocks; i++)
	{
		if (!decoder->decode(data, decode_ptr))
		{
			DPRINTF("%s\n", decoder->error_as_string());
		}

		data += code_length;
		decode_ptr += data_length;
	}

	size = (uint32_t)(decode_ptr - decode_data);
	return decode_data;
}

} // namespace reed_solomon
