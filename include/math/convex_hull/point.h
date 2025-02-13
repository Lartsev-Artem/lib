#ifndef POINT_H
#define POINT_H

#include<fstream>
#include<iostream>

#include<vector>
#include<list>

#include <math.h>


template<typename M_TYPE = double> class Point;
template<typename M_TYPE> class Point
{
private:
	M_TYPE m_data[2];	
public:

	Point(const M_TYPE x = 0, const M_TYPE y = 0)
	{
		m_data[0] = x;
		m_data[1] = y;
	}
	Point(const Point& dot)
	{
		*this = dot;
	}	

	M_TYPE operator [] (const int i) const
	{
		return m_data[i];
	}
	
	M_TYPE& operator [] (const int i) 
	{
		return m_data[i];
	}

	Point& operator = (const Point& a)
	{
		m_data[0] = a[0];
		m_data[1] = a[1];
		return *this;
	}
	Point& operator = (Point&& a)
	{
		*this = a;
		return *this;
	}  // используетс€ в std::swap
	bool operator !=(const Point next) const
	{
		return ((m_data[0] != next[0]) || (m_data[1] != next[1]));
	}// не равны координаты
	bool operator ==(const Point next) const
	{
		return !(next != *this);
	}//равны координаты
	bool operator >(const Point& next)const
	{
		return ((next[1] < m_data[1]) || ((next[0] < m_data[0]) && (next[1] <= m_data[1])));
	}// (1) сама€ нижн€€ || (2) сама€ лева€ из нижних точка
	double Mod() const
	{
		return sqrt(pow(m_data[0], 2) + pow(m_data[1], 2));
	}// рассто€ние от начала координат
	double Dist(const Point& V) const
	{
		return  sqrt(pow(m_data[0] - V.m_data[0], 2) + pow(m_data[1] - V.m_data[1], 2));
	}// рассто€ние до точки V
};
template<typename M_TYPE> Point<M_TYPE> operator-(const Point<M_TYPE>& A, const Point<M_TYPE>& B)
{
	return Point<M_TYPE>(A[0] - B[0], A[1] - B[1]); // A-B
}
template<typename M_TYPE> std::ostream& operator<<(std::ostream& os, const Point<M_TYPE>& dot)
{
	os  << dot[0] << "," << dot[1] << ";" << std::endl;
	return os;
} // вывод на экран по эл-ам
template<typename M_TYPE> std::ostream& operator<<(std::ostream& os, const std::vector<Point<M_TYPE>>& points)
{
	for (auto el : points)
	{
		std::cout << el;
	}

	return os;
} // вывод на экран весь std::vector
template<typename M_TYPE> std::ostream& operator<<(std::ostream& os, const std::list<Point<M_TYPE>>& points)
{
	for (auto el : points)
	{
		std::cout << el;
	}

	return os;
} // вывод на экран весь std::list

template<typename M_TYPE> std::ofstream& operator<<(std::ofstream& ofs, const Point<M_TYPE>& dot)
{
	ofs  << dot[0] << "," << dot[1] << ";" << std::endl;
	return ofs;
}// запись в файл
template<typename M_TYPE> std::ofstream& operator<<(std::ofstream& ofs, const std::list<Point<M_TYPE>>& points)
{
	for (auto el : points)
	{
		ofs << el;
	}

	return ofs;
}
template<typename M_TYPE> std::ofstream& operator<<(std::ofstream& ofs, const std::vector<Point<M_TYPE>>& points)
{
	for (auto el : points)
	{
		ofs << el;
	}

	return ofs;
}

#endif //POINT_H
