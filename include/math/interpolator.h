/**
 * @file interpolator.h
 * @author Artem ()
 * @brief Табличное задание функций
 * @version 0.1
 * @date 2025-10-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H
#include <algorithm>
#include <assert.h>
#include <functional>
#include <math.h>

/**
 * @brief Класс обертка для табличной интерполяции
 *
 * @tparam T тип данных с плавающей точкой
 * @tparam idxGetter - функция определяющая обратное преобразование координаты в
 * индекс (по умолчанию определен для линейной равномерной сетке)
 * @tparam _irregular_grid - признак регулярности сетки
 * (по умолчанию сетка равномерная)
 */
template <typename T, int (*idxGetter)(const T) = nullptr,
          bool _irregular_grid = false>
class Interpolator
{

public:
  Interpolator()
      : _size(0), _left_bound(0), _right_bound(0), _step(0), _tab(nullptr),
        _nodes(nullptr), _func(nullptr)
  {
  }

  ~Interpolator()
  {
    if (_tab)
    {
      delete[] _tab;
    }
    if (_nodes)
    {
      delete[] _nodes;
    }
  }

  const T *GetTab() const { return _tab; }
  unsigned GetSize() const { return _size; }
  T GetStep() const { return _step; }

  /**
   * @brief Init
   * @param a - левая граница отрезка
   * @param b - правая граница отрезка
   * @param N - число точек сетки
   * @param function - интерполируемая функция
   * @param только для регулярных сеток
   */
  void Init(const T a, const T b, const int N, std::function<T(T)> function)
  {
    static_assert(_irregular_grid == false);

    _size = N;
    _left_bound = a;
    _right_bound = b;
    _step = (b - a) / (N - 1);
    if (_tab)
    {
      delete[] _tab;
    }
    _tab = new T[N];
    _func = function;

    for (int i = 0; i < N; i++)
    {
      _tab[i] = _func(a + i * _step);
    }

    // здесь всегда равномерная сетка
    if (_nodes)
    {
      delete[] _nodes;
    }
  }

  void Init(const int N, const T *x, const T *y)
  {
    _func = nullptr;
    _size = N;
    _left_bound = x[0];
    _right_bound = x[N - 1];
    _step = (_right_bound - _left_bound) / (N - 1);
    if (_tab)
    {
      delete[] _tab;
    }
    _tab = new T[N];

    for (int i = 0; i < N; i++)
    {
      _tab[i] = y[i];
    }

    if (_irregular_grid)
    {
      if (_nodes)
      {
        delete[] _nodes;
      }
      _nodes = new T[N];

      for (int i = 0; i < N; i++)
      {
        _nodes[i] = x[i];
      }
    }
  }

  /**
   * @brief Интерполяция к ближайшей <<снизу>> точке
   */
  T ClosePoint(T x) const
  {
    int idx = __get_idx(x);

    if (idx >= _size - 1)
    {
      return _tab[_size - 1];
    }

    if (idx <= 0)
    {
      return _tab[0];
    }

    return _tab[idx];
  }

  /**
   * @brief Интерполяция по средней точки
   */
  T AvgPoint(T x) const
  {
    int idx = __get_idx(x);

    if (idx >= _size - 1)
    {
      return _tab[_size - 1];
    }

    if (idx <= 0)
    {
      return _tab[0];
    }

    return 0.5 * (_tab[idx] + _tab[idx + 1]);
  }

  /**
   * @brief Линейная интерполяция
   */
  T Linear(T x) const
  {
    int idx = __get_idx(x);

    T dx = _step;
    if (_irregular_grid)
    {
      if (idx >= _size - 1)
      {
        dx = _nodes[_size - 1] - _nodes[_size - 2];
      }
      dx = _nodes[idx + 1] - _nodes[idx];
    }

    if (idx >= _size - 1)
    {
      idx = _size - 1;
      T k = (_tab[idx] - _tab[idx - 1]) / dx;
      T m = _tab[idx] - k * (_left_bound + idx * dx);
      return k * x + m;
    }

    if (idx < 0)
    {
      idx = 1;
      T k = (_tab[idx] - _tab[idx - 1]) / dx;
      T m = _tab[idx] - k * (_left_bound + idx * dx);
      return k * x + m;
    }

    T k = (_tab[idx + 1] - _tab[idx]) / dx;
    T m = _tab[idx] - k * (_left_bound + idx * dx);
    return k * x + m;
  }

  enum e_norm_type
  {
    e_norm_max, // абсолютная норма
    e_norm_eps,
    e_norm_l2 // интегральная норма
  };

  enum e_interpolation_type
  {
    e_inter_close,
    e_inter_avg,
    e_inter_linear
  };

  /**
   * @brief Проверка качества интерполяции
   * @param dx - шаг по отрезку
   * @param type_int - тип интерполяции
   * @param norm_type - тип нормы
   * @param ref_function - интерполируемая функция
   * @return норма ошибки
   */
  T CheckQuality(T dx, e_interpolation_type type_int, e_norm_type norm_type,
                 std::function<T(T)> ref_function = nullptr) const
  {
    if (_func)
    {
      ref_function = _func;
    }

    if (!ref_function)
      return -1;

    T norm = 0;

    for (T x = _left_bound; x <= _right_bound; x += dx)
    {
      T df = 0;
      if (type_int == e_inter_linear)
      {
        df = fabs(Linear(x) - ref_function(x));
      }
      else if (type_int == e_inter_avg)
      {
        df = fabs(AvgPoint(x) - ref_function(x));
      }
      else
      {
        df = fabs(ClosePoint(x) - ref_function(x));
      }

      if (norm_type == e_norm_max)
      {
        if (df > norm)
        {
          norm = df;
        }
      }
      else if (norm_type == e_norm_eps)
      {
        norm = df / fabs(_func(x));
      }
      else
      {
        norm += df * df;
      }
    }

    if (norm_type == e_norm_l2)
    {
      int N = (_right_bound - _left_bound) / dx;
      norm = sqrt(norm) / N;
    }
    return norm;
  }

private:
  int __get_idx(const T &x) const
  {
    if (idxGetter != nullptr)
    {
      return idxGetter(x);
    }

    if (_irregular_grid)
    {
      return std::lower_bound(_nodes, _nodes + _size, x) - _nodes - 1;
    }
    return floor((x - _left_bound) / _step);
  }

private:
  int _size;      ///< число узлов сетки
  T _left_bound;  ///< левая граница отрезка
  T _right_bound; ///< правая граница отрезка
  T _step;        ///< шаг сетки
  T *_tab;        ///< табуляция функция
  T *_nodes; ///< коорднаты узлов (формируется только для неравномерной сетке)
  std::function<T(T)> _func; ///< интерполируемая функция
};

template <typename T>
class Interpolator2D
{
public:
  Interpolator2D()
      : _sizeX(0), _sizeY(0), _left_boundX(0), _right_boundX(0),
        _left_boundY(0), _right_boundY(0), _stepX(0), _stepY(0), _tab(nullptr),
        _func(nullptr)
  {
  }

  ~Interpolator2D()
  {
    if (_tab)
    {
      delete[] _tab;
    }
  }

  const T *GetTab() const { return _tab; }
  int GetSize() const { return _sizeX * _sizeY; }
  int GetSizeX() const { return _sizeX; }
  int GetSizeY() const { return _sizeY; }
  T GetStepX() const { return _stepX; }
  T GetStepY() const { return _stepY; }

  /**
   * @brief Init
   * @param a - левая граница отрезка
   * @param b - правая граница отрезка
   * @param N - число точек сетки
   * @param function - интерполируемая функция
   */
  void Init(const T ax, const T bx, const T ay, const T by, const unsigned Nx,
            const unsigned Ny, std::function<T(T, T)> function)
  {
    _sizeX = Nx;
    _sizeY = Ny;

    _left_boundX = ax;
    _right_boundX = bx;
    _stepX = (bx - ax) / (Nx - 1);

    _left_boundY = ax;
    _right_boundY = bx;
    _stepY = (bx - ax) / (Nx - 1);

    if (_tab)
    {
      delete[] _tab;
    }
    _tab = new T[Nx * Ny];
    _func = function;

    for (unsigned i = 0; i < Nx; i++)
    {
      for (unsigned j = 0; j < Ny; j++)
      {
        _tab[i * Ny + j] = _func(ax + i * _stepX, ay + j * _stepY);
      }
    }
  }

  /**
   * @brief Интерполяция к ближайшей снизу точке
   */
  T ClosePoint(T x, T y) const
  {
    int idx = floor((x - _left_boundX) / _stepX);
    int idy = floor((y - _left_boundY) / _stepY);

    idx = std::clamp(idx, 0, (int)_sizeX - 1);
    idy = std::clamp(idy, 0, (int)_sizeY - 1);

    return _tab[idx * _sizeY + idy];
  }

  /**
   * @brief Интерполяция по средней точки
   */
  T AvgPoint(T x, T y) const
  {
    int idx = floor((x - _left_boundX) / _stepX);
    int idy = floor((y - _left_boundY) / _stepY);

    idx = std::clamp(idx, 0, (int)_sizeX - 1);
    idy = std::clamp(idy, 0, (int)_sizeY - 1);

    T z0 = _tab[idx * _sizeY + idy];
    T z1 = z0;
    if (idx < _sizeX - 1)
    {
      z1 = _tab[(idx + 1) * _sizeY + idy];
    }

    T z2 = z0;
    if (idy < _sizeY - 1)
    {
      z2 = _tab[idx * _sizeY + idy + 1];
    }

    idx = std::min(idx, (int)_sizeX - 2);
    idy = std::min(idy, (int)_sizeY - 2);
    T z3 = _tab[(idx + 1) * _sizeY + idy + 1];
    return 0.25 * (z0 + z1 + z2 + z3);
  }

  /**
   * @brief Линейная интерполяция
   */
  T Linear(const T x, const T y) const
  {
    int idx = floor((x - _left_boundX) / _stepX);
    int idy = floor((y - _left_boundY) / _stepY);

    idx = std::clamp(idx, 0, (int)_sizeX - 1);
    idy = std::clamp(idy, 0, (int)_sizeY - 1);

    T x0 = _left_boundX + idx * _stepX;
    T y0 = _left_boundY + idy * _stepY;

    T z0 = _tab[idx * _sizeY + idy];

    T z1 = z0;
    if (idx < _sizeX - 1)
    {
      z1 = _tab[(idx + 1) * _sizeY + idy];
    }

    T z2 = z0;
    if (idy < _sizeY - 1)
    {
      z2 = _tab[idx * _sizeY + idy + 1];
    }

    T a = (z1 - z0) / _stepX;
    T b = (z2 - z0) / _stepY;
    T dx = x0 / _stepX;
    T dy = y0 / _stepY;
    T c = z0 * (1.0 + dx + dy) - z1 * dx - z2 * dy;

    return a * x + b * y + c;
  }

  /**
   * @brief Параболическая интерполяция
   *  z(x,y) = ax^2 +by^2 + cxy + d
   * @note  без линейной части!!!! [коэффициенты ... ex+hy - опущены]
   */
  T Parabolic(const T x, const T y) const
  {
    int idx = floor((x - _left_boundX) / _stepX);
    int idy = floor((y - _left_boundY) / _stepY);

    idx = std::clamp(idx, 0, (int)_sizeX - 1);
    idy = std::clamp(idy, 0, (int)_sizeY - 1);

    T x0 = _left_boundX + idx * _stepX;
    T y0 = _left_boundY + idy * _stepY;

    T z0 = _tab[idx * _sizeY + idy];

    T z1 = z0;
    if (idx < _sizeX - 1)
    {
      z1 = _tab[(idx + 1) * _sizeY + idy];
    }

    T z2 = z0;
    if (idy < _sizeY - 1)
    {
      z2 = _tab[idx * _sizeY + idy + 1];
    }

    idx = std::min(idx, (int)_sizeX - 2);
    idy = std::min(idy, (int)_sizeY - 2);
    T z3 = _tab[(idx + 1) * _sizeY + idy + 1];

    T dx = _stepX;
    T dy = _stepY;
    T dxy = dx * dy;
    T x1 = x0 + dx;
    T y1 = y0 + dy;
    T y1_2 = y1 * y1;
    T y0_2 = y0 * y0;

    T a = (-y1 * z0 + y1 * z1 + y0 * z2 - y0 * z3) / (x0 + x1);
    T b = (-x1 * z0 + x0 * z1 + x1 * z2 - x0 * z3) / (y0 + y1);
    T c = (z0 - z1 - z2 + z3);
    T d = (x1 * x1 * (y1_2 * z0 - y0_2 * z2) +
           x0 * x0 * (-y1_2 * z1 + y0_2 * z3) - c * x0 * x1 * y0 * y1) /
          ((x0 + x1) * (y0 + y1));

    return (a * x * x + b * y * y + c * x * y + d) / dxy;
  }

  enum e_norm_type
  {
    e_norm_max, // абсолютная норма
    e_norm_eps, // абсолютная норма
    e_norm_l2   // интегральная норма
  };

  enum e_interpolation_type
  {
    e_inter_close,
    e_inter_avg,
    e_inter_linear,
    e_inter_parabolic
  };

  /**
   * @brief Проверка качества интерполяции
   * @param dx - шаг по отрезку
   * @param type_int - тип интерполяции
   * @param norm_type - тип нормы
   * @return норма ошибки
   */
  T CheckQuality(T dx, T dy, e_interpolation_type type_int,
                 e_norm_type norm_type) const
  {
    T norm = 0;

    for (T x = _left_boundX; x <= _right_boundX; x += dx)
    {
      for (T y = _left_boundY; y <= _right_boundY; y += dy)
      {
        T df = 0;
        if (type_int == e_inter_parabolic)
        {
          df = fabs(_func(x, y) - Parabolic(x, y));
        }
        else if (type_int == e_inter_linear)
        {
          df = fabs(_func(x, y) - Linear(x, y));
        }
        else if (type_int == e_inter_avg)
        {
          df = fabs(AvgPoint(x, y) - _func(x, y));
        }
        else
        {
          df = fabs(ClosePoint(x, y) - _func(x, y));
        }

        if (norm_type == e_norm_max)
        {
          if (df > norm)
          {
            norm = df;
          }
        }
        else if (norm_type == e_norm_eps)
        {
          norm = df / fabs(_func(x, y));
        }
        else
        {
          norm += df * df;
        }
      }
    }

    if (norm_type == e_norm_l2)
    {
      int Nx = (_right_boundX - _left_boundX) / dx;
      int Ny = (_right_boundY - _left_boundY) / dy;
      norm = sqrt(norm) / (Nx * Ny);
    }
    return norm;
  }

private:
  unsigned _sizeX; ///< число узлов сетки
  unsigned _sizeY; ///< число узлов сетки
  T _left_boundX;  ///< левая граница отрезка
  T _right_boundX; ///< правая граница отрезка
  T _left_boundY;  ///< левая граница отрезка
  T _right_boundY; ///< правая граница отрезка
  T _stepX;        ///< шаг сетки
  T _stepY;        ///< шаг сетки

  T *_tab;                      ///< табуляция функция
  std::function<T(T, T)> _func; ///< интерполируемая функция
};

#endif //! INTERPOLATOR_H