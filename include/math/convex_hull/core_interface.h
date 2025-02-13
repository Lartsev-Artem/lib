#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include "point.h"

#include <list>
#include <vector>

template<typename M_TYPE> class HullCore
{
public:

	virtual void Run(std::vector<Point<M_TYPE>>& fvect) = 0;

	const std::list<Point<M_TYPE>>& get_hull() const
	{
		return hull;
	}

public:
	int Rotate(const Point<M_TYPE>& A, const Point<M_TYPE>& B, const Point<M_TYPE>& C) const
	{
		Point<M_TYPE> u = B - A; //вектор АB
		Point<M_TYPE> v = C - B; //вектор ВС
		M_TYPE cross = u[0] * v[1] - u[1] * v[0];   //векторное произведение

		if (cross < 0)
		{
			return -1;
		}
		else
			if (cross > 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
	}

protected:
	Point<M_TYPE> p_start;
	std::list<Point<M_TYPE>> hull;
};


#endif //CORE_INTERFACE_H
