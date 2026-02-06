#ifndef BIT_PACK_H
#define BIT_PACK_H

#include <math.h>
#include "bitmath.h"

//eps = 0.1, max_err=2648
//log(x)*2^2
inline uint8_t PackX4(uint16_t lin)
{
	constexpr uint32_t factor = 10;
	uint16_t log = lin ^ (~((lin >> 15) - 1));
	uint16_t n = 15 - countLeadZeroBits16(log);

	uint16_t a = n << factor;
	a += (((log >> (--n)) & 0x1) << (factor - 1));
	a += (((log >> (--n)) & 0x1) << (factor - 2));
	return ((lin & 0x8000) | a) >> 8;
}
inline int16_t UnpackX4(uint8_t log)
{
	float abs = (log & 0x7F);
	float x = 1.125f * powf(2.0f, 0.25f * abs);
	x = std::isnormal(x) ? x : 0;
	return (log & 0x80) ? -x : x;
}

//eps = 0.25, max_err=4000
//log(x)*2^1
inline uint8_t PackX2(uint16_t lin)
{
	constexpr uint32_t factor = 9;
	uint16_t abs = lin ^ (~((lin >> 15) - 1));      //abs(lin)
	uint16_t n = 15 - countLeadZeroBits16(abs);     //(int)log2(x)

	uint16_t a = n << factor;
	a += (((abs >> (--n)) & 0x1) << (factor - 1));  //a=(int)(2*log2(x))
	return ((lin & 0x8000) | a) >> 8;               //a | sign(lin)
}
inline int16_t UnpackX2(uint8_t log)
{
	float abs = (log & 0x7F);           //abs(x)
	float x = powf(2.0, 0.5 * abs);     //2^log2(x)
	x *= 1.25f;                         //поправка из norm(x-x0)->min
	x = std::isnormal(x) ? x : 0;       //защита от log(0)
	return (log & 0x80) ? -x : x;       //востановление знака
#if 0
	//ниже точность
	int32_t abs = (log & 0x7F);
	int x = (1 << (abs >> 1));
	x = (5 * x) >> 2;
	return (log & 0x80) ? -x : x;
#endif
}

//5bit
//int(log(x))
inline uint8_t PackX(uint16_t lin)
{
	uint16_t abs = lin ^ (~((lin >> 15) - 1));      //abs(lin)
	uint16_t n = 15 - countLeadZeroBits16(abs);  //(int)log2(x)
	uint16_t a = ((lin & 0x8000) >> 11) | n;             //a | sign(lin)
	return a & 0x1F;
}
inline float UnpackX(uint8_t log)
{
	float abs = (log & 0xF);           //abs(x)
	float x = powf(2.0, abs);     //2^log2(x)
	x *= 1.25f;                         //поправка из norm(x-x0)->min
	x = std::isnormal(x) ? x : 0;       //защита от log(0)
	return (log & 0x10) ? -x : x;       //востановление знака
}

//16 + 6 = 22 бит
//1024*whole(6,bit) + frac*2^6(16,bit)
//frac =[0,1023].1234
inline uint32_t PackFloat_1024(float v)
{
	int32_t whole = v / 1024.0f;
	uint32_t res = (whole & 0x3F) << 16;
	uint16_t frac = 64.0f * (v - 1024 * whole);
	return res | frac;
}
inline float UnpackFloat_1024(int8_t whole, uint16_t frac)
{
	return whole * 1024.0f + (float)frac / 64.0f;
}


#endif // BIT_PACK_H
