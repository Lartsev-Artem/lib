/*
На первом шаге ищется любая точка в p0, гарантированно входящая в выпуклую оболчку.
Это гарантировано будет самая левая из нижних точек.
Вторым шагом сортируем все точеки по полярному углу относительно стартовой точки,
с помощью алгоритма быстрой сортировки за nlog(n).
Третий шаг - проверям последовательно все точки (тройками), и удаляем те из них,
в которых выполняется правый поворот.

Для этого создаем массив, добавляем в него 2 первые точки(гарантировнно входят в оболчку)
Затем проверяем все остальные вершины, с точки зрения последних двух вершин в массиве.
Если тройка образует отрицательный поворот, то удаляем последнюю точку из массива.
Если тройка образует положительный поворот, то текущая точка заносится в массив.
*/
#ifndef CORE_GRAHAM_H
#define CORE_GRAHAM_H

#include "core_interface.h"

#include <algorithm>

template<typename TYPE> class GrahamCore : public HullCore<TYPE>
{
public:

	virtual void  Run(std::vector<Point<TYPE>>& fvect) override
	{
		if (fvect.size() == 1)
		{
			HullCore<TYPE>::hull.push_back(fvect[0]);
			return;
		}

		HullCore<TYPE>::p_start = *std::min_element(fvect.begin(),fvect.end(),
							 [](const Point<TYPE>& a, const Point<TYPE>& b){return a>b;});

		// сортировка по полярному углу
		std::sort(fvect.begin(), fvect.end(),
				  [&](const Point<TYPE>& A, const Point<TYPE>& B)
		{
			Point<TYPE> v = A - HullCore<TYPE>::p_start;
			Point<TYPE> u = B - HullCore<TYPE>::p_start;
			TYPE ux_vy = u[0] * v[1];
			TYPE uy_vx = u[1] * v[0];
			return  ( (ux_vy < uy_vx) || ((ux_vy == uy_vx) && (v.Mod() < u.Mod())));
		}
		);

		/* формируем оболочку*/
		HullCore<TYPE>::hull.clear();
		HullCore<TYPE>::hull.push_back(fvect[0]);  // первые 2 эл-та входят в оболочку
		HullCore<TYPE>::hull.push_back(fvect[1]);

		for (size_t i = 2; i < fvect.size(); i++)
		{
			auto it = HullCore<TYPE>::hull.end();			
			auto _end = --it;
			auto _pre_end = --it;			

			//пока три точки образуют правый поворот			
			while (HullCore<TYPE>::Rotate(*_pre_end, *_end, fvect[i]) <= 0 && it != HullCore<TYPE>::hull.begin())
			{				
				_end = _pre_end;
				_pre_end = --it;				

				HullCore<TYPE>::hull.pop_back();            // удаляем точку			
			}
			
			HullCore<TYPE>::hull.push_back(fvect[i]);		 // левый поворт => добавить точку
		}

		return;
	}
};

#endif //CORE_GRAHAM_H
