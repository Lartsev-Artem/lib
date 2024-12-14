#pragma once
#include<fstream>
#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<utility>
#include <algorithm> 
#include <ctime>  // RandCoord
#include <random> // RandCoord

double startX = 0;
double startY = 0;

template<typename M_TYPE = double> class Coord;
using namespace std;
const double eps = 1e-10;
template<typename M_TYPE = double> class Coord {
private:
	M_TYPE m_x;
	M_TYPE m_y;
public:
	int copy(const Coord& rDot) {
		m_x = rDot.m_x;
		m_y = rDot.m_y;
		return 0;
	}
	Coord(const M_TYPE x = 0, const M_TYPE y = 0) {
		m_x = x;
		m_y = y;
	}
	Coord(const Coord& dot) {
		copy(dot);
	}
	M_TYPE getX() const {
		return m_x;
	}
	M_TYPE getY() const {
		return m_y;
	}
	void SetX(M_TYPE x) {
		m_x = x;
	}
	void SetY(M_TYPE y) {
		m_y = y;
	}
	Coord& operator = (const Coord& a) {
		copy(a);
		return *this;
	}
	Coord& operator = (Coord&& a) {
		copy(a);
		return *this; 
	}  // используется в std::swap
	bool operator !=(const Coord next) const {
		if ((m_x != next.m_x) || (m_y != next.m_y)) return true;
		else return false;
	}// не равны координаты
	bool operator ==(const Coord next) const {
		return !(next != *this);
	}//равны координаты
	bool operator >(const Coord& next)const {
		if ((next.m_y < m_y) || ((next.m_x < m_x) && (next.m_y <= m_y))) return true;
		else return false;
	}// (1) самая нижняя || (2) самая левая из нижних точка
	double Mod() const {
		return sqrt(pow(m_x, 2) + pow(m_y, 2));
	}// расстояние от начала координат
	double Dist(const Coord& V) const {
		return  sqrt(pow(m_x - V.getX(), 2) + pow(m_y - V.getY(), 2));
	}// расстояние до точки V
};
template<typename M_TYPE> Coord<M_TYPE> operator-(const Coord<M_TYPE>& A, const Coord<M_TYPE>& B) {
	return Coord<M_TYPE>(A.getX() - B.getX(), A.getY() - B.getY()); // A-B
}
template<typename M_TYPE> ostream& operator<<(ostream& os, const Coord<M_TYPE>& dot) {
	os << "x= " << dot.getX() << " y= " << dot.getY() << endl;
	return os;
} // вывод на экран по эл-ам
template<typename M_TYPE> ostream& operator<<(ostream& os, const vector<Coord<M_TYPE>>& points) {
	for (auto el : points)
		cout << el;
	return os;
} // вывод на экран весь vector
template<typename M_TYPE> ostream& operator<<(ostream& os, const list<Coord<M_TYPE>>& points) {
	for (auto el : points)
		cout << el;
	return os;
} // вывод на экран весь list
template<typename M_TYPE> ifstream& operator>>(ifstream& ifs, Coord<M_TYPE>& dot) {
	M_TYPE x, y;
	ifs >> x >> y;
	dot.SetX(x); dot.SetY(y);
	return ifs;
} // чтение из файла 
template<typename M_TYPE> ofstream& operator<<(ofstream& ofs, const Coord<M_TYPE>& dot) {
	ofs << dot.getX() << " " << dot.getY() << endl;
	return ofs;
}// запись в файл эл-та
template<typename M_TYPE> ofstream& operator<<(ofstream& ofs, const list<Coord<M_TYPE>>& points) {
	ofs << points.size() << endl;
	for (auto el : points)
		ofs << el;
	return ofs;
}// запись в файл list
template<typename M_TYPE> ofstream& operator<<(ofstream& ofs, const vector<Coord<M_TYPE>>& points) {
	ofs << points.size() << endl;
	for (auto el : points)
		ofs << el;
	return ofs;
}// запись в файл vector
template<typename TYPE>  int RandCoord(string name, const int n, const int range) { //колв-во точек, радиус точек
	if (n <= 0) return 1;
	ofstream ofs(name);
	if (!ofs) return 2;
	ofs << n << endl;
	std::mt19937 gen(time(0));
	std::uniform_real_distribution<> urd(-range, range);
	for (int i = 0; i < n; ++i) {
		ofs << Coord<TYPE>((TYPE)urd(gen), (TYPE)urd(gen));
	}
	ofs.close();
	return 0;
};// генератор координат
template<typename TYPE>  int ReadFromFile(string nameFile, vector<Coord<TYPE>>& points) {
	ifstream inf(nameFile);
	if (!inf) {
		cout << "No opened" << endl;
		return 1;
	}
	int n = 0;
	if (!(inf >> n) || n <= 0) {
		cout << "The first element is invalid" << endl;
		inf.close();
		return 2;
	}
	points.reserve(n);
	Coord<TYPE> curPoint;
	if (inf >> curPoint)                 // считываем первую точку
		points.push_back(curPoint);        //добавляем в массив точек 
	while (inf >> curPoint) {         // пока чтение из файла без ошибок(или до конца)
		points.push_back(curPoint);
		if (points[0] > curPoint)      // если текущая точка меньше начальной
			swap(points[0], points.back());
	}
	inf.close();
	return 0;
}
template<typename TYPE> int OutFile(const string name, const list<Coord<TYPE>>& ls) {
	ofstream ofs(name);
	if (!ofs) { 
		cout<<"Opening error file (list)"<<endl;
		return 1; }
	ofs << ls;
	ofs.close();
	return 0;
}// вывод списка
template<typename TYPE> int OutFile(const string name, const vector<Coord<TYPE>>& v) {
	ofstream ofs(name);
	if (!ofs) {
		cout << "Opening error file (vector)" << endl;
		return 1;
	}
	ofs << v;
	ofs.close();
	return 0;
}// вывод массива
template<typename TYPE> list<Coord<TYPE>> MySpan(const vector<Coord<TYPE>>& points) {
	list<Coord<TYPE>> fSpan{ points[0] };
	Coord<TYPE> curPoint = points[0];
	TYPE Dot;
	int k = 0;                             // счетчик  сколько точек выше/ниже прямой)

	do {	                            //do{} while т.к. все данные начинаются с vect[0] и условие сразу будет выполнено 
		for (int i = 0; i < points.size(); i++) {
			for (int j = 0; j < points.size(); j++) {
				Dot = ((curPoint.getY() - points[i].getY()) * points[j].getX() +                  // (y1-y2)X
					(points[i].getX() - curPoint.getX()) * points[j].getY() +                     // (x2-x1)Y
					(curPoint.getX() * points[i].getY() - points[i].getX() * curPoint.getY()));   // (x1y2-x2y1) 
				// (y1-y2)X+(x2-x1)Y+(x1y2-x2y1)=0 <=> Ax+By+C=0, выводится из канонического ур-я прямой 	

				if (Dot > eps)
					k++;
				else if (abs(Dot) < eps && curPoint.Dist(points[i]) < curPoint.Dist(points[j]))
					k++;
			}
			if (((k == 0 || k == points.size())) && (points[i] != curPoint)) {   // если {(все точки ниже прямой) || (ни одна)} и (полученая точка не предыдущая[иначе может зациклиться])
				if (points[i] != points[0])                            // если не пришли в начало(избежать двойного включения точки)
					fSpan.push_back(points[i]);
				curPoint = points[i];                 // смещаем начало отсчета для следующей итерации
				break;                                            // т.к. точка найдена выходим из for(i)
			}
			k = 0;
		}
	} while (curPoint != points[0]);    // пока не пришли в начало

	return fSpan;
}
template<typename TYPE> int Rotate(const Coord<TYPE>& A, const Coord<TYPE>& B, const Coord<TYPE>& C) {
	Coord<TYPE> u = B - A; //вектор АB
	Coord<TYPE> v = C - B; //вектор ВС
	if (u.getX() * v.getY() - u.getY() * v.getX() < 0) return -1;    //векторное произведение 
	else if (u.getX() * v.getY() - u.getY() * v.getX() > 0) return 1;
	else return 0;
}
template<typename TYPE> int Jarvis_Binary_Search(const Coord<TYPE>& p1, const vector<Coord<TYPE>>& fvect) {
	if (fvect.size() == 1) {
		return 0;
	}

	int mid_before;   int mid_after;
	int l_mid;        int r_mid;
	int l_after;
	int mid_l;        int mid_r;
	int l_r;

	int n = fvect.size();
	int left; (p1 == fvect[0]) ? left = 1 : left = 0;
	int right = fvect.size() - 1;
	int mid = 0;

	while (left <= right) {
		mid = (right + left) >> 1;

		if (mid == 0) {
			if (right != 0) {
				if (Rotate(p1, fvect[mid], fvect[right]) > 0 && Rotate(p1, fvect[right], fvect[mid]) < 0) break;
				else mid = right;
			}
			else break;
		}

		mid_before = Rotate(p1, fvect[mid], fvect[(mid - 1) % n]);
		mid_after = Rotate(p1, fvect[mid], fvect[(mid + 1) % n]);
		l_mid = Rotate(p1, fvect[left], fvect[mid]);
		r_mid = Rotate(p1, fvect[right], fvect[mid]);
		l_after = Rotate(p1, fvect[left], fvect[(left + 1) % n]);
		mid_r = Rotate(p1, fvect[mid], fvect[right]);
		mid_l = Rotate(p1, fvect[mid], fvect[left]);
		l_r = Rotate(p1, fvect[left], fvect[right]);

		if ((mid_before > 0) && (mid_after > 0)) {
			break;
		}// точка найдена

		if ((mid_before) < 0 && (mid_after > 0)) {
			if ((l_mid < 0 && l_after > 0) || (l_mid > 0 && r_mid < 0) || (mid_l > 0 && mid_r > 0) || (mid_l < 0 && mid_r < 0 && l_r>0))
				right = mid - 1;  // точка в начале
			else left = mid + 1;  // точка в конце
		}

		if ((mid_before) > 0 && (mid_after < 0)) {
			left = mid + 1; // точка в конце
		}

		if ((mid_before) < 0 && (mid_after < 0)) {
			if (l_mid > 0 && l_r > 0)
				right = mid - 1;
			else left = mid + 1;// точка в конце
		}

		if (mid_after == 0 || mid_before == 0) {    // точки на одной прямой
			if ((mid_after == 0) || (mid_after < 0))
				left = mid + 1;
			else right = mid - 1;
		}

	}// while
	return mid;
}
template<typename TYPE> int Jarvis_Next_Point(const Coord<TYPE>& p1, const vector<Coord<TYPE>>& fvect) {
	if (fvect.size() == 1) return 0;
	int j = 1;
	for (int i = 0; i < fvect.size(); ++i) {
		if (Rotate(p1, fvect[j], fvect[i]) < 0)
			j = i;
		if (Rotate(p1, fvect[j], fvect[i]) == 0 && p1.dist(fvect[j]) < p1.dist(fvect[i]))
			j = i;
	}
	return j;
}
template<typename TYPE> bool cmp(const Coord<TYPE > A, const Coord<TYPE > B) {
	Coord<TYPE> start(startX, startY);
	Coord<TYPE> v = A - start;
	Coord<TYPE> u = B - start;
	if ((u.getX() * v.getY() < u.getY() * v.getX()) ||
		((u.getX() * v.getY() == u.getY() * v.getX()) && (v.Mod() < u.Mod()))) return true;
	return false;
};
template<typename TYPE> vector<Coord<TYPE>> Graham(vector<Coord<TYPE>> fvect) {
	{
		Coord<TYPE> cur(fvect[0]);
		for (int i = 0; i < fvect.size(); i++)
			if (cur > fvect[i])      // если текущая точка меньше начальной
				cur = fvect[i];
		startX = cur.getX();
		startY = cur.getY();
	}

	vector<Coord<TYPE>> fS;
	if (fvect.size() == 1) {
		fS.push_back(fvect[0]);
		return fS;
	}
	sort(fvect.begin(), fvect.end(), cmp<TYPE>); // сортировка по полярному углу

	/* формируем оболочку*/
	fS.push_back(fvect[0]);  // первые 2 эл-та входят в оболочку
	fS.push_back(fvect[1]);
	for (int i = 2; i < fvect.size(); i++) {
		while (Rotate(fS[fS.size() - 2], fS.back(), fvect[i]) <= 0) {         //пока три точки образуют правый поворот
			fS.pop_back();                                                    // удаляем точку
			if (fS.size() == 1) { fS.push_back(fvect[i]); break; }
		}
		fS.push_back(fvect[i]);                                               // левый поворт => добавить точку
	}

	return fS;         // возвращаем сформированный vector
}
template<typename TYPE> vector<Coord<TYPE>> Chan(const vector<Coord<TYPE>>& fvect, const int m) {

	int r = ceil(double(fvect.size()) / m);                                                       // [n/m] (в большую сторону)            
	vector<vector<Coord<TYPE>>> P1(r);                                                          // массив из r оболочек
	vector<Coord<TYPE>> Chan{ Coord<TYPE>((fvect[0].getX() - 10), fvect[0].getY()),fvect[0] };  // оболочка Чана
								   /*созданная p(-1) точка           &&   p0 */

	/* разбиение на подмножества Pi (n)*/
	{
		int h = 0;
		while (h * m < fvect.size()) {                                     //пока не конец массива                  
			for (int i = h * m; i < (h + 1) * m && i < fvect.size(); i++)
				/*  m раз  || до конца массива*/
				P1[h].push_back(fvect[i]);                                //добавить в Pi-множество точку

			h++;
		}
	}

	/*нахождение оболочки в Pi множествах (nLOGm)*/
	{
		for (int i = 0; i < r; i++)
			P1[i] = Graham(P1[i]);
	}

	/*построение оболчки*/
	{
		pair<int, int> dot(0, 0);
		int l = 0;

		for (int k = 0; k < m; k++) {
			dot = pair<int, int>(0, 0);     // обнуление пары
			for (int i = 0; i < r; i++) {
				l = Jarvis_Binary_Search(Chan.back(), P1[i]);
				{
					if (Rotate(Chan.back(), P1[dot.first][dot.second], P1[i][l]) < 0) {
						dot.first = i;
						dot.second = l;
					}
					if (Rotate(Chan.back(), P1[dot.first][dot.second], P1[i][l]) == 0
						&& Chan.back().Dist(P1[dot.first][dot.second]) < Chan.back().Dist(P1[i][l])) {
						dot.first = i;
						dot.second = l;
					}
				}
			}/*for(i<r)*/

			Chan.push_back(P1[dot.first][dot.second]);

			if (P1[dot.first].size() != 1)
				P1[dot.first].erase(P1[dot.first].begin() + dot.second); // удаление точки, вошедшей в оболчку(кроме первой)

			if (Chan.back() == Chan[1]) // если пришли в начало => оболочкаа построена (Chan[1] т.к. Chan[0]- создана)
				break;

		}/*for(k<m)*/
		Chan.erase(Chan.begin()); // удаление первой созданной точки(p(-1))

	}

	return Chan;
}
template<typename TYPE> vector<Coord<TYPE>> ChanSpan(vector<Coord<TYPE>> fvect) {
	int m;
	vector<Coord<TYPE>> Sp;
	for (int t = 1; t < fvect.size(); t++) {
		(pow(2, pow(2, t)) < fvect.size()) ? (m = pow(2, pow(2, t))) : (m = fvect.size()); // минимум из 2^(2^t) и n
		Sp = Chan(fvect, m);                       // алгоритм Чана для выбранного m
		if (Sp.back() == Sp.front()) break;        // оболочка построена
	}
	Sp.pop_back();
	return Sp;
}
