#include <iostream>
using namespace std;

#include "macro.h"
#define PI 3.1415926535897931

void generic_fir_ii(int16_t* dst, const int16_t* src, int len, const int* filter, int f_len, int16_t* dbuf)
{
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

		memcpy(dbuf + cpy_len, src, len * sizeof(int16_t));
	}
	else
	{
		int shift_len = len - f_len + 1;
		memcpy(dbuf, src + shift_len, (f_len - 1) * sizeof(int16_t));
	}
}