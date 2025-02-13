#ifndef EVOLUATION_ENGINE
#define EVOLUATION_ENGINE

#include "entity.h"

typedef enum SelectionType
{
	eSelTupe_Tourney,
	eSelTupe_RouletteWheel
} selection_type_t;

typedef enum CrossingType
{
	eCross_ONE_POINT_RECOMBINATION,
	eCross_TWO_POINT_RECOMBINATION,
	eCross_ELEMENTWISE_RECOMBINATION,
	eCross_ONE_ELEMENT_EXCHANGE
}crossing_type_t;

typedef enum MutationType
{
	eMutation_BitFlip
} mutation_type_t;


class GeneticEngine
{
private:

	int _max_generations;		// число итераций
	int _max_population;		//избежание перенаселения (максмальный размер популяции)

	Entity* _entity; // \todo: указатель на что-то базовое

	selection_type_t _selectionType;	//Тип Селекции

    crossing_type_t _crossingType;		//Тип Скрещивания
	Type _cross_probability;			// вероятность скрещивания

	struct mutate_set
	{
		mutation_type_t mutationType; //тип мутации
		Type mutation_probability;	// вероятность мутации
		Type invers_probability;// вероятность инверсии
		int max_mutate_gens;	  // максиальное число генов, которое может быть подверженно мутации ("глубина" мутации)
	}_mutate_state;


	int _population_size;		//Кол-во Геномов(Индивидов,Особей) в поколении (текущее)
	std::vector<gen_chain> _population;
	std::vector<gen_chain> _new_population;

	//наиболее приспособленная особь за всю историю популяции (это и будет ответ, т.к. когда мы не знаем точного решения)
	// мы проводим генерацию поколений,
	//и если область устойчисвости для лучшего решения не велика, особь вероятно "вымрет")
	struct best_individual
	{
		Type fitness; //уровень приспособленности
		gen_chain gens; //ген
	}_best_ever;

	std::vector<Type> _fitness_value; // кеш для функции приспособленности
	std::vector<Type> wheel_array;  // "колесо" вероятностей

public:
	GeneticEngine(Entity* entity, int size = 100);

	void SetEngineCond(int max_population, int _max_iter);

	void SetSelectCond(const selection_type_t selectionType = eSelTupe_RouletteWheel);
	void SetMutateCond(mutation_type_t mutationType = eMutation_BitFlip, Type mutation_probability = 0.1, Type invers_probability = 0.01, int max_mutate_gens = 3);
    void SetCrossCond(crossing_type_t crossingType = eCross_ONE_POINT_RECOMBINATION, Type cross_probability = 0.9);

private:
	void BornPopulation();

	Type fitness_calc(const std::vector<gen_chain>& population);
		
	void wheel_calc();

	void mutation(gen_chain& gen1);

	void cross(const gen_chain& gen1, const gen_chain& gen2);
	
	int find_idx(Type control_val);
	void get_parents_id(int& id1, int& id2);

	void crossover();

	void mutator();

	void selection_backpack();
	
	void selection_equation();
	

public:
	void run();

	void print();
};

#endif //EVOLUATION_ENGINE
