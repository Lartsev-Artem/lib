#ifndef MACRO_H
#define MACRO_H

/// Определение макросов предсказания перехода
#ifdef __GNUC__
#define LIKELY(expr) __builtin_expect(!!(expr), 1)
#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#else
#define LIKELY(expr)    (expr)
#define UNLIKELY(expr)  (expr)
#endif

/// Макрос для получения числа элементов одномерного статического массива произвольного типа
#ifndef sizeof_array
#define sizeof_array(array)  (sizeof(array)/sizeof((array)[0]))
#endif


/// Макрос встраеваемой функции
#ifdef __GNUC__
#define _INLINE(_function) inline _function __attribute__ ((always_inline))
#else
#define _INLINE(_function) inline _function
#endif

/// Макрос выравнивания по границе
#ifdef __GNUC__
#define _ALIGN(N)  __attribute__((aligned(N)))
#else
#define _ALIGN(N)  __declspec(align(N))
#endif


/// Макрос перевода с cтроку
#define TO_STR(a) #a

#define __STR_CONCAT(x,y) x##y
/// Макрос конкатенации строк с подстановкой
#define STR_CONCAT(x,y) __STR_CONCAT(x,y)

/// Статическая проверка выражения
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif
#ifndef c_static_assert
#define c_static_assert(name, cond) enum { STR_CONCAT(name##_##assertion_, __LINE__) = 1 / (!!(cond)) };
#endif
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif //MACRO_H
