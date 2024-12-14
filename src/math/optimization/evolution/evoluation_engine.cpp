#include "evoluation_engine.h"

#include <numeric>

GeneticEngine::GeneticEngine(Entity* entity, int size)
{
	_population_size = size;
	_entity = entity;
	BornPopulation();

	SetEngineCond(_population_size, 100);
	SetMutateCond();
	SetSelectCond();
	SetCrossCond();

	switch (_entity->id())
	{
	case eEntityId_equation:
		_best_ever.fitness = 1;
		break;
	case eEntityId_backpack:
		_best_ever.fitness = -1;
		break;
	default:
		EXIT_ERR("unknow entity\n");
	}


	return;
}

void GeneticEngine::SetEngineCond(int max_population, int _max_iter)
{
	_max_population = max_population;
	_max_generations = _max_iter;
}

void GeneticEngine::SetSelectCond(const selection_type_t selectionType)
{
	_selectionType = selectionType;
}
void GeneticEngine::SetMutateCond(mutation_type_t mutationType, Type mutation_probability, Type invers_probability, int max_mutate_gens)
{
	_mutate_state.mutationType = mutationType;
	_mutate_state.mutation_probability = mutation_probability;
	_mutate_state.max_mutate_gens = max_mutate_gens;
	_mutate_state.invers_probability = invers_probability;
}
void GeneticEngine::SetCrossCond(сrossing_type_t crossingType, Type cross_probability)
{
	_crossingType = crossingType;
	_cross_probability = cross_probability;
}

void GeneticEngine::BornPopulation()
{
	_population.resize(_population_size);
	for (size_t i = 0; i < _population_size; i++) // по попул€ции
	{		
		_entity->born(_population[i]);
	}
}

Type GeneticEngine::fitness_calc(const std::vector<gen_chain>& population)
{
	const int size = population.size();
	_fitness_value.resize(size);
	for (int i = 0; i < size; i++)
	{
		_fitness_value[i] = _entity->FitnessFunction(population[i]);
	}

	std::pair<Type, int> max_and_id = _entity->cond(_fitness_value, _best_ever.fitness);

	if (max_and_id.second >= 0)
	{
		_best_ever.fitness = max_and_id.first;
		_best_ever.gens = population[max_and_id.second];
	}

	return max_and_id.first;
}
void GeneticEngine::wheel_calc()
{
	wheel_array.resize(_fitness_value.size());	//веро€тности со сдвигом

	switch (_entity->id())
	{
	case eEntityId_backpack:
	{
		wheel_array[0] = _fitness_value[0];			//«начение ‘итнесс‘ункции дл€ 1-ого генома 		
		for (int i = 1; i < _fitness_value.size(); i++)
		{
			wheel_array[i] = wheel_array[i - 1] + _fitness_value[i];//«начение ‘итнесс‘ункции дл€ i-ого генома
		}
	}
	break;

	case eEntityId_equation:
	{
		Type multinv = 0;
		for (int i = 1; i < _population_size; i++)
		{
			multinv += 1. / ((Type)_fitness_value[i]);
		}

		wheel_array[0] = ((1. / ((Type)_fitness_value[0]) / multinv));
		for (int i = 1; i < _population_size; i++)
		{
			wheel_array[i] = wheel_array[i - 1] + ((1. / ((Type)_fitness_value[i]) / multinv));
		}
	}
	break;

	default:EXIT_ERR("unknow entity\n");
	}
}

void GeneticEngine::mutation(gen_chain& gen1)
{
	const Type p = _entity->rand<Type>(0, 1);
	if (_mutate_state.mutation_probability < p || _mutate_state.max_mutate_gens == 0)
	{
		return; //мутаци€ не произошла или отключена
	}
	const int size = _entity->size();
	const int n = _entity->rand<int>(1, _mutate_state.max_mutate_gens) % size; // число замен€ющих генов

	switch (_mutate_state.mutationType)
	{
	case eMutation_BitFlip:

		for (int i = 0; i < n; i++)
		{
			_entity->mutate_bitflip(gen1);
		}
		break;
	default:
		EXIT_ERR("err mutation type\n");
	}
}

void GeneticEngine::cross(const gen_chain& gen1, const gen_chain& gen2)
{
	switch (_crossingType)
	{
	case eCross_ONE_POINT_RECOMBINATION: // одноточечный cross
	{
		if (_entity->rand<int>(0, 2))
		{
			gen_chain new1 = gen1;
			_entity->cross_wheel(new1, gen2);
			_new_population.push_back(new1);
		}
		else
		{
			gen_chain new2 = gen2;
			_entity->cross_wheel(new2, gen1);
			_new_population.push_back(new2);
		}
	}
	break;
	default:
		EXIT_ERR("err cross type\n");
	}
}
int GeneticEngine::find_idx(Type control_val)
{	
	int i = 0;
	for (i = 0; i < wheel_array.size() - 1; i++)
	{
		if (control_val < wheel_array[i]) // && control_val < array[i + 1])
		{
			return i;
		}
	}
	return i;
}
void GeneticEngine::get_parents_id(int& id1, int& id2)
{
	Type rand_val1 = _entity->rand<Type>(0, wheel_array.back());
	id1 = find_idx(rand_val1);  //первый наиболее веро€тный родитель
	int it = 0;
	do
	{
		Type rand_val1 = _entity->rand<Type>(0, wheel_array.back());
		id2 = find_idx(rand_val1);  // пытаемс€ расчитать второго по веро€тности (5 попыток)

		if (it++ > 5)
		{
			do  //расчитываем второго на удачу
			{
				id2 = _entity->rand<int>(0, _population_size);
			} while (id1 == id2);
			break;
		}
	} while (id1 == id2);
}

void GeneticEngine::crossover()
{
	wheel_calc();

	int id1, id2;
	for (int i = 0; i < _population_size; i++)
	{
		get_parents_id(id1, id2);

		cross(_population[id2], _population[id2]); //в конец дописываютс€ потомки
	}
}

void GeneticEngine::mutator()
{
	for (size_t i = 0; i < _new_population.size(); i++)
	{
		mutation(_new_population[i]);
	}
}

void GeneticEngine::selection_backpack()
{
	_population.insert(_population.end(), _new_population.begin(), _new_population.end()); //-родители + потомки(уже с мутаци€ми)
	_population_size = _population.size();
	fitness_calc(_population);

	switch (_selectionType)
	{
	case eSelTupe_RouletteWheel:
	{
		wheel_calc();
		Type sum = wheel_array.back();

		const int size = std::min(_max_population, _population_size);
		_new_population.resize(size);
		for (int i = 0; i < size; i++)
		{
			Type rand_val1 = _entity->rand<Type>(0, sum);
			int ind1 = 0;

			if (sum > 1e-10) // если есть живые
			{
				ind1 = find_idx(rand_val1);
			}
			else
			{
				ind1 = size - i - 1; // все потомки и что захватим
			}

			_new_population[i] = (_population[ind1]); // все потомки и что захватим
		}

		break;
	}

	default:
		EXIT_ERR("err selection type\n");
	}


	_population_size = _new_population.size();
	_population.resize(_population_size);
	return;
}

void GeneticEngine::selection_equation()
{
	const Type sum = std::accumulate(_fitness_value.begin(), _fitness_value.end(), 0.0);;

	fitness_calc(_new_population); // дл€ текущего поколени€		
	Type sum1 = std::accumulate(_fitness_value.begin(), _fitness_value.end(), 0.0);;

	int cc = 0;
	while (sum <= sum1)
	{
		mutator();

		fitness_calc(_new_population); // дл€ текущего поколени€
		sum1 = std::accumulate(_fitness_value.begin(), _fitness_value.end(), 0.0);;

		if (cc++ > 30) break;
	}
}


void GeneticEngine::run()
{
	int iter = 0;
	while (iter++ < _max_generations && _entity->stop_iter(_best_ever.fitness))
	{
		_new_population.clear();

		fitness_calc(_population); // дл€ текущего поколени€

		crossover(); //->_new_population 

		switch (_entity->id())
		{
		case eEntityId_backpack:
		{
			mutator(); //	->_new_population 		 

			selection_backpack(); //-> _new_population
		}
		break;
		case eEntityId_equation:
		{
			selection_equation();
		}
		break;
		default:
			EXIT_ERR("unknow entity\n");
		}

		_new_population.swap(_population);
	}

	printf("iters= %d\n", iter);
	return;
}

void GeneticEngine::print()
{
	_entity->print_answer(_best_ever.gens);
	printf("\n\n");
}