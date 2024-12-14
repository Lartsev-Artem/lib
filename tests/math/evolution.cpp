#include "evoluation_engine.h"
#include "backpack.h"
#include "equations.h"

int main()
{
	int a = 0;

	do
	{
		const int max_iter = 1000;	 // макс. итераций
		const int size_population = 10;	//размер попул€ции

		const Type p_c = 0.9;
		const Type p_i = 0.01;
		const Type p_m = 0.1;

		const int n = 10;			 //количество предметов
		const Type max_w = 7;		// макс. вес
		BackPackClass backpack(n, max_w, eBackPack_Shuffle);

		backpack.print();
		backpack.ExactAnswer();
		GeneticEngine gen_backpack((Entity*)&backpack, size_population);

		gen_backpack.SetEngineCond(size_population, max_iter);
		gen_backpack.SetMutateCond(eMutation_BitFlip, p_m, p_i, 5);

		gen_backpack.run();
		gen_backpack.print();

		///////////////////////////////////////

		std::vector<int> coef = { 3,2,1,25 };
		EquationClass equ(coef.size(), coef.data());
		equ.print();

		GeneticEngine gen_equation((Entity*)&equ, size_population);

		gen_equation.SetEngineCond(size_population, max_iter);
		gen_equation.SetMutateCond(eMutation_BitFlip, p_m, p_i, 2);

		gen_equation.run();
		gen_equation.print();
	} while (++a < 10);
	return 0;
}
