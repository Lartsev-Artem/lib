/**
 * @file gauss_int.h
 * @author Artem ()
 * @brief
 * @version 0.1
 * @date 2025-09-13
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef GAUSS_INT_H
#define GAUSS_INT_H


#include "quadrature.h"

#include <functional>


/**
 * @brief Функция интегрирования по ячейке
 *
 * @param[in] t - тетраэдр
 * @param[in] q_points - квадратурные точки
 * @param[in] func - под интегральная функция
 * @return значение интеграла
 */
double TetraIntegrate(const Tetra_t &t, const QuadraturePoints_t &q_points,
                    const std::function<double(const Vector3 &)> &func);

Vector3 TetraIntegrate(const Tetra_t &t, const QuadraturePoints_t &q_points,
                       const std::function<Vector3(const Vector3 &)> &func);

#endif // GAUSS_INT_H