#include "annealing.h"
#include <math.h>

Annealing::~Annealing()
{
	if (_new_solution)
	{
		delete _new_solution;
	}
}

Annealing::Annealing(const Config& cfg) : 
														_solution(nullptr),
														_new_solution(nullptr)
{
	_cfg = cfg;		
	_temperature = cfg.START_TEMPERATURE;
}

	double Annealing::reduce_temperature(int step) const {
	return _cfg.ALPHA * _temperature / step;
}
	
double Annealing::get_propabilty() const
{
	double de = _new_solution->get_energy() - _solution->get_energy();
	return exp(-de / _temperature);
}

Entity* Annealing::make_desision()
{
	// если число конфликтов в новой системе меньше, то берем новое состояние
	if (_new_solution->get_energy() <= _solution->get_energy())
	{
		return _new_solution;
	}
	// если число конфликтов в новой системе больше, НО берем новое состояние        
	else if (get_propabilty() > (double)(rand() % 1000) / 1000)
	{
		//если вероятность больше, чем случайное число из интервала от 0 до 1 (здесь с точностью до 3 знаков), то принимается «худшее» решение
		return _new_solution;
	}
	else
	{
		//оставляем сторое состояниие
	}
	return _solution;
}
	
double Annealing::start_anneling()
{
	int step_number = 0;
	
	_solution->init();
	_solution->copy(_new_solution);	

	do
	{
		for (int step = 0; step < _cfg.COUNT_ITERATIONS; step++) //шаг цикла
		{
			_new_solution->recombination();				
			_new_solution->calculate_energy(); // вычисляем ее энергию

			if (_new_solution->get_energy() < _cfg.ENERGY_THRESHOLD) // если решение найдено - прекращение поиска
			{
				_new_solution->copy(_solution); //копируем выбранное новое приближение в следующее _solution = _new_solution;								
				return _solution->get_energy();
			}
			else
			{
				_solution = make_desision();
			}

			/// \warning эта самая тяжелая операция. Для конкретных задач, оптимально запоминать рекомбинацию

			_new_solution->copy(_solution); //копируем выбранное новое приближение в следующее
		}
		reduce_temperature(++step_number); // понижение температуры(в зависимости от шага)

	} while (_temperature <= _cfg.FINISH_TEMPERATURE); // если система "остыла", то выход
		
	return _solution->get_energy();   // возвращаем полученную энергию, чтобы потом проверить достигли ли мы результата
}

bool Annealing::Run(Entity* entity)
{
	_solution = entity;
	
	int try_counter = 0;
	double energy = 0;
	do
	{
		energy = start_anneling();				
		try_counter += (energy > _cfg.ENERGY_THRESHOLD);
	} while (try_counter < _cfg.MAX_NUMBER_OF_RESTART && energy > _cfg.ENERGY_THRESHOLD);   // если система "остыла", но результат не был найден, то повторить

	_solution->copy(entity);

	return (try_counter < (_cfg.MAX_NUMBER_OF_RESTART));
}
