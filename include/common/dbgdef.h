#ifndef DBGDEF_H
#define DBGDEF_H

#include <stdio.h>
#include <stdlib.h>

#if __GNUC__
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#endif

#define DPRINTF(...) printf(__VA_ARGS__)

//! @brief Печать позиции в коде
#define D_L DPRINTF( "D_L: [%s %s, %d .c",   __FILE__,__FUNCTION__,__LINE__)
  

//! @brief Прерывание работы программы с указанием места

#define D_LD   \
    do  {\
    DPRINTF( "[%s %s, %d .c\n __fatal_error!!!",   __FILE__,__FUNCTION__,__LINE__);\
    abort();\
    } while (0)

#define DIE_IF(_cond)                                                          \
  do                                                                           \
  {                                                                            \
    if (UNLIKELY(_cond))                                                       \
    {                                                                          \
      DPRINTF("DIE_IF:");                                                      \
      D_LD;                                                                    \
    }                                                                          \
  } while (0)


#endif //! DBGDEF_H
