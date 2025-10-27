#ifndef REED_SOLOMON_ENCODER_H
#define REED_SOLOMON_ENCODER_H

#include "galois_field_polynomial.h"

namespace reed_solomon
{
class Encoder
{
public:

	Encoder(uint32_t code_len, uint32_t fec_len, const galois::field& gfield,
			uint32_t generator_polynomial_index = 0,
			uint32_t generator_polynomial_root_count = 8);

	~Encoder();

	void encode(const uint8_t* data, uint8_t* rsblock_out);

private:
	Encoder();
	Encoder(const Encoder& enc);
	Encoder& operator=(const Encoder& enc);
private:
	const uint32_t code_length;
	const uint32_t fec_length;
	const uint32_t data_length;
	const uint32_t natural_length;
	const uint32_t padding_length;

	const galois::field&           field_;
	galois::field_polynomial generator_;
	uint8_t* rsblock_;
};

} // namespace reed_solomon


#endif //REED_SOLOMON_ENCODER_H
