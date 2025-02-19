#ifndef QUEEN_PROBLEM_H
#define QUEEN_PROBLEM_H

#include "annealing.h"

#include <iostream>
class Board : public Entity
{
	static constexpr int SIZE_CHESSBOARD = 8;	
public:

	double calculate_energy() override;
	
	/*	переставляет два случайно выбранных ферзя  */
	void recombination() override;	
	void copy(Entity*& solution) const override;
	void init() override;	

	void print_result() 
	{
		for (int i = 0; i < SIZE_CHESSBOARD; i++) 
		{
			for (int j = 0; j < SIZE_CHESSBOARD; j++) 
			{
				std::cout << ((queen_map[i] == j) ? "Q " : ". ");				
			}
			std::cout << std::endl;
		}
	}

private:
	std::vector<int> queen_map;      //полученное решение (порядок в массиве---столбец, значение---строка)
};


#endif //QUEEN_PROBLEM_H
