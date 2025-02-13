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
#ifndef CORE_JARVIS_H
#define CORE_JARVIS_H

#include "core_interface.h"

#include <algorithm>

template<typename TYPE> class JarvisCore : public HullCore<TYPE>
{
public:
	virtual void  Run(std::vector<Point<TYPE>>& fvect) override
	{
		Point<TYPE> minp = *std::min_element(fvect.begin(),fvect.end(),
									 [](const Point<TYPE>& a, const Point<TYPE>& b){return a>b;});
		HullCore<TYPE>::hull.clear();
		HullCore<TYPE>::hull.push_back(minp); // нижняя точка всегда входит в оболочку
		std::swap(fvect[0], fvect.back());		  // начальную точку в конец
		int k = 0;                                // счетчик для определения правово поворота

		do
		{
			k = 0;

			for (int i = 0; i < fvect.size(); ++i)
			{
				if (HullCore<TYPE>::Rotate(HullCore<TYPE>::hull.back(), fvect[k], fvect[i]) > 0) // наименьший поворот относительно последней точки оболочки
				{
					k = i;
				}
			}

			if (!(fvect[k] != HullCore<TYPE>::hull.front()))
			{
				break;    // если начало равно текущей, то оболчка построена
			}
			else
			{
				HullCore<TYPE>::hull.push_back(fvect[k]);                      //добавить найденный эл-т
				fvect.erase(fvect.begin() + k);                 // удалить этот эл-т из исходного массива
			}
		}
		while (true);

		return;
	}

public:
	 size_t Jarvis_Binary_Search(const Point<TYPE>& p1, const std::vector<Point<TYPE>>& fvect)
	{
		if (fvect.size() == 1)
		{
			return 0;
		}

		int mid_before;
		int mid_after;
		int l_mid;
		int r_mid;
		int l_after;
		int mid_l;
		int mid_r;
		int l_r;

		size_t n = fvect.size();
		size_t left;
		(p1 == fvect[0]) ? left = 1 : left = 0;
		size_t right = fvect.size() - 1;
		size_t mid = 0;

		while (left <= right)
		{
			mid = (right + left) >> 1;

			if (mid == 0)
			{
				if (right != 0)
				{
					if (HullCore<TYPE>::Rotate(p1, fvect[mid], fvect[right]) > 0 && HullCore<TYPE>::Rotate(p1, fvect[right], fvect[mid]) < 0)
					{
						break;
					}
					else
					{
						mid = right;
					}
				}
				else
				{
					break;
				}
			}

			mid_before = HullCore<TYPE>::Rotate(p1, fvect[mid], fvect[(mid - 1) % n]);
			mid_after = HullCore<TYPE>::Rotate(p1, fvect[mid], fvect[(mid + 1) % n]);
			l_mid = HullCore<TYPE>::Rotate(p1, fvect[left], fvect[mid]);
			r_mid = HullCore<TYPE>::Rotate(p1, fvect[right], fvect[mid]);
			l_after = HullCore<TYPE>::Rotate(p1, fvect[left], fvect[(left + 1) % n]);
			mid_r = HullCore<TYPE>::Rotate(p1, fvect[mid], fvect[right]);
			mid_l = HullCore<TYPE>::Rotate(p1, fvect[mid], fvect[left]);
			l_r =HullCore<TYPE>:: Rotate(p1, fvect[left], fvect[right]);

			if ((mid_before > 0) && (mid_after > 0))
			{
				break;// точка найдена
			}

			if ((mid_before) < 0 && (mid_after > 0))
			{
				if ((l_mid < 0 && l_after > 0) || (l_mid > 0 && r_mid < 0) || (mid_l > 0 && mid_r > 0) || (mid_l < 0 && mid_r < 0 && l_r > 0))
				{
					right = mid - 1;  // точка в начале
				}
				else
				{
					left = mid + 1;  // точка в конце
				}
			}

			if ((mid_before) > 0 && (mid_after < 0))
			{
				left = mid + 1; // точка в конце
			}

			if ((mid_before) < 0 && (mid_after < 0))
			{
				if (l_mid > 0 && l_r > 0)
				{
					right = mid - 1;
				}
				else
				{
					left = mid + 1;    // точка в конце
				}
			}

			// точки на одной прямой
			if (mid_after == 0 || mid_before == 0)
			{
				if ((mid_after == 0) || (mid_after < 0))
				{
					left = mid + 1;
				}
				else
				{
					right = mid - 1;
				}
			}

		}// while

		return mid;
	}
};

#endif //CORE_JARVIS_H
