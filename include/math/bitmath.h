#ifndef BITMATH_H
#define BITMATH_H

#include <stdint.h>

#define IF_NO_BRANCH(cond, a,b, res)\
{\
    uint32_t reg = (cond) - 1;\
    res = ((((uint32_t)(a)) & (~reg)) + ((b) & reg));\
}

uint8_t countBits8 (uint8_t a);
uint8_t countBits16 (uint16_t a);
uint8_t countBits32 (uint32_t a);
uint8_t countBits64 (uint64_t a);

uint8_t countLeadZeroBits8 (uint8_t a);
uint8_t countLeadZeroBits16 (uint16_t a);
uint8_t countLeadZeroBits32 (uint32_t a);
uint8_t countLeadZeroBits64 (uint64_t a);

//! Модуль с потерей младшего бита
uint32_t _abs32_lost_bit(uint32_t a);

//! Точный модуль
uint32_t _abs32(int32_t a);

//! максимум двух чисел
int32_t _max32(int32_t a, int32_t b);

//! минимум двух чисел
int32_t _min32(int32_t a, int32_t b);

//! Относительная ошибка порядка 5%, абсолютная < 0.5
float _log2f(float x);
uint32_t _log2u(uint32_t a);
uint32_t _log2u(uint32_t val, uint32_t bits);

//! Обратный корень 1/sqrt(a)
float _inv_sqrt(const float a);

//! целочисленный корень
uint32_t _sqrt32(uint32_t value);

///! sqrt(a^2+b^2)
uint32_t sqrt_4375(uint32_t d1, uint32_t d2);

#endif //BITMATH_H
