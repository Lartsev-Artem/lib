/**
 * @file xor_shift.h
 * @author Artem (you@domain.com)
 * @brief Генерация псевдослучайных чисел на основе алгоритма xor_shift
 * @note Удобен тем, что зная стартовое значение возможно востановить всю цепочку чисел
 * @version 0.1
 * @date 2024-08-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/* Example
* #include "xor_shift.h"
#include <iostream>
int main() {
	const int N = 10000;
	math::XorShiftGenerator gen;
	std::vector <int> sum(10, 0);
	for (size_t i = 0; i < N; i++)
	{
		sum[gen.Random(0, sum.size() - 1)]++;
	}

	std::cout << "Number of occurrences out of " << N << " iterations" << std::endl;
	for (size_t i = 0; i < sum.size(); i++)
	{		
		std::cout << i << ": " << sum[i] << std::endl;
	}

	for (size_t i = 0; i < 10; i++)
	{
		std::cout << "RandomFloat: " << gen.RandomF() << std::endl;
	}
	

	std::cout << "Number of occurrences out of 3 weighted indexes with propability 10/30/60%" << std::endl;
	std::vector<float> weights = { 1,3,6 };	 
	std::vector<int> distribution_statistics(weights.size());  // статистика распределения чисел 

	for (int i = 0; i < 100; i++) {
		distribution_statistics[gen.GetRandomWeightedIndex(weights)]++; // счетчик индексов
	}

	for (int i = 0; i < distribution_statistics.size(); i++) {
		std::cout << i << ": " << distribution_statistics[i] << std::endl;	//вывод статистики	
	}
	return 0;
}

*/

#include <stdint.h>
#include <vector>
#include <ctime>

#if 0 
/*генерация индексов с нужным весом*/
int main() {
	test = (uint64_t)time(0);  // инициализируем текущей датой

	vector<float> weights = { 1,1,1 }; // массив весов. Чем больше вес, тем чаще встречается индекс(индекс массива данных, которые надо распределить) 	
	// float т.к. часто удобно использовать не целые веса

	vector<int> distribution_statistics;  // статистика распределения чисел 
	distribution_statistics.resize(weights.size()); // размерность совпадает с размерностью массива весов

	for (int i = 0; i < 100; i++) {
		distribution_statistics[RandomChoiceFollowingDistribution(weights)]++; // счетчик индексов
	}

	for (int i = 0; i < distribution_statistics.size(); i++) {
		cout << i << ": " << distribution_statistics[i] << endl;	//вывод статистики	
	}
	return 0;
}

/* генерация случаных чисел из диапазона*/
//int main() {
//  uint64_t test = (uint64_t)time(0);  // инициализируем текущей датой
//	cout << "Numbers:" << endl;
//	for (int i = 1; i < 10; i++) {
//		test = xorshift64s(new xorshift64s_state(test));
//		cout << i << ": " << test<< endl;
//		}	
//	cout<< endl << "Get numbers from range" << endl;
//	for (int i = 1; i < 10; i++) {
//		test = xorshift64s(new xorshift64s_state(test));
//		cout << i << ": " << test % 10 << endl;
//	}
//	return 0;
//}
//
#endif


namespace math
{

	/**
	 * @brief Класс реализует генератор псведослучайных чисел
	 *
	 */
	class XorShiftGenerator
	{
	public:
		XorShiftGenerator(uint64_t init_value = 0);

		/**
		 * @brief Генерация псведослучайного целого безнакового чисела в указанном диапазоне
		 *
		 * @param start левая граница возможно числа
		 * @param end  правая граница возможно числа
		 * @return псведослучайное число
		 */
		uint32_t Random(uint32_t start = 0, uint32_t end = UINT16_MAX);


		/**
		 * @brief Генерация псведослучайного числа с плавающей точкой
		 *
		 * @param start левая граница возможно числа
		 * @param end  правая граница возможно числа
		 * @return псведослучайное число
		 */
		float RandomF(float start = 0.0f, float end = 1.0f);

		/**
		 * @brief Выбор случайного индекса с вероятностью заданной весом
		 * @note веса лучше задавать целыми числами. Вероятность каждого индекса определяется отношением веса к сумме всех весов
		 * @param[in] weights массив весов
		 * @return индекс
		 */
		int GetRandomWeightedIndex(const std::vector<float>& weights);


	private:
		uint64_t xorshift64s(); //используем битовый сдвиг для получения нового числа 

		uint64_t _xorshift64s_state;	///< текущее значение определяющие псевдослучайное число
	};


	XorShiftGenerator::XorShiftGenerator(uint64_t init_value) : _xorshift64s_state(init_value)
	{
		if (init_value == 0)
		{
			_xorshift64s_state = (uint64_t)time(0);
		}
	}

	uint64_t XorShiftGenerator::xorshift64s()
	{
		/*
		Принцип:
		a ->   010000101100100110   /переводим в двоичное представление
					   |
					   V
					  010000101100100110
		010000101100100110                  /сдвигаем три раза получаем
			   010000101100100110
					   |
					   V
		0100001_******************_0100110  // складываем - получаем новое число
					   |
					   V
				1*****************  -> a	    // отсекаем лишние и переводим в десятичное представление

		*/
		uint64_t x = _xorshift64s_state;
		x ^= x >> 12; // a  
		x ^= x << 25; // b
		x ^= x >> 27; // c

		/// \note в классическом xor_shift: state=x, но домножение на "случайное" значение позволяет лучше приблизится к нормальному распределению
		_xorshift64s_state = x * UINT64_C(0x2545F4914F6CDD1D); // новое большое число
		return _xorshift64s_state;
	}

	uint32_t XorShiftGenerator::Random(uint32_t start, uint32_t end)
	{
		if (end < start)
		{
			return -1;
		}

		return xorshift64s() % (end - start + 1) + start;
	}

	float XorShiftGenerator::RandomF(float start, float end)
	{
		uint64_t whole_limit = (uint64_t)(end - start + 1) + start;
		uint64_t frac_limit = ((uint64_t)(end * 1e6) % 1000000);

		uint64_t frac = (xorshift64s() % 1000000);
		if (frac_limit != 0)
		{
			frac = frac % frac_limit;
		}
		else
		{
			whole_limit--;
		}
		uint64_t whole = xorshift64s() % whole_limit;
		return whole + frac / 1e6;
	}

	//Выбор случайного элемента из списка весов
	int XorShiftGenerator::GetRandomWeightedIndex(const std::vector<float>& weights)
	{
		const size_t size = weights.size();

		// Сумма для создания случайного распределения 
		std::vector<float> cdf(size, 0);
		float sum = 0;
		for (int i = 0; i < size; ++i)
		{
			sum += weights[i];
			cdf[i] = sum;
		}

		// Выбор числа из распределения:		 	
		float cdf_value = RandomF(0.0f, cdf[size - 1]); // генерируем псевдослучайные числа

		int index = 0; // останется 0, если число менье первого веса
		for (int i = 0; i < size - 1; i++)
		{
			// полученному числу соответвует ближайший  индекс справа 
			if (cdf_value > cdf[i] && cdf_value <= cdf[i + 1])
			{
				index = i + 1;
				break;
			}
		}

		return index;
	}
} //namespace math
