/**
 * @file "quadrature.h"
 * @author Artem
 * @brief Квадратурные точки для интегрирования в тетраэдрах
 * @version 0.1
 * @date 2025-11-14
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef QUADRATURE_H
#define QUADRATURE_H

#include <vector>
#include "vector3.h"
#include <math.h>


struct Tetra_t
{
  Vector3 p[4];
  Tetra_t() = default;
  double Volume() const
  {
    Vector3 a = p[1] - p[0];
    Vector3 b = p[2] - p[0];
    Vector3 c = p[3] - p[0];
    return fabs((a.dot(b.cross(c)))) / 6.0;
  }

  Vector3 box_min() const
  {
    Vector3 min = p[0];
    for (size_t i = 1; i < 4; i++)
    {
      min[0] = std::min(min[0], p[i][0]);
      min[1] = std::min(min[1], p[i][1]);
      min[2] = std::min(min[2], p[i][2]);
    }
    return min;
  }
  Vector3 box_max() const
  {
    Vector3 max = p[0];
    for (size_t i = 1; i < 4; i++)
    {
      max[0] = std::max(max[0], p[i][0]);
      max[1] = std::max(max[1], p[i][1]);
      max[2] = std::max(max[2], p[i][2]);
    }
    return max;
  }
}; 

/**
 * @brief Коллекция квадратурных точек и весов.
 */
struct QuadraturePoints_t
{
  struct Point
  {
    Vector3 coords; ///< Координаты в эталонном тетраэдре.
    double weight; ///< Вес функции в точке.
  };
  std::vector<Point> qp; ///< Контейнер предвычисленных точек.

public:
  /**
   * @brief Создает пустой набор квадратурных точек.
   */
  QuadraturePoints_t() = default;
  /**
   * @brief Инициализирует квадратуру Ньютона с 10 точками.
   */
  void InitNewton10()
  {
    qp.resize(10);

    qp[0].coords = {0.0000000000000000, 0.0000000000000000, 0.0000000000000000};
    qp[0].weight = -0.0500000000000000;

    qp[1].coords = {1.0000000000000000, 0.0000000000000000, 0.0000000000000000};
    qp[1].weight = -0.0500000000000000;

    qp[2].coords = {0.0000000000000000, 1.0000000000000000, 0.0000000000000000};
    qp[2].weight = -0.0500000000000000;

    qp[3].coords = {0.0000000000000000, 0.0000000000000000, 1.0000000000000000};
    qp[3].weight = -0.0500000000000000;

    qp[4].coords = {0.5000000000000000, 0.0000000000000000, 0.0000000000000000};
    qp[4].weight = 0.2000000000000000;

    qp[5].coords = {0.0000000000000000, 0.5000000000000000, 0.0000000000000000};
    qp[5].weight = 0.2000000000000000;

    qp[6].coords = {0.0000000000000000, 0.0000000000000000, 0.5000000000000000};
    qp[6].weight = 0.2000000000000000;

    qp[7].coords = {0.0000000000000000, 0.5000000000000000, 0.5000000000000000};
    qp[7].weight = 0.2000000000000000;

    qp[8].coords = {0.5000000000000000, 0.0000000000000000, 0.5000000000000000};
    qp[8].weight = 0.2000000000000000;

    qp[9].coords = {0.5000000000000000, 0.5000000000000000, 0.0000000000000000};
    qp[9].weight = 0.2000000000000000;
  }
};

#endif // QUADRATURE_H
