#include "equations.h"

#include <numeric>


EquationClass::EquationClass(const int n, const int* init)
	{
		_unique_id = eEntityId_equation;
		_size = n - 1;
		coefs.resize(n);
		for (int i = 0; i < n; i++)
		{
			coefs[i] = init[i];
		}
	}

Type EquationClass::FitnessFunction(const gen_chain& gen)
{	
	int sum = std::inner_product(gen.cbegin(), gen.cend(), coefs.begin(), 0);	
	return abs(sum - coefs.back());
}

void EquationClass::born(gen_chain& gen)
{
	gen.resize(_size);
	born_gen(gen, 0, 50);	
}


std::pair<Type, int> EquationClass::cond(const std::vector<Type>& fit, Type base_value)
{
	int id = 0;
	uint32_t min = 0xFFFFFFFF;
	const int size = fit.size();
	for (int i = 0; i < size; i++)
	{
		if (fit[i] < min)
		{
			id = i;
			min = fit[i];
		}
	}

	if (min < base_value)
	{
		return std::make_pair(min, id);
	}

	return std::make_pair(0, -1);
}

void EquationClass::mutate_bitflip(gen_chain& gen)
{
	const int idx = rand<int>(0, _size);
	gen[idx] = rand<BitsType>(1, 100); //может дважды мутировать и не изменить состояние!
}

void EquationClass::cross_wheel(gen_chain& gen1, const gen_chain& gen2)
{
	const uint32_t change_bit = rand<uint32_t>(1, _size); // номер бита на замену
	for (size_t i = change_bit; i < _size; i++)
	{
		gen1[i] = gen2[i - change_bit];
	}
}

bool EquationClass::stop_iter(const Type val)
{
	return ((int)val != 0);
}

#include <iostream>

void EquationClass::print()
{
	printf("equation:\n");

	std::cout << coefs[0] << "*x" << 0;
	for (int i = 1; i < coefs.size() - 1; i++)
	{
		std::cout << " + " << coefs[i] << "*x" << i;
	}
	std::cout << " = " << coefs.back() << '\n';
}

void EquationClass::print_answer(gen_chain& gen)
{
	if (gen.size() == 0)
	{
		printf("no solve\n");
		return;
	}

	std::cout << "roots: ";
	for (auto& el : gen)
	{
		std::cout << el << ' ';
	}
	std::cout << '\n';
}