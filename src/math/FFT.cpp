#include "FFT.h"

#include <stdlib.h>
#include <math.h>

#include <algorithm>

int sinTab(int num_elements, real_type*& Sin)
{
	int size = 0;
	for (size_t i = 1; i < num_elements; i <<= 1, size++) {};

	Sin = (real_type*)malloc((size + 1) * sizeof(real_type));//new real_type[size + 1];

	int idx = 1;
	Sin[0] = sin(-M_PI);
	for (size_t i = 1; i < num_elements; i <<= 1, idx++)
	{
		Sin[idx] = sin(0.5 * -M_PI / i);
	}

#if 0
	printf("\n\n");
	for (size_t i = 0; i < cnt; i++)
	{
		printf("%f, ", Sin[i]);
	}
	printf("\n\n");
#endif

	return size;
}
int sinTab(int num_elements, int16_t*& Sin)
{
	int size = 0;
	for (size_t i = 1; i < num_elements; i <<= 1, size++) {};

	Sin = (int16_t*)malloc((size + 1) * sizeof(int16_t));

	int idx = 1;
	Sin[0] = (1 << FFT_SCALE_FACTOR) * sin(-M_PI);
	for (size_t i = 1; i < num_elements; i <<= 1, idx++)
	{
		Sin[idx] = ((1 << FFT_SCALE_FACTOR) * sin(0.5 * -M_PI / i));
	}

#if 0
	printf("\n\n");
	for (size_t i = 0; i < cnt; i++)
	{
		printf("%f, ", Sin[i]);
	}
	printf("\n\n");
#endif

	return size;
}
static inline int16_t int16_normolize(int32_t val)
{
	if (val > 32767)
	{
		return 32767;
	}
	if (val < -32767)
	{
		return -32767;
	}

	return (int16_t)val;
}

static void rearrangeData(real_type* data, const size_t num_elements)
{
	size_t target_index = 0;
	size_t bit_mask;

	for (size_t i = 0; i < num_elements; i++)
	{
		if (target_index > i)
		{
			std::swap(data[2 * target_index], data[2 * i]);
			std::swap(data[2 * target_index + 1], data[2 * i + 1]);
		}

		// Initialize the bit mask
		bit_mask = num_elements;

		// While bit is 1
		while (target_index & (bit_mask >>= 1))
		{
			target_index &= ~bit_mask;  // Drop bit:
		}

		// | is bitwise OR
		target_index |= bit_mask;
	}
}
static void makeTransform(real_type* data, const size_t num_elements)
{
	real_type* SinTab = nullptr;
	sinTab(num_elements, SinTab);

	// declare variables to cycle the bits of initial signal
	size_t next, match;
	real_type multR, multI, factorR, factorI, productR, productI;

	real_type* p_sin = SinTab;

	// cycle for all bit positions of initial signal
	for (size_t i = 1; i < num_elements; i <<= 1)
	{
#if 1
		multI = *p_sin++;
		multR = -2.0 * (*p_sin) * (*p_sin);
#else		
		multI = sin(-M_PI / i);
		multR = -2.0 * pow(sin(0.5 * -M_PI / i), 2);
#endif
		next = i << 1;  // getting the next bit		
		factorR = 1.0;   // start transform factor
		factorI = 0;

		for (size_t j = 0; j < i; ++j) // iterations through groups with different transform factors
		{
			for (size_t k = j; k < num_elements; k += next) // iterations through pairs within group
			{
				match = k + i;

				productR = (data[2 * match] * factorR) - (data[2 * match + 1] * factorI);
				productI = (data[2 * match] * factorI) + (data[2 * match + 1] * factorR);

				data[2 * match] = (data[2 * k] - productR);
				data[2 * match + 1] = (data[2 * k + 1] - productI);

				data[2 * k] += productR;
				data[2 * k + 1] += productI;
			}
			
			real_type tmp = (multR * factorR) - (multI * factorI) + factorR;
			factorI = (multR * factorI) + (multI * factorR) + factorI;
			factorR = tmp;
		}
	}

	free(SinTab);	
}

static void rearrangeData(int16_t* data, const size_t num_elements)
{
	uint32_t  target_index = 0;
	uint32_t  bit_mask;

	uint32_t tmp;
	uint32_t* complex_data = (uint32_t*)data;

	for (uint32_t i = 0; i < num_elements; i++)
	{
		if (target_index > i)
		{
			tmp = complex_data[target_index];
			complex_data[target_index] = complex_data[i];
			complex_data[i] = tmp;
		}

		// Initialize the bit mask
		bit_mask = num_elements;

		// While bit is 1
		while (target_index & (bit_mask >>= 1))
		{
			target_index &= ~bit_mask;  // Drop bit:
		}

		// | is bitwise OR
		target_index |= bit_mask;
	}
}
static void makeTransform(int16_t* data, const size_t num_elements)
{
	int16_t* SinTab = nullptr;
	sinTab(num_elements, SinTab);
	int16_t* p_sin = SinTab;

	// declare variables to cycle the bits of initial signal
	uint32_t next, match, cur;
	int32_t multR, multI, factorR, factorI, productR, productI;

	// cycle for all bit positions of initial signal
	for (uint32_t i = 1; i < num_elements; i <<= 1)
	{
		multI = *p_sin++;
		multR = -((int32_t)(*p_sin) * (int32_t)(*p_sin) >> (FFT_SCALE_FACTOR - 1));

		next = i << 1;  // getting the next bit		
		factorR = (1 << FFT_SCALE_FACTOR);   // start transform factor
		factorI = 0;

		for (uint32_t j = 0; j < i; ++j) // iterations through groups with different transform factors
		{
			for (uint32_t k = j; k < num_elements; k += next) // iterations through pairs within group
			{
				match = (k + i) << 1;
				cur = k << 1;

				productR = (((int32_t)data[match] * factorR) - ((int32_t)data[match + 1] * factorI)) >> FFT_SCALE_FACTOR;
				productI = (((int32_t)data[match] * factorI) + ((int32_t)data[match + 1] * factorR)) >> FFT_SCALE_FACTOR;

				data[match] = int16_normolize((int32_t)data[cur] - productR);
				data[match + 1] = int16_normolize((int32_t)data[cur + 1] - productI);

				data[cur] = int16_normolize((int32_t)data[cur] + productR);
				data[cur + 1] = int16_normolize((int32_t)data[cur + 1] + productI);
			}

			productR = ((multR * factorR) - (multI * factorI)) >> FFT_SCALE_FACTOR;
			productI = ((multR * factorI) + (multI * factorR)) >> FFT_SCALE_FACTOR;

			factorR += productR;
			factorI += productI;
		}
	}

	free(SinTab);	
}

bool FFT(real_type* data, const size_t num_elements)
{
	if (!isPowerOfTwo(num_elements))
	{
		return false;
	}

	rearrangeData(data, num_elements);
	makeTransform(data, num_elements);

	return true;
}

bool FFT(int16_t* data, const size_t num_elements)
{
	if (!isPowerOfTwo(num_elements))
	{
		return false;
	}

	rearrangeData(data, num_elements);
	makeTransform(data, num_elements);

	return true;
}