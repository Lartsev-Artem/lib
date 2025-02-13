#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <stdint.h>
#include <math.h>

/**
 *\brief  ласс обертка дл€ линейной интерпол€ции
 */
template<typename Type, int64_t factor = 10>
class IntInterpolator
{
public:
	IntInterpolator():
	_size(0),
	_left_bound(0),
	_right_bound(0),
	_step(0),
	_tab(nullptr)
	{
	}

	~IntInterpolator()
	{
		if(_tab)
		{
			delete[] _tab;
		}
	}

	/**
	 * @brief Init
	 * @param a - лева€ граница отрезка
	 * @param b - права€ граница отрезка
	 * @param N - число точек сетки
	 * @param function - интерполируема€ функци€
	 */
	void Init(const Type a, const Type b, const unsigned N, double (*function)(double))
	{
		_size = N;
		_left_bound = a << factor;
		_right_bound = b << factor;
		_step = (_right_bound-_left_bound)/(N-1);
		if(_tab)
		{
			delete[] _tab;
		}
		_tab = new Type[N];

		double dx = ((double)(b-a))/(N-1);
		for(unsigned i = 0; i < N; i++)
		{
			_tab[i]=llround((1<<factor)*(1<<factor)*function((double)a+i*dx));
		}
	}

	/**
	 * @brief »нтерпол€ци€ по средней точки
	 */
	Type AvgPoint(Type x) const
	{
		x <<= factor;
		int idx = (x-_left_bound)/_step;

		if(idx >= _size-1)
		{
			return _tab[_size-1]>>(factor+factor);
		}

		if(idx < 0)
		{
			return _tab[0]>>(factor+factor);
		}

		return _tab[idx]>>(factor+factor);
	}

	/**
	 * @brief Ћинейна€ интерпол€ци€
	 */
	Type Linear(Type x) const
	{
		x <<= factor;
		int idx = (x-_left_bound)/_step;

		if(idx >= _size-1)
		{
			idx = _size-2;
		}

		if(idx < 0)
		{
			idx = 0;
		}

		Type k = ((_tab[idx+1]-_tab[idx]))/_step;
		Type m = _tab[idx] - k * (_left_bound+ idx * _step);
		return (k*x + m)>>(factor+factor);
	}

private:
	unsigned _size;		///< число узлов сетки
	Type _left_bound;	///< лева€ граница отрезка
	Type _right_bound;	///< права€ граница отрезка
	Type _step;			///< шаг сетки
	Type* _tab;			///< табул€ци€ функци€
};

#endif //INTERPOLATION_H
