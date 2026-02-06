#include "interpolator.h"
#include <iostream>
#include <math.h>
#include "timer.h"


static const int time_repeat = 10000000;

static double f0(double x) { return x; };
static double f1(double x) { return x * x; };
static double f2(double x)
{
  return pow(exp(cos(sqrt(x) * sin(x) * log(x * x * pow(x, 7.3)))), 5.433);
}

static double f00(double x, double y) { return x + y; };
static double f11(double x, double y) { return x * x + y * y; };
static double f22(double x, double y) { return f2(x) * f2(y); };

int test1D_reg()
{
  std::cout << " ---- START test1D_reg() ---- \n";

  typedef Interpolator<double> Int;
  Int tab1;

  const double a = 0;
  const double b = 1;
  const int N = 11;
  const double dx_check = 0.01;

  const volatile double x = 1.0 / sqrt(2);

  std::cout << " ======= f0 run: ========\n";
  {
    tab1.Init(a, b, N, f0);
    std::cout << "cls_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "linear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.Linear(x); }, time_repeat)
              << "\n";

    printf("Quality0: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_close, Int::e_norm_max));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_avg, Int::e_norm_max));
    printf("Quality2: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_linear, Int::e_norm_max));
  }

  std::cout << "\n ======= f1 run: ========\n";
  {
    tab1.Init(a, b, N, f1);
    std::cout << "cls_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "linear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.Linear(x); }, time_repeat)
              << "\n";

    printf("Quality0: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_close, Int::e_norm_max));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_avg, Int::e_norm_max));
    printf("Quality2: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_linear, Int::e_norm_max));
  }

  std::cout << "\n ======= f2 run: ========\n";
  {
    tab1.Init(a, b, N, f2);
    std::cout << "cls_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "linear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.Linear(x); }, time_repeat)
              << "\n";

    printf("Quality0: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_close, Int::e_norm_max));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_avg, Int::e_norm_max));
    printf("Quality2: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_linear, Int::e_norm_max));
  }
  return 0;
}

int test1D_irreg()
{
  std::cout << " ---- START test1D_irreg() ---- \n";

  typedef Interpolator<double, nullptr, true> Int;
  Int tab1;

  const double a = 0;
  const double b = 1;
  const int N = 11;
  const double dx_check = 0.01;
  double X[N] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
  double Y[N];
  const volatile double x = 1.0 / sqrt(2);

  std::cout << " ======= f0 run: ========\n";
  {
    for (size_t i = 0; i < N; i++)
    {
      Y[i] = f0(X[i]);
    }
    tab1.Init(N, X, Y);
    std::cout << "cls_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "linear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.Linear(x); }, time_repeat)
              << "\n";

    printf(
        "Quality0: %0.16lf\n",
        tab1.CheckQuality(dx_check, Int::e_inter_close, Int::e_norm_max, f0));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_avg, Int::e_norm_max, f0));
    printf(
        "Quality2: %0.16lf\n",
        tab1.CheckQuality(dx_check, Int::e_inter_linear, Int::e_norm_max, f0));
  }

  std::cout << "\n ======= f1 run: ========\n";
  {
    for (size_t i = 0; i < N; i++)
    {
      Y[i] = f1(X[i]);
    }
    tab1.Init(N, X, Y);
    std::cout << "cls_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "linear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.Linear(x); }, time_repeat)
              << "\n";

    printf(
        "Quality0: %0.16lf\n",
        tab1.CheckQuality(dx_check, Int::e_inter_close, Int::e_norm_max, f1));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_avg, Int::e_norm_max, f1));
    printf(
        "Quality2: %0.16lf\n",
        tab1.CheckQuality(dx_check, Int::e_inter_linear, Int::e_norm_max, f1));
  }

  std::cout << "\n ======= f2 run: ========\n";
  {
    for (size_t i = 0; i < N; i++)
    {
      Y[i] = f2(X[i]);
    }
    tab1.Init(N, X, Y);
    std::cout << "cls_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x); },
                     time_repeat)
              << "\n";
    std::cout << "linear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.Linear(x); }, time_repeat)
              << "\n";

    printf(
        "Quality0: %0.16lf\n",
        tab1.CheckQuality(dx_check, Int::e_inter_close, Int::e_norm_max, f2));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, Int::e_inter_avg, Int::e_norm_max, f2));
    printf(
        "Quality2: %0.16lf\n",
        tab1.CheckQuality(dx_check, Int::e_inter_linear, Int::e_norm_max, f2));
  }
  return 0;
}

int test2D_reg()
{
  std::cout << " ---- START test2D_reg() ---- \n";

  typedef Interpolator2D<double> Int;
  Int tab1;

  const double a = 0;
  const double b = 1;
  const int N = 11;
  const double dx_check = 0.01;

  const volatile double x = 1.0 / sqrt(2);

  std::cout << " ======= f0 run: ========\n";
  {
    tab1.Init(a, b, a, b, N, N, f00);
    std::cout << "cls_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "bilinear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.BiLinear(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "parabolic_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.Parabolic(x, x); },
                     time_repeat)
              << "\n";

    printf("Quality0: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_close,
                             Int::e_norm_max));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_avg,
                             Int::e_norm_max));
    printf("Quality2: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_bilinear,
                             Int::e_norm_max));
    printf("Quality3: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_parabolic,
                             Int::e_norm_max));
  }

  std::cout << "\n ======= f1 run: ========\n";
  {
    tab1.Init(a, b, a, b, N, N, f11);
    std::cout << "cls_time:\t"
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t"
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "bilinear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.BiLinear(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "parabolic_time:\t"
              << time::measure_time(
                     [&]() { volatile double a = tab1.Parabolic(x, x); },
                     time_repeat)
              << "\n";

    printf("Quality0: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_close,
                             Int::e_norm_max));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_avg,
                             Int::e_norm_max));
    printf("Quality2: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_bilinear,
                             Int::e_norm_max));
    printf("Quality3: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_parabolic,
                             Int::e_norm_max));
  }

  std::cout << "\n ======= f2 run: ========\n";
  {
    tab1.Init(a, b, a, b, N, N, f22);
    std::cout << "cls_time:\t"
              << time::measure_time(
                     [&]() { volatile double a = tab1.ClosePoint(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "avg_time:\t"
              << time::measure_time(
                     [&]() { volatile double a = tab1.AvgPoint(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "bilinear_time:\t" << std::fixed
              << time::measure_time(
                     [&]() { volatile double a = tab1.BiLinear(x, x); },
                     time_repeat)
              << "\n";
    std::cout << "parabolic_time:\t"
              << time::measure_time(
                     [&]() { volatile double a = tab1.Parabolic(x, x); },
                     time_repeat)
              << "\n";

    printf("Quality0: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_close,
                             Int::e_norm_max));
    printf("Quality1: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_avg,
                             Int::e_norm_max));
    printf("Quality2: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_bilinear,
                             Int::e_norm_max));
    printf("Quality3: %0.16lf\n",
           tab1.CheckQuality(dx_check, dx_check, Int::e_inter_parabolic,
                             Int::e_norm_max));
  }
  return 0;
}
int main()
{
  std::cout.precision(16);
  test1D_reg();
  test1D_irreg();
  test2D_reg();

  return 0;
}