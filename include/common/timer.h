/**
 * @file "timer.h"
 * @author Artem
 * @brief Утилита замера времени выполнения
 * @version 0.1
 * @date 2025-11-14
 *
 * @copyright Copyright (c) 2025
 *
 * @details Предоставляет функции измерения среднего времени и класс таймера для
 * получения дельты между событиями.
 */
#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ctime>
#include <string>

namespace time
{

/**
 * @brief Замеряет среднее время выполнения произвольного
 * @tparam Func Тип вызываемого объекта - функции.
 * @param[in] f Функция, которую требуется измерить.
 * @param[in] iterations Количество повторных запусков.
 * @return Среднее время выполнения в секундах.
 */
template <typename Func>
double measure_time(Func f, int iterations = 1000)
{
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < iterations; ++i)
  {
    f();
  }
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration<double>(end - start).count() / iterations;
}

class Timer
{
  typedef std::chrono::time_point<std::chrono::steady_clock,
                                  std::chrono::nanoseconds>
      time_point;

public:
  /**
   * @brief Конструирует таймер и мгновенно запоминает текущее время.
   */
  Timer() { start_timer(); }

  /**
   * @brief Формирует строку с текущим локальным временем.
   * @return Строка в формате `[HH:MM:SS]: `.
   */
  static inline std::string get_time()
  {

    std::time_t result = std::time(nullptr);
    const std::tm *t = std::localtime(&result);
    return "[" + std::to_string(t->tm_hour) + ":" + std::to_string(t->tm_min) +
           ":" + std::to_string(t->tm_sec) + "]: ";
  }

  /**
   * @brief Сбрасывает таймер и фиксирует текущее время.
   */
  inline void start_timer() { m_time = get_cur_time(); }

  /**
   * @brief Возвращает прошедшее время в секундах с последнего сброса.
   * @return Дельта времени в секундах.
   */
  inline double get_delta_time_sec()
  {
    return static_cast<double>(
               std::chrono::duration_cast<std::chrono::milliseconds>(
                   get_cur_time() - m_time)
                   .count()) /
           1000.;
  }

  /**
   * @brief Возвращает прошедшее время в миллисекундах.
   * @return Дельта времени в миллисекундах.
   */
  inline int64_t get_delta_time_ms()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               get_cur_time() - m_time)
        .count();
  }

  /**
   * @brief Возвращает прошедшее время в наносекундах.
   * @return Дельта времени в наносекундах.
   */
  inline int64_t get_delta_time_ns()
  {
    return (get_cur_time() - m_time).count();
  }

private:
  /**
   * @brief Получает текущее время высокоточного таймера.
   * @return Текущая отметка времени.
   */
  inline time_point get_cur_time() { return std::chrono::steady_clock::now(); }

private:
  time_point m_time; ///< Отметка времени последнего сброса.
};

} // namespace time

#endif // TIMER_H
