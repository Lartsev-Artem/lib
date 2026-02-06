
#include "integrator.h"

#include <stdio.h>
#include <math.h>
#include <vector>
using Type = double;

Type poly(Type x, int n) { return pow(x, n); }
Type IntPoly(int n, Type a, Type b)
{
  return (pow(b, n + 1) - pow(a, n + 1)) / (n + 1);
}

Type f0(Type x) { return x * x * sin(x); }
Type Intf0(Type a, Type b)
{
  return (-2 + a * a) * cos(a) - (-2 + b * b) * cos(b) +
         2 * (b * sin(b) - a * sin(a));
}

Type f1(Type x) { return exp(x) * sin(x); }
Type Intf1(Type a, Type b)
{
  return 0.5 * (pow(exp(1), a) * (cos(a) - sin(a)) +
                pow(exp(1), b) * (-cos(b) + sin(b)));
}

int main(int argc, char **argv)
{
  Type a = -2.5;
  Type b = 5.4;

  std::vector<int> orders = {1, 2, 4, 5, 8, 16, 32};
  for (auto N : orders)
  {
    Integrator_t<Type> integrator(N);

    printf("Order: %d=\n", N);
    {
      Type y =
          integrator.Integrate(a, b, [&](Type x) { return poly(x, N - 1); });
      Type y0 = IntPoly(N - 1, a, b);
      Type eps = fabs(y - y0) / y0;
      printf("P: %lf %lf eps=%.16lf\n", y, y0, eps);
    }
    {
      Type y = integrator.Integrate(a, b, f0);
      Type y0 = Intf0(a, b);
      Type eps = fabs(y - y0) / y0;
      printf("f0: %lf %lf eps=%.16lf\n", y, y0, eps);
    }
    {
      Type y = integrator.Integrate(a, b, f1);
      Type y0 = Intf1(a, b);
      Type eps = fabs(y - y0) / y0;
      printf("f0: %lf %lf eps=%.16lf\n", y, y0, eps);
    }
    printf("\n");
  }

  return 0;
}
