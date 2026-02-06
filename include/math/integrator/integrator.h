/**
 * @file integrator.h
 * @author Artem ()
 * @brief Интегрирование функций на отрезке
 * @version 0.1
 * @date 2025-12-13
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <functional>
#include <vector>
#include <assert.h>

/**
 * @brief Класс предоставляющий алгоритмы интегрирования функций одной
 * переменной
 */
template <typename T>
class Integrator_t
{
public:
  Integrator_t(const int size)
  {
    std::vector<T> w, n;
    int res = get_gauss_points(size, n, w);
    assert(res == 0);

    _size = size;
    if (size % 2 == 1)
    {
      _nodes.assign(n.begin(), n.end());
      _weights.assign(w.begin(), w.end());
    }
    else
    {
      int N = size / 2;
      _weights.resize(N);
      _nodes.resize(N);
      for (size_t i = 0; i < N; i++)
      {
        _weights[i] = (w[N + i]);
        _nodes[i] = (n[N + i]);
      }
    }
  }

  T Integrate(T a, T b, const std::function<T(T)> &f)
  {
    if (_size % 2 == 1)
    {
      return __integrate(a, b, f);
    }
    else
    {
      return __integrate_symmetric(a, b, f);
    }
  }

private:
  T __integrate_symmetric(T a, T b, const std::function<T(T)> &f)
  {
    T sum = 0.0;
    T shift = 0.5 * (a + b);
    T scale = 0.5 * (b - a);
    size_t n = _nodes.size();

    for (int i = 0; i < n; ++i)
    {
      T npos = scale * _nodes[i];
      T x_pos = npos + shift;
      T x_neg = -npos + shift;
      sum += _weights[i] * (f(x_pos) + f(x_neg));
    }

    return sum * scale;
  }

  T __integrate(T a, T b, const std::function<T(T)> &f)
  {
    T sum = 0.0;
    T shift = 0.5 * (a + b);
    T scale = 0.5 * (b - a);
    size_t n = _nodes.size();

    for (int i = 0; i < n; ++i)
    {
      T x = scale * _nodes[i] + shift;
      sum += _weights[i] * f(x);
    }

    return sum * scale;
  }

private:
  /**
   * @brief Вычисление полиномов Лежандра
   *
   * @param[in] n Степень полинома
   * @param[in] x точка
   * @return пара полином - производная
   */
  std::pair<T, T> legendre_poly(int n, T x)
  {
    if (n == 0)
      return {1.0, 0.0};
    if (n == 1)
      return {x, 1.0};

    T p0 = 1.0;
    T p1 = x;
    T p2;

    for (int k = 2; k <= n; ++k)
    {
      p2 = ((2 * k - 1) * x * p1 - (k - 1) * p0) / k;
      p0 = p1;
      p1 = p2;
    }

    // Вычисление производной: P_n'(x) = n*(P_{n-1}(x) - x*P_n(x))/(1-x^2)
    T deriv = n * (p0 - x * p1) / (1 - x * x);
    return {p1, deriv};
  };

  /**
   * @brief Вычисление квадратурных точек гаусса
   *
   * @param[in] n кол-во точек
   * @param[out] nodes узлы
   * @param[out] weights веса
   * @note Точна для полиномов степени 2n-1
   * @return код ошибки
   */

  int get_gauss_points(int n, std::vector<T> &nodes, std::vector<T> &weights)
  {
    constexpr int max_iter = 100;
    constexpr T eps = std::numeric_limits<T>::epsilon() * 10;

    if (n <= 0)
    {
      return 1;
    }

    nodes.resize(n);
    weights.resize(n);
    bool flag_fail = false;

    // Инициализация узлов приближениями Чебышева
    for (int i = 0; i < n; ++i)
    {
      nodes[i] = std::cos(math::PI * (4 * (i + 1) - 1) / (4 * n + 2));
    }

    // Уточнение узлов методом Ньютона
    for (int i = 0; i < n; ++i)
    {
      T x = nodes[i];
      int iter = 0;

      while (iter < max_iter)
      {
        auto [Pn, dPn] = legendre_poly(n, x);
        if (std::abs(dPn) < eps)
          break;

        T dx = Pn / dPn;
        x -= dx;

        if (std::abs(dx) < eps)
          break;
        ++iter;
      }

      if (iter >= max_iter)
      {
        flag_fail = true;
      }

      nodes[i] = x;

      // Вычисление весов по формуле: w_i = 2/((1-x_i^2)*[P_n'(x_i)]^2)
      auto [Pn, dPn] = legendre_poly(n, x);
      weights[i] = 2.0 / ((1.0 - x * x) * dPn * dPn);
    }

    if (flag_fail)
    {
      weights.clear();
      nodes.clear();
      return 1;
    }

    // Сортировка узлов по возрастанию
    std::vector<std::pair<T, T>> pairs(n);
    for (int i = 0; i < n; ++i)
    {
      pairs[i] = {nodes[i], weights[i]};
    }

    std::sort(pairs.begin(), pairs.end(),
              [](const std::pair<T, T> &a, const std::pair<T, T> &b)
              { return a.first < b.first; });

    for (int i = 0; i < n; ++i)
    {
      nodes[i] = pairs[i].first;
      weights[i] = pairs[i].second;
    }

    return 0;
  }

private:
  size_t _size;
  std::vector<T> _weights; ///< веса квадратурной формулы
  std::vector<T> _nodes;   ///< узлы квадратурной формулы
};

#endif // INTEGRATOR_H
