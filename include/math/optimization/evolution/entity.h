#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <vector>

typedef double Type;
typedef uint32_t BitsType;
typedef std::vector<BitsType> gen_chain;

#define bitsType_size (sizeof(BitsType) * 8)

#define get_frame(pos) (pos / bitsType_size)
#define get_frame_count(size) ((size + bitsType_size - 1) / bitsType_size) // DIMA
#define get_num_bit(pos) (pos % bitsType_size)

#define check_bit(word, idx) ((word >> (idx)) & 0x1)  // проверка i-го бита
#define clear_bit(word, idx) (word & (~(1 << (idx)))) // выключение i-го бита
#define set_bit(word, idx) (word | (1 << (idx)))      // установка i-го бита

#define EXIT_ERR(str) {printf(str); exit(1);}

typedef enum EntityId
{
	eEntityId_default,
	eEntityId_backpack,
	eEntityId_equation
}entity_id_t;


#include <random>
// это абстрактный класс сущности, который используется в GeneticEngineClass
class Entity
{
public:
	inline int size() const noexcept { return _size; }
	inline entity_id_t id() const noexcept  { return _unique_id; }

	virtual Type FitnessFunction(const gen_chain& genom) = 0;
	
	virtual void born(gen_chain& gen) = 0;
	
	// для добавления новых мутаций и скрещиваний нужны аналогичные функции
	virtual void mutate_bitflip(gen_chain& gen) = 0;
	virtual void cross_wheel(gen_chain& gen1, const gen_chain& gen2) = 0;

	virtual std::pair<Type, int> cond(const std::vector<Type>& fit, Type base_value) = 0; // условие для выбора лучшего состояния
	virtual void print_answer(gen_chain& gen) = 0;

	virtual bool stop_iter(const Type val) = 0;

	template <typename T>
	T rand(const T min, const T max)
	{
		// Engines:	
		std::mt19937 e2(std::random_device{}());
		//std::knuth_b e2(rd());
		//std::default_random_engine e2(rd()) ;

		// Distribtuions:		
		//std::normal_distribution<> dist(2, 2);
		//std::student_t_distribution<> dist(5);
		//std::poisson_distribution<> dist(2);
		//std::extreme_value_distribution<> dist(0,2);		
		return std::uniform_real_distribution<>(min, max)(e2);
	}

protected:
	void born_gen(gen_chain& gen, int left, int right)
	{
		std::generate(gen.begin(), gen.end(), [min = left, max = right]() {
			std::mt19937 e2 = std::mt19937(std::random_device{}());
			return std::uniform_real_distribution<double>(min, max)(e2);
			});
	}

protected:
	entity_id_t _unique_id;  //id сущности
	int _size; // размерность задачи

};

#endif // ENTITY_H