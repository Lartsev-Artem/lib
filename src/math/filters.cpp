#include <iostream>
using namespace std;

#define PI 3.1415926535897931
#define sizeof_array(array)  (sizeof(array)/sizeof((array)[0]))

static void memcpy_int16_t(void* dst, const void* src, unsigned int len)
{
#ifdef TECH_MODE

	if (((uint32_t)dst & 1) || ((uint32_t)src & 1))
	{
		HAL_fatal_error("memcpy_int16_t not aligned d=%08x, s=%08x len=%d",
			dst, src, len);
	}

#endif

	int16_t* d = (int16_t*)dst;
	const int16_t* s = (const int16_t*)src;
	const int16_t* e = (const int16_t*)src + len;
#if MMONITOR == 1
	check_ram_bounds(dst, (const int16_t*)dst + len);
#endif

	while (s < e)
	{
		*d++ = *s++;
	}
}

void generic_fir_ii(int16_t* dst, const int16_t* src, int len, const int* filter, int f_len, int16_t* dbuf)
{	
	//if (dbuf == NULL)
	//{
	//	//FIXME: rise exeption!
	//	HAL_fatal_error("fir_ff: dbuf==NULL!!");
	//	//while (1);
	//}

	for (int i = 0; i < len; i += 1)
	{		
		int64_t aa = 0;
		int k = 0;
		int s = i - f_len + 1;

		if (s < 0)
		{
			for (s = i; s < f_len - 1; k++, s++)
			{
				aa += (int64_t)dbuf[s] * (int64_t)filter[k];
			}

			s = 0;
		}

		for (; k < f_len; k++, s++)
		{
			aa += (int64_t)src[s] * (int64_t)filter[k];
		}
		
		aa >>= 20;

		int bb = (int)(aa); //   ( bb - int )

		if (bb > 32767)
		{
			bb = 32767;
		}

		if (bb < -32767)
		{
			bb = -32767;
		}

		*dst++ = (int16_t)bb;
	}

	if (f_len > len)
	{
		//FIXME: проверить бы ситуации когда фильтр больше входного буффера
		int cpy_len = f_len - 1 - len;
		int i;

		for (i = 0; i < cpy_len; i++) //memmove
		{
			dbuf[i] = dbuf[i + len];
		}

		memcpy_int16_t(dbuf + cpy_len, src, len);
	}
	else
	{
		int shift_len = len - f_len + 1;
		memcpy_int16_t(dbuf, src + shift_len, f_len - 1);
	}
}