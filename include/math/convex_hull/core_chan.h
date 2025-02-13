#ifndef CORE_CHAN_H
#define CORE_CHAN_H

#include "core_interface.h"

#include "core_graham.h"
#include "core_jarvis.h"

template<typename TYPE> class ChanCore : public HullCore<TYPE>
{	
private:

	std::vector<Point<TYPE>> fChan(const  std::vector<Point<TYPE>>& fvect, const int m)
	{

		Point<TYPE> minp = *std::min_element(fvect.begin(),fvect.end(),
									 [](const Point<TYPE>& a, const Point<TYPE>& b){return a>b;});

		int r = ceil(double(fvect.size()) / m);                                                     // [n/m] (в большую сторону)
		std::vector<std::vector<Point<TYPE>>> P1(r);                                                          // массив из r оболочек

		/*созданная p(-1) точка           &&   p0 */
		std::vector<Point<TYPE>> Chan{ Point<TYPE>((minp[0] - 10), minp[1]), minp}; // оболочка Чана


		/* разбиение на подмножества Pi (n)*/
		{
			int h = 0;

			while (h * m < fvect.size())                                       //пока не конец массива
			{
				for (int i = h * m; i < (h + 1) * m && i < fvect.size(); i++)
					/*  m раз  || до конца массива*/
				{
					P1[h].push_back(fvect[i]);    //добавить в Pi-множество точку
				}

				h++;
			}
		}

		/*нахождение оболочки в Pi множествах (nLOGm)*/
		{
			GrahamCore<TYPE> gr;
			for (int i = 0; i < r; i++)
			{
				gr.Run(P1[i]);
				P1[i].resize(gr.get_hull().size());
				P1[i].assign(gr.get_hull().begin(), gr.get_hull().end());
				//P1[i] = Graham(P1[i]);
			}
		}

		/*построение оболчки*/
		{
			JarvisCore<TYPE> jr;
			std::pair<int, int> dot(0, 0);
			int l = 0;

			for (int k = 0; k < m; k++)
			{
				dot = std::pair<int, int>(0, 0);     // обнуление пары

				for (int i = 0; i < r; i++)
				{
					l = jr.Jarvis_Binary_Search(Chan.back(), P1[i]);
					{
						if (HullCore<TYPE>::Rotate(Chan.back(), P1[dot.first][dot.second], P1[i][l]) < 0)
						{
							dot.first = i;
							dot.second = l;
						}

						if (HullCore<TYPE>::Rotate(Chan.back(), P1[dot.first][dot.second], P1[i][l]) == 0
								&& Chan.back().Dist(P1[dot.first][dot.second]) < Chan.back().Dist(P1[i][l]))
						{
							dot.first = i;
							dot.second = l;
						}
					}
				}/*for(i<r)*/

				Chan.push_back(P1[dot.first][dot.second]);

				if (P1[dot.first].size() != 1)
				{
					P1[dot.first].erase(P1[dot.first].begin() + dot.second);    // удаление точки, вошедшей в оболчку(кроме первой)
				}

				if (Chan.back() == Chan[1]) // если пришли в начало => оболочкаа построена (Chan[1] т.к. Chan[0]- создана)
				{
					break;
				}

			}/*for(k<m)*/

			Chan.erase(Chan.begin()); // удаление первой созданной точки(p(-1))

		}

		return Chan;
	}
public:

	virtual void Run(std::vector<Point<TYPE>>& fvect) override
	{

		int m;
		std::vector<Point<TYPE>> Sp;

		for (int t = 1; t < fvect.size(); t++)
		{
			m=std::min((size_t)pow(2, pow(2, t)), fvect.size()); // минимум из 2^(2^t) и n
			Sp = fChan(fvect, m);                      // алгоритм Чана для выбранного m

			if (Sp.back() == Sp.front())
			{
				break;    // оболочка построена
			}
		}

		Sp.pop_back();

		HullCore<TYPE>::hull.assign(Sp.begin(),Sp.end());
	}

};

#endif //CORE_CHAN_H
