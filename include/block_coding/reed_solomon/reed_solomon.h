#ifndef REED_SOLOMON_H
#define REED_SOLOMON_H

#include "reed_solomon_encoder.h"
#include "reed_solomon_decoder.h"

namespace reed_solomon
{

class RsCoder_t
{
private:
	static constexpr uint32_t code_length = 18;
	static constexpr uint32_t fec_length  = 8;
	static constexpr uint32_t data_length = code_length - fec_length;

public:
	static constexpr uint32_t GetCodeLength()
	{
		return code_length;
	}
	static constexpr uint32_t GetDataLength()
	{
		return data_length;
	}

	RsCoder_t(bool encoder_enable, bool decoder_enable);

	~RsCoder_t();

	uint8_t* Encode(const uint8_t* data,  uint32_t& size);
	uint8_t* Decode(const uint8_t* data, uint32_t& size);

private:
	Encoder* encoder;
	Decoder* decoder;
	const galois::field field_;
};

} // namespace reed_solomon
#endif
