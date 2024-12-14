#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "entity.h"

class EquationClass :public Entity
{
public:
	std::vector<int> coefs; // коэф+правая часть	

	EquationClass(const int n, const int* init);

	Type FitnessFunction(const gen_chain& gen) override;


	void born(gen_chain& gen)override;

	void print();

	void print_answer(gen_chain& gen)override;

	std::pair<Type, int> cond(const std::vector<Type>& fit, Type base_value)override;

	void mutate_bitflip(gen_chain& gen)override;

	void cross_wheel(gen_chain& gen1, const gen_chain& gen2)override;

	bool stop_iter(const Type val)override;

};

#endif //EQUATIONS_H