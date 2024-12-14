#include <iostream>
#include <vector>
using namespace std;

// метод отжига с фиксацией ферзя по 1 строке. 
class Annealing {
	int QueenX;
	int QueenY;
	const int SIZE_CHESSBOARD = 8;              //размер шахматной доски
	const double START_TEMPERATURE = 10;        //инициализация температуры нагрева
	const double FINISH_TEMPERATURE = 0.00001; //конечная температура охлаждения
	const int COUNT_ITERATIONS = 1000;        //количество итераций при понижении температуры	
	const double ALPHA = 0.98;                //постоянная альфа используется при вычислении понижения температуры
	bool isShow;

public:
	Annealing(int y, int x = 0) {
		QueenX = x;
		QueenY = y;
		isShow = false;
	}

private:
	struct Cell {
		int energy;           //энергия полученного решения.
		vector<int> val;      //полученное решение (порядок в массиве---столбец, значение---строка)

		Cell& operator=(Cell right_value) {
			val = right_value.val;
			energy = right_value.energy;
			return *this;
		}
	} cell;                   // структура решения 

	  /*	заполняем шахматную доску так чтобы ферзи не попали по горизонтальный и вертикальный удары		*/
	Cell init() {
		Cell cl;
		cl.val.resize(SIZE_CHESSBOARD);
		//выставим ферзи по диагонали шахматной доски
		for (int i = 0; i < SIZE_CHESSBOARD; i++) {
			if (i != QueenX && i != QueenY)cl.val[i] = i; // места на диагонале, где фиксированная фигура не бьет их по вертикале и горизонтале 
		}
		cl.val[QueenX] = QueenY;// фиксированная фигура
		cl.val[QueenY] = QueenX; // недоставленная на диагональ фигура 

		cl.energy = getEnergy(cl); //сразу вычисляем стартовое значение энергии
		return cl;
	}

	/*	переставляет два случайно выбранных ферзя  */
	Cell combination(Cell solution) {
		int first_position = 0;                 // первая диагональ
		int second_position = 0;                //вторая горизонталь

		do {
			first_position = rand() % (SIZE_CHESSBOARD); // пока равно зафиксированной фигуре
		} while (first_position == QueenX);

		do {
			second_position = rand() % (SIZE_CHESSBOARD);   //вторая горизонталь от 0 до 7;
		} while (first_position == second_position || second_position == QueenX); // чтобы не получить равные значения

		swap(solution.val[first_position], solution.val[second_position]);  // меняем строки, сохранением столбца

		return solution;  // вернуть полученную перестановку
	}

	/*	 вычисляет количество шахматных конфликтов (т.е. текущее состояние системы (энергия)).	*/
	int getEnergy(Cell solution) {
		int step_x[] = { -1, 1, -1, 1 }; //обход вверх-влево 
		int step_y[] = { -1, 1, 1, -1 }; //обход вниз-вправо
		int conflicts = 0; //число конфликтов
		int board[8][8]; // 8=SIZE_CHESSBOARD матрица доски
		for (int i = 0; i < 8; i++) //инициализация нулями
			for (int j = 0; j < 8; j++)
				board[i][j] = 0;
		//заполним матрицу шахматной доски
		for (int i = 0; i < SIZE_CHESSBOARD; i++) {
			board[i][solution.val[i]] = 1; // 1---ферзь
		}
		int tempx = 0;
		int tempy = 0;

		// в цикле проверяются конфликты по диагоналям
		for (int i = 0; i < SIZE_CHESSBOARD; i++) {
			int x = i;
			int y = solution.val[i];
			for (int j = 0; j < 4; j++) {
				tempx = x;
				tempy = y;
				while (true) {
					tempx += step_x[j];
					tempy += step_y[j];
					if ((tempx < 0) || (tempx >= (SIZE_CHESSBOARD - 1)) || (tempy < 0) || (tempy >= (SIZE_CHESSBOARD - 1))) { //перемещение осуществляется до тех пор пока не будет достигнут конец верха или низа доски
						if (tempx > 0 && tempx < SIZE_CHESSBOARD && tempy>0 && tempy < SIZE_CHESSBOARD) {
						if (board[tempx][tempy] == 1) // если ферзи бьют друг друга по краям(например [1,7]--[7,1])
							conflicts++;
						}
						break;
					}
					if (board[tempx][tempy] == 1) //Если на этом «пути» попадется ферзь, то увеличиваем счетчик конфликт
						conflicts++;

				}
			}
		}
		return conflicts; // вернуть число конфликтов текущего состояния (энергия)
	}

	/*	Понижает температуру	*/
	double getT(double temperature, int step) {
		return ALPHA * temperature / step;
	}

public:
	/*	Основная работа алгоритма	*/
	int run() {

		double temperature = START_TEMPERATURE;  //текущая температура системы 
		cell = init(); // инициализировали систему
		Cell new_solution; // новое состояние после перестановки
		int i = 1;
		while (true) {
			for (int step = 0; step < COUNT_ITERATIONS; step++) { //шаг цикла
				new_solution = combination(cell);             // запоминаем перестановку
				new_solution.energy = getEnergy(new_solution); // вычисляем ее энергию

				if (new_solution.energy == 0) { // если решение найдено, печать и прекращение поиска
					cell = new_solution;
					result_print();
					isShow = true;
					return new_solution.energy;
				}

				if (new_solution.energy <= cell.energy) { // если число конфликтов в новой системе меньше, то берем новое состояние
					cell = new_solution;
				}
				else {// если число конфликтов в новой системе больше, то берем новое состояние
					double p = exp(-(new_solution.energy - cell.energy) / temperature); // вероятность принять решение заведомо неудовлетворяющее условиям задачи
					if (p > (double)(rand() % 1000) / 1000) { //если вероятность больше, чем случайное число из интервала от 0 до 1 (здесь с точностью до 3 знаков), то принимается «худшее» решение
						cell = new_solution;
					}
				}
			}
			temperature = getT(temperature, i); // понижение температуры(в зависимости от шага)
			if (temperature <= FINISH_TEMPERATURE) // если система "остыла", то выход
				break;
			i++;
		}
		return cell.energy;   // возвращаем полученную энергию, чтобы потом проверить достигли ли мы результата
	}

	/*	Печать результата	*/
	void result_print() {
		for (int i = 0; i < SIZE_CHESSBOARD; i++) {
			for (int j = 0; j < SIZE_CHESSBOARD; j++) {
				cout << ((cell.val[i] == j) ? "Q " : ". ");
				//cout << ((cell.val[i] == j) ? "1 " : "0 "); // отрисовка в виде матрицы
			} cout << endl;
		}
	}
};

/*
метод возврата с фиксацией ферзя по 1 строке.
*/
bool isShow = false; //прекратить выполнение, после отрисовки.
const int SIZE = 8; // Размер.
int board[SIZE][SIZE]; //доска

 //Функция showBoard() - отображает доску.
void showBoard()
{
	for (int a = 0; a < SIZE; ++a)
	{
		for (int b = 0; b < SIZE; ++b)
		{
			std::cout << ((board[a][b]) ? "Q " : ". ");
			//std::cout << ((board[a][b]) ? "1 " : "0 "); //отрисовка в виде матрицы
		}
		std::cout << '\n';
	}
	isShow = true; //доска отрисована
}


// Функция tryQueen() - проверяет можно ли уже установить ферзя в клетку [a,b].
bool tryQueen(int a, int b)
{
	for (int i = 0; i < a; ++i)
	{
		if (board[i][b])
		{
			return false;
		}
	}

	for (int i = 1; i <= a && b - i >= 0; ++i)
	{
		if (board[a - i][b - i])
		{
			return false;
		}
	}

	for (int i = 1; i <= a && b + i < SIZE; i++)
	{
		if (board[a - i][b + i])
		{
			return false;
		}
	}

	return true;
}

// Функция setQueen() - пробует найти результаты решений.
void setQueen(int a, int X=0) // a - номер очередной строки в которую нужно поставить очередного ферзя, X--- зафиксированныый столбец.
{
	if (a == SIZE)
	{
		showBoard();
		return; 
	}
	
	for (int i = 0; i < SIZE; ++i)
	{
		if (isShow) return;  //если решение было найдено

		if (a == 0) i = X; // зфиксированный ферзь в 1 строке в i столбце 

		if (tryQueen(a, i)) // проверяем, в board[a][i]  ферзь(единица) единственная в этой строке, столбце и диагоналях.
		{
			board[a][i] = 1;
			setQueen(a + 1);			
			board[a][i] = 0;
		}
	}
}



int main() {
	
	int column=0;
	cout << "Input column(0,...,7): ";
	cin >> column;

	cout << "Backtracking: "<< '\n';
	setQueen(0, column);
	cout << '\n';

	Annealing test(column); //метод отжига
	cout << "Annealing: "<< '\n';

	while (test.run() != 0)    // если система "остыла", но результат не был найден, то повторить
	{
			cout << "new_step" << endl;  // контроль повторов
	}
	return 0;
}

