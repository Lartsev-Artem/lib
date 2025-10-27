#include "bitmath.h"

#include "macro.h"

#include <assert.h>

#define USE_ASM 0
#define PL_ADDRESS_SIZE 32

uint8_t countBits8 (uint8_t a)
{
#ifndef __GNUC__
	a = a - ((a >> 1) & 0x55);
	a = (a & 0x33) + ((a >> 2) & 0x33);
	a = (a + (a >> 4));
#else
	a = (uint8_t)__builtin_popcount(a);
#endif
	return (a & 0xF);
}

uint8_t countBits16 (uint16_t a)
{
#ifndef __GNUC__
	a = a - ((a >> 1) & 0x5555);
	a = (a & 0x3333) + ((a >> 2) & 0x3333);
	a = (a + (a >> 4)) & 0x0f0f;
	a = a + (a >> 8);
#else
	a = (uint8_t)__builtin_popcount(a);
#endif
	return (a & 0x1F);
}

uint8_t countBits32 (uint32_t a)
{
#ifndef __GNUC__
	a = a - ((a >> 1) & 0x55555555);
	a = (a & 0x33333333) + ((a >> 2) & 0x33333333);
	a = (a + (a >> 4)) & 0x0f0f0f0f;
	a = a + (a >> 8);
	a = a + (a >> 16);
#else
	a = (uint8_t)__builtin_popcount(a);
#endif
	return (a & 0x3F);
}

uint8_t countBits64 (uint64_t a)
{
#ifndef __GNUC__
	a = a - ((a >> 1) & 0x5555555555555555);
	a = (a & 0x3333333333333333) + ((a >> 2) & 0x3333333333333333);
	a = (a + (a >> 4)) & 0x0f0f0f0f0f0f0f0f;
	a = a + (a >> 8);
	a = a + (a >> 16);
	a = a + (a >> 32);
#else
#if (PL_ADDRESS_SIZE == 64)
	a = (uint8_t)__builtin_popcountl(a);
#elif (PL_ADDRESS_SIZE == 32)
	//c_static_assert(pop_int_check, 2 * sizeof(unsigned int) == sizeof(uint64_t));
	a = (uint8_t)(__builtin_popcount(a&0xFFFFFFFF) + __builtin_popcount(a >> 32));
#else
#error pop_int_check
#endif
#endif
	return (a & 0x7F);
}

uint8_t countLeadZeroBits8 (uint8_t a)
{
#if !USE_ASM

	if (a == 0)
	{
		return 8;
	}

#ifndef __GNUC__
	uint8_t n = 1;

	if ((a >> 4) == 0)
	{
		n += 4;
		a <<= 4;
	}

	if ((a >> 6) == 0)
	{
		n += 6;
		a <<= 6;
	}

	n -= (a >> 7);
	return n;
#else
	return (uint8_t)__builtin_clz((unsigned int)a << 24);
#endif
#else
	uint32_t val = ((uint32_t)a << 24) | 0x800000;
	uint32_t n;
	asm volatile (" clz %0,%1" :"=r"(n): "r"(val));
	return n;
#endif
}

uint8_t countLeadZeroBits16 (uint16_t a)
{
#if !USE_ASM

	if (a == 0)
	{
		return 16;
	}

#ifndef __GNUC__
	uint8_t n = 1;

	if ((a >> 8) == 0)
	{
		n += 8;
		a <<= 8;
	}

	if ((a >> 12) == 0)
	{
		n += 4;
		a <<= 4;
	}

	if ((a >> 14) == 0)
	{
		n += 2;
		a <<= 2;
	}

	n -= (a >> 15);
	return n;
#else
	return (uint8_t)__builtin_clz((unsigned int)a << 16);
#endif
#else
	uint32_t val = (((uint32_t) a) << 16) | 0x8000;
	uint32_t n;
	asm volatile (" clz %0,%1" :"=r"(n): "r"(val));
	return n;
#endif
}

uint8_t countLeadZeroBits32 (uint32_t a)
{
#if !USE_ASM

	if (a == 0)
	{
		return 32;
	}

#ifndef __GNUC__
	uint8_t n = 1;

	if ((a >> 16) == 0)
	{
		n += 16;
		a <<= 16;
	}

	if ((a >> 24) == 0)
	{
		n += 8;
		a <<= 8;
	}

	if ((a >> 28) == 0)
	{
		n += 4;
		a <<= 4;
	}

	if ((a >> 30) == 0)
	{
		n += 2;
		a <<= 2;
	}

	n -= (a >> 31);
	return n;
#else
	return (uint8_t)__builtin_clz(a);
#endif
#else
	uint32_t n;
	asm volatile (" clz %0,%1" :"=r"(n): "r"(a));
	return n;
#endif

}

uint8_t countLeadZeroBits64 (uint64_t a)
{

#if !USE_ASM

	if (a == 0)
	{
		return 64;
	}

#ifndef __GNUC__
	uint8_t n = 1;

	if ((a >> 32) == 0)
	{
		n += 32;
		a <<= 32;
	}

	if ((a >> 48) == 0)
	{
		n += 16;
		a <<= 16;
	}

	if ((a >> 56) == 0)
	{
		n += 8;
		a <<= 8;
	}

	if ((a >> 60) == 0)
	{
		n += 4;
		a <<= 4;
	}

	if ((a >> 62) == 0)
	{
		n += 2;
		a <<= 2;
	}

	n -= (a >> 63);
	return n;
#else
#if PL_ADDRESS_SIZE == 64
	return (uint8_t)__builtin_clzll(a);
#elif PL_ADDRESS_SIZE == 32
	uint8_t res = countLeadZeroBits32(a >> 32);

	if (UNLIKELY(res == 32))
	{
		res += countLeadZeroBits32(a&0xFFFFFFFF);
	}

	return res;
#else
#error clz_uint_check
#endif
#endif
#else
	uint32_t n;
	uint32_t val = (a >> 32);
	uint32_t r;
	asm volatile (" clz %0,%1" :"=r"(r): "r"(val));
	n = r;

	if (UNLIKELY(r == 32))
	{
		val = (uint32_t)a;
		asm volatile (" clz %0,%1" :"=r"(r): "r"(val));
		n += r;
	}

	return n;
#endif
}

uint32_t _abs32_lost_bit(uint32_t a)
{
	return (~((a >> (sizeof(a)*8-1)) - 1)) ^ a;
}

//точный модуль
uint32_t _abs32(int32_t a)
{
	uint32_t mask = (uint32_t)a >> (sizeof(a) * 8 - 1);
	return ((uint32_t)a + mask) ^ mask;
}

int32_t _max32(int32_t a, int32_t b)
{
	return a ^ ((a ^ b) & -(a< b)); //max
}

int32_t _min32(int32_t a, int32_t b)
{
	return a ^ ((a ^ b) & -(a > b)); //min
}

//относительная ошибка порядка 5%, абсолютная <0.5
float _log2f(float x)
{
#if 0
	union
	{
		float x;
		struct
		{
			uint32_t mant : 23;
			uint32_t exp : 8;
			uint32_t sign : 1;
		};
	}ax;

	ax.x = x;
	int exp = ax.exp - 127;
	ax.sign = 0;
	ax.exp = 127;
	x = ax.x;
#else
	uint32_t ux = *((uint32_t*)&x);

	int exp = ((ux >> 23) & 0xFF) - 127;
	ux &= 0x3FFFFFFF;
	ux |= 0x3f800000;

	x = *((float*)&ux);
#endif

	constexpr float LOG2E = 1.4426950408889634f;//log2f(expf(1.0f));
	return (x - 1.0f) * LOG2E + exp;
}

uint32_t _log2u(uint32_t a)
{
	return 31 - countLeadZeroBits32(a);
}

/**
 * @brief Целочисленный двоичный логарифм с сохранением младших бит
 * @param val --- значение под логарифмом
 * @param bits --- число учитываемых младших бит
 * @return (2^bits)*log2(val)
 */
uint32_t _log2u(uint32_t val, uint32_t bits)
{
	uint32_t n = 31 - countLeadZeroBits32(val); //целая часть

	uint32_t a = n << bits;
	for(uint32_t i = 1; i <= bits; i++)
	{
		a += (((val >> (--n)) & 0x1) << (bits - i));
	}

	return a;
}

float _inv_sqrt(const float a)
{
	int i;
	float x2, y;

	y = a;
	i = *(int*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;

	constexpr float threehalf = 1.5f;
	x2 = a * 0.5f;
	y = y * (threehalf - (x2 * y * y));
	return y;
}

uint32_t _sqrt32(uint32_t value)
{
	if (value <= 1)
	{
		return value;
	}

	uint16_t s;
	uint32_t g0, g1, x1;
	s = 1;
	x1 = value - 1;

	if (x1 > 65535)
	{
		s = s + 8;
		x1 = x1 >> 16;
	}

	if (x1 > 255)
	{
		s = s + 4;
		x1 = x1 >> 8;
	}

	if (x1 > 15)
	{
		s = s + 2;
		x1 = x1 >> 4;
	}

	if (x1 > 3)
	{
		s = s + 1;
	}

	g0 = (1 << s);
	g1 = (g0 + (value >> s)) >> 1;

	while (g1 < g0)
	{
		g0 = g1;
		g1 = (g0 + (value / g1)) >> 1;
	}

	if (g0 * (g0 + 1) < value)
	{
		g0++;
	}
	else
		if (g0 * (g0 - 1) > value)
		{
			g0--;
		}

	return g0;
}

// sqrt(a^2+b^2)
uint32_t sqrt_4375(uint32_t d1, uint32_t d2)
{
	d1 ^= (~((d1 >> 31) - 1));
	d2 ^= (~((d2 >> 31) - 1));

	uint32_t cond = -(d1 < d2);
	uint32_t tmp = (d1 ^ d2);

	d2 = (d1 ^ (tmp & cond));
	d1 = (d1 ^ (tmp & (~cond)));

	d1 = (d1 * d1) / (d2 + (d2 == 0));
	return d2 + (d1 >> 1) - (d1 >> 4);

	/*unsigned int max, min, mn;
	d1 = abs32(d1);
	d2 = abs32(d2);

	if (d1 > d2)
	{
		max = d1;
		min = d2;
	}
	else
	{
		max = d2;
		min = d1;
	}

	if (max == 0)
	{
		return 0;
	}

	mn = min * min / max;
	return max + (mn >> 1) - (mn >> 4);*/
}

// sqrt(a^2-b^2)
int64_t sqrt_dif(const int64_t a, const int64_t b, const int num_of_iter)
{
	assert(b <= a);

	const int64_t y = b * b - a * a;
	int64_t S1 = a - (b * b + a) / (a << 1);

	for (int i = 0; i < 0; i++)
	{
		S1 -= (S1 * S1 + y) / (S1 << 1);
	}
	return S1;
}