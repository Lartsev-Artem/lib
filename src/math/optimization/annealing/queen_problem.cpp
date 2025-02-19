#include "queen_problem.h"
#include <numeric>


double Board::calculate_energy()
{
	int step_x[] = { -1, 1, -1, 1 }; //обход вверх-влево 
	int step_y[] = { -1, 1, 1, -1 }; //обход вниз-вправо

	std::vector<std::vector<int>> board(SIZE_CHESSBOARD); // матрица доски
	for (int i = 0; i < SIZE_CHESSBOARD; i++)
	{
		board[i].assign(SIZE_CHESSBOARD, 0);
		board[i][queen_map[i]] = 1; // 1---ферзь
	}


	int conflicts = 0; //число конфликтов

	// в цикле проверяются конфликты по диагоналям
	for (int i = 0; i < SIZE_CHESSBOARD; i++)
	{
		int x = i;
		int y = queen_map[i];
		for (int j = 0; j < 4; j++)
		{
			int tempx = x;
			int tempy = y;
			while (true)
			{
				tempx += step_x[j];
				tempy += step_y[j];

				//перемещение осуществляется до тех пор пока не будет достигнут конец верха или низа доски
				if ((tempx < 0) || (tempx >= (SIZE_CHESSBOARD - 1)) || (tempy < 0) || (tempy >= (SIZE_CHESSBOARD - 1)))
				{
					if (tempx > 0 && tempx < SIZE_CHESSBOARD && tempy>0 && tempy < SIZE_CHESSBOARD)
					{
						if (board[tempx][tempy] == 1)
							conflicts++; // если ферзи бьют друг друга по краям(например [1,7]--[7,1])
					}
					break;
				}

				//Если на этом «пути» попадется ферзь, то увеличиваем счетчик конфликт
				if (board[tempx][tempy] == 1)
				{
					conflicts++;
				}
			}
		}
	}
	_energy = conflicts;
	return _energy;
}

	
void Board::recombination()
{
	int first_position = first_position = rand() % (SIZE_CHESSBOARD); // первая диагональ

	int second_position = 0;                //вторая горизонталь
	do {
		second_position = rand() % (SIZE_CHESSBOARD);   //вторая горизонталь от 0 до 7;
	} while (first_position == second_position); // чтобы не получить равные значения

	std::swap(queen_map[first_position], queen_map[second_position]);  // меняем строки, сохранением столбца
	return;  // вернуть полученную перестановку
}

void Board::copy(Entity*& solution) const 
{
	if (!solution)
	{
		solution = new Board();
	}

	((Board*)solution)->queen_map.assign(queen_map.begin(), queen_map.end());
	((Board*)solution)->_energy = _energy;
}

void Board::init()
{				
	queen_map.resize(SIZE_CHESSBOARD);
	std::iota(queen_map.begin(), queen_map.end(), 0); //выставим ферзи по диагонали шахматной доски						
	calculate_energy(); //сразу вычисляем стартовое значение энергии				
}
