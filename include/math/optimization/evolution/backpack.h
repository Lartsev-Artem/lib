#ifndef BACKPACK_H
#define BACKPACK_H

#include "entity.h"

typedef enum InitBackPackMode
{
	eBackPack_Const,
	eBackPack_Shuffle, //перемешаный const ответ тот же! 
	eBackPack_Random
}init_backpack_mode_t;

class BackPackClass :public Entity
{
private:
	struct Item
	{
		Type weight;
		Type value;
	};
	Type _max_weight;	
	std::vector<Item> _items; //набор предметов

	Type right_ans; // точный ответ

public:
	BackPackClass(int num_item = 5, Type max_weight = 5, init_backpack_mode_t init_mod = eBackPack_Const);

	
	Type FitnessFunction(const gen_chain& genom) override;

	void born(gen_chain& gen)override;

	std::pair<Type, int> cond(const std::vector<Type>& fit, Type base_value)override;

	void print();

	void  ExactAnswer();

	void mutate_bitflip(gen_chain& gen)override;

	void cross_wheel(gen_chain& gen1, const gen_chain& gen2)override;
	void print_answer(gen_chain& gen)override;

	bool stop_iter(const Type val)override;
};
#endif //BACKPACK_H