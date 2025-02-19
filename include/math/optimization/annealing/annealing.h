#ifndef ANNEALING_H
#define ANNEALING_H

#include <vector>

class Entity
{
public:	
	virtual double calculate_energy() = 0;
	virtual void init() = 0;
	virtual void copy(Entity*& solution) const = 0;
	virtual void recombination() = 0;

	inline double get_energy() const { return _energy; }
protected:
	double _energy;           //энергия полученного решения.  
};

// метод отжига с фиксацией ферзя по 1 строке. 
class Annealing
{
public:
	struct Config
	{
		int COUNT_ITERATIONS;          ///< количество итераций при понижении температуры	
		int MAX_NUMBER_OF_RESTART;                  ///< постоянная альфа используется при вычислении понижения температуры
		double START_TEMPERATURE;      ///< инициализация температуры нагрева
		double FINISH_TEMPERATURE;     ///< конечная температура охлаждения	 
		double ENERGY_THRESHOLD;       ///< доверительный порог энергии (ниже которого считаем, что решение получено)
		double ALPHA;                  ///< постоянная альфа используется при вычислении понижения температуры		

		Config(int max_iter = 1000, int restart_cnt = 5, double start_t = 10, double finish_t = 0.00001, double energy_thr = 0.01, double alpha = 0.98) :
			COUNT_ITERATIONS(max_iter),
			MAX_NUMBER_OF_RESTART(restart_cnt),
			START_TEMPERATURE(start_t),
			FINISH_TEMPERATURE(finish_t),
			ENERGY_THRESHOLD(energy_thr),
			ALPHA(alpha)
		{}
	};

public:
	Annealing(const Config& cfg = Config());
	~Annealing();
	bool Run(Entity* entity);

private:		
	double reduce_temperature(int step) const; ///< Понижает температуру
	double get_propabilty() const;	///< вероятность принять решение заведомо неудовлетворяющее условиям задачи
	Entity* make_desision();		
	double start_anneling();	 ///<	Основная работа алгоритма

private:
	double _temperature;
	Config _cfg;
	Entity* _solution;
	Entity* _new_solution;
};




#endif //ANNEALING_H
