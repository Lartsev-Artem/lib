#include "xor_shift.h"
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