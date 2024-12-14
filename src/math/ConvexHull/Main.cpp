#include"TCoord.h"
typedef int TYPE;
using namespace std;

int main() {
	const string FNameAllPoints = "Rand.txt";
	const string FNameMySpanPoints = "MySpan.txt";
	const string FNameChanSpanPoints = "ChSpan.txt";

	//if (RandCoord<TYPE>(FNameAllPoints, 15, 100))        // генерация точек 
			  /*(имя файла, кол-во точек, радиус)*/
		//return 1;                                      // если не ноль => ошибка

	vector<Coord<TYPE>> vect;                    // массив точек	
	ReadFromFile(FNameAllPoints, vect);           // чтения из файла в массив {левая из нижних в начале}
		/*(имя файла, кол-во точек, массив)*/

	list <Coord<TYPE>> Span = MySpan(vect);   // список из точек оболочки
	OutFile(FNameMySpanPoints, Span);        // вывод в файл
			/*(имя файла, список)*/

	vector<Coord<TYPE>> ChanS = ChanSpan(vect);  // список из точек оболочки
	OutFile(FNameChanSpanPoints, ChanS);        // вывод в файл
			 /*(имя файла, список)*/

  /*cout << "All points: " << endl << vect << endl;
	cout << "Span points: "<< endl << Span << endl;
	cout << "Span points: "<< endl << ChanS<< endl; */
	return 0;
};
/*#include <ctime>
clock_t  start_time = clock();
clock_t	end_time = 0;*/