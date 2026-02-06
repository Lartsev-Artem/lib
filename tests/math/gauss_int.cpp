#include "gauss_int.h"
#include <assert.h>
#include <stdio.h>


double func(const Vector3 &x) { return x.dot(x); }
double func2(const Vector3 &x) { return x[0] + x[1] + x[2]; }

int main()
{
  QuadraturePoints_t qp;
  qp.InitNewton10();

  Tetra_t t;
  t.p[0] = Vector3(-0.5, 0., 0.5);
  t.p[1] = Vector3(0.5, 0, 0.5);
  t.p[2] = Vector3(-0.5, 1, 0.5);
  t.p[3] = Vector3(-0.5, 0, 1.5);

  Tetra_t t2;
  t2.p[0] = Vector3(0, 0, 0);
  t2.p[1] = Vector3(1, 0, 0);
  t2.p[2] = Vector3(0, 1, 0);
  t2.p[3] = Vector3(0, 0, 1);

  double s0 = TetraIntegrate(t, qp, func);
  double s1 = TetraIntegrate(t, qp, func2);
  double s2 = TetraIntegrate(t2, qp, func);
  double s3 = TetraIntegrate(t2, qp, func2);

  printf("int= %.16lf\n", s0);
  printf("int= %.16lf\n", s1);
  printf("int= %.16lf\n", s2);
  printf("int= %.16lf\n", s3);

  assert(fabs(s0 - 0.1333333333333333) < 1e-14);
  assert(fabs(s1 - 0.125) < 1e-14);
  assert(fabs(s2 - 0.05) < 1e-14);
  assert(fabs(s3 - 0.125) < 1e-14);


  return 0;
}

/*
 T = Tetrahedron[{{-0.5, 0., 0.5},{0.5, 0, 0.5},{-0.5, 1, 0.5},{-0.5, 0, 1.5}}];
 F[x_, y_, z_] := x*x + y*y + z*z;
 Integrate[F[x, y, z], {x, y, z} \[Element] T]
 */
