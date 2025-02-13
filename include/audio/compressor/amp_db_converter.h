#ifndef AMP_DB_CONVERTER_H
#define AMP_DB_CONVERTER_H

#include <stdint.h>

#include "bitmath.h"
#include <math.h>
#include "macro.h"
namespace compressor
{
    static inline float to_db0(float lin)
    {
        return 20*log10f(lin);
    }

    static inline float to_lin0(float db)
    {
        return powf(10, 0.05f*db);
    }

    static _INLINE(uint32_t to_db(uint32_t lin));
    static inline uint32_t to_db(uint32_t lin)
    {
        constexpr uint32_t factor = 10;
        constexpr uint32_t inv_log2_10 = 6165; // (2^10*20/log2(10))

#if 1
        uint32_t n = 31 - countLeadZeroBits32(lin);
#else
        uint32_t n;
        asm volatile (" clz %0,%1" :"=r"(n): "r"(lin));
        n = 31 - n;
#endif

        uint32_t a = n << factor;
        a += (((lin >> (--n)) & 0x1) << (factor - 1));
        a += (((lin >> (--n)) & 0x1) << (factor - 2));
        a += (((lin >> (--n)) & 0x1) << (factor - 3));
        return ((inv_log2_10 * a) >> factor);
    }

}



#endif // ! AMP_DB_CONVERTER_H
