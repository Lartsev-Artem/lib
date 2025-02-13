#include "interpolator.h"

#include <iostream>

typedef double Type;

Type f(Type x)
{
	//return log10(x);
	constexpr double L = 20;
	constexpr double x0 = 50;
	constexpr double k = 0.075;
	return -(L/(1+exp(-k*(x-x0))) - 0.5*L);
}

int main(int argc, char** argv)
{
	std::cout << "Interpolation test:\n";

	Interpolator<Type> A;

	Type a = 1;
	Type b = (2<<15)-1;
	unsigned N = 64;
	//A.Init(a, b, N, f);

	A.Init(1,100,10, [](double x){return (1<<20) * f(x); });


//	Type a = 20;
//	Type b = 91;
//	unsigned N = 64;
//	A.Init(a,b,N, [](float x){return powf(10,0.05f*x);});

	constexpr double eps = 1;
	std::cout << "NormMax_avgPoint: " << A.CheckQuality(eps,
								Interpolator<Type>::e_inter_avg,
								Interpolator<Type>::e_norm_max) << std::endl;

	std::cout << "NormMax_Linear: " << A.CheckQuality(eps,
								Interpolator<Type>::e_inter_linear,
								Interpolator<Type>::e_norm_max) << std::endl;

	std::cout << "NormL2_avgPoint: " << A.CheckQuality(eps,
								Interpolator<Type>::e_inter_avg,
								Interpolator<Type>::e_norm_l2) << std::endl;

	std::cout << "NormL2_Linear: " << A.CheckQuality(eps,
								Interpolator<Type>::e_inter_linear,
								Interpolator<Type>::e_norm_l2) << std::endl;

	std::cout << "\ndx = " << A.GetStep() <<"\n";
	std::cout << "tab[" << N << "] = {";
	for(unsigned i = 0; i < N-1; i++)
	{
		std::cout << A.GetTab()[i] << ",";
	}
	std::cout << A.GetTab()[N-1] << "}\n";

#if 0
	Type x = a;
	Type dx = 10;

	std :: cout  << "\n\n[";
	while(x <= b)
	{
		std :: cout << x << "," << fabs(A.Linear(x) - f(x)) << ";";
		//std :: cout << x << "," << A.Linear(x)  << ";";
		x += dx;
	}
	std :: cout  << "];\n";
#endif

	return 0;
}
