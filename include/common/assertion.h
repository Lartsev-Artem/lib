#ifndef ASSERTION_H
#define ASSERTION_H

// Вспомогательные макросы для подсчета аргументов
#define ASSERT_GET_ARG_COUNT(...)                                              \
  ASSERT_GET_ARG_COUNT_IMPL(__VA_ARGS__, 2, 1, 0)
#define ASSERT_GET_ARG_COUNT_IMPL(_1, _2, count, ...) count

// Версия с одним аргументом (только условие)
#define ASSERT1(_cond)                                                         \
  do                                                                           \
  {                                                                            \
    if (UNLIKELY(!(_cond)))                                                    \
    {                                                                          \
      DPRINTF("\nASSERT: %s\n", #_cond);                                           \
      D_LD;                                                                    \
    }                                                                          \
  } while (0)

// Версия с двумя аргументами (условие и сообщение)
#define ASSERT2(_cond, text)                                                   \
  do                                                                           \
  {                                                                            \
    if (UNLIKELY(!(_cond)))                                                    \
    {                                                                          \
      DPRINTF("\nASSERT: %s\n Msg: %s\n", #_cond, (text));                               \
      D_LD;                                                                    \
    }                                                                          \
  } while (0)

//! Главный макрос с выбором реализации
// #define ASSERT(...) \
  ASSERT_SELECTOR(ASSERT_GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)

#define ASSERT_SELECTOR(count, ...) ASSERT_SELECTOR_IMPL(count, __VA_ARGS__)
#define ASSERT_SELECTOR_IMPL(count, ...) ASSERT##count(__VA_ARGS__)

// "Проглатывает" лишние аргументы (если их передано больше 2)
#define ASSERT_IGNORE(...) ((void)0)

#endif //! ASSERTION_H