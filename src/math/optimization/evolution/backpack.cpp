#include "backpack.h"

#include <algorithm>
#include <random>

BackPackClass::BackPackClass(int num_item, Type max_weight, init_backpack_mode_t init_mod)
	{
		_unique_id = eEntityId_backpack;
		right_ans = -1;

		_size = num_item;
		_max_weight = max_weight;
		_items.resize(num_item);

		switch (init_mod)
		{
		case eBackPack_Const:
			for (size_t i = 0; i < num_item; i++)
			{
				_items[i].weight = 1 + i;
				_items[i].value = 1 + i * 10;
			}
			break;
		case eBackPack_Shuffle:
			for (size_t i = 0; i < num_item; i++)
			{
				_items[i].weight = 1 + i;
				_items[i].value = 1 + i * 10;
			}
			std::shuffle(_items.begin(), _items.end(), std::mt19937(std::random_device{}()));						
			break;

		case eBackPack_Random:
			for (int i = 0; i < num_item; i++)
			{
				_items[i].value = rand<Type>(0.001, 1e3);
				_items[i].weight = rand<Type>(0.001, max_weight); // вес не превышает max
			}
			break;
		default:
			EXIT_ERR("bad init_backpack_mode_t\n");
		}

	}
	
Type BackPackClass::FitnessFunction(const gen_chain& genom)
{
	Type sum_value = 0;
	Type sum_weight = 0;

	int gen_id = 0;
	for (auto& el : _items)
	{
		if (check_bit(genom[get_frame(gen_id)], get_num_bit(gen_id)))
		{
			sum_value += el.value;
			sum_weight += el.weight;

		}
		gen_id++;

		if (sum_weight > _max_weight)
		{
			sum_value = 0;
			break;
		}
	}

	return sum_value;
}

void BackPackClass::born(gen_chain& gen)
{
	// const int frame_count = get_frame(_size) + 1; 
	const int frame_count = get_frame_count(_size); // DIMA
	const BitsType right_pos = (1 << (_size)) - 1; // ограничиваем все гены == 1
	
	gen.resize(frame_count);
	born_gen(gen, 0, right_pos);	
}

std::pair<Type, int> BackPackClass::cond(const std::vector<Type>& fit, Type base_value)
{
	int id = 0;
	Type max = -1;
	const int size = fit.size();

	for (size_t i = 0; i < size; i++)
	{
		if (fit[i] > max)
		{
			id = i;
			max = fit[i];
		}
	}

	if (max > base_value)
	{
		return std::make_pair(max, id);
	}

	return std::make_pair(0, -1);
}

void BackPackClass::mutate_bitflip(gen_chain& gen)
{
	const int idx = rand<int>(0, _size);
	gen[get_frame(idx)] ^= (1 << (get_num_bit(idx))); //может дважды мутировать и не изменить состояние!
}

void BackPackClass::cross_wheel(gen_chain& gen1, const gen_chain& gen2)
{
	const uint32_t change_bit = rand<uint32_t>(1, _size); // номер бита на замену
	//const uint32_t frame = change_bit / bitsType_size; // где он хранится
	const uint32_t frame = get_frame(change_bit); // где он хранится // DIMA
	const BitsType mask = (1 << change_bit) - 1;

	/// \note: для длинных цепочек обмен будет только по bitsType_size генам!, остальные останутся от главного родителя
	gen1[frame] = (gen1[frame] & (~mask)) | ((gen2[frame] & mask)); //потомок М|П
}

bool BackPackClass::stop_iter(const Type val)
{
	return (fabs(val - right_ans) > 1e-6);
}


void  BackPackClass::ExactAnswer()
{
	if (_size > 15)
	{
		printf("Too mutch to exact solve\n");
		return;
	}
	BitsType best_state = 0;
	Type max = -1;

	gen_chain gens(1);
	const BitsType iters = (1 << _size) - 1;
	for (BitsType state = 0; state <= iters; state++)
	{
		gens[0] = state;
		Type buf = FitnessFunction(gens);
		if (buf > max)
		{
			max = buf;
			best_state = state;
		}
	}

	right_ans = max;
	gen_chain ans = { best_state };
	printf("exact:\n");
	print_answer(ans);
}

#include <iostream>
void BackPackClass::print()
{
	printf("backpack: ");

	printf("\nweight: ");
	for (auto& el : _items)
	{
		printf("%6.02lf, ", el.weight);
	}

	printf("\nvalue:  ");
	for (auto& el : _items)
	{
		printf("%6.02lf, ", el.value);
	}
	printf("\n");
}
void BackPackClass::print_answer(gen_chain& gen)
{
	// for (int i = 0; i < get_frame(_size) + 1; i++)
	for (int i = 0; i < get_frame_count(_size); i++) // DIMA
	{
		BitsType frame = gen[i];

		const int iter = std::min(bitsType_size, _size - i * bitsType_size);
		for (int j = 0; j < iter; j++)
		{
			std::cout << check_bit(frame, j) << ' ';
		}
	}

	std::cout << "\nfit= " << FitnessFunction(gen) << '\n';
}