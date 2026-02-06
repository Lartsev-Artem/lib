#include "gauss_int.h"


double TetraIntegrate(const Tetra_t &t, const QuadraturePoints_t &q_points,
                    const std::function<double(const Vector3 &)> &func)
{

  double integral = 0.0;
  for (const auto &qp : q_points.qp)
  {
    // Преобразование координат в физический тетраэдр
    Vector3 phys_point = t.p[0] + qp.coords[0] * (t.p[1] - t.p[0]) +
                         qp.coords[1] * (t.p[2] - t.p[0]) +
                         qp.coords[2] * (t.p[3] - t.p[0]);

    integral += qp.weight * func(phys_point);
  }

  // Вычисление якобиана (объема тетраэдра)
  double detJ = t.Volume();
  return detJ * integral;
}

Vector3 TetraIntegrate(const Tetra_t &t, const QuadraturePoints_t &q_points,
                       const std::function<Vector3(const Vector3 &)> &func)
{

  Vector3 integral = {0,0,0};
  for (const auto &qp : q_points.qp)
  {
    // Преобразование координат в физический тетраэдр
    Vector3 phys_point = t.p[0] + qp.coords[0] * (t.p[1] - t.p[0]) +
                         qp.coords[1] * (t.p[2] - t.p[0]) +
                         qp.coords[2] * (t.p[3] - t.p[0]);

    integral += qp.weight * func(phys_point);
  }

  // Вычисление якобиана (объема тетраэдра)
  double detJ = t.Volume();
  return detJ * integral;
}
