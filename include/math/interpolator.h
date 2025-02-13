#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <functional>
#include <math.h>

/**
 *\brief  ласс обертка дл€ табличной интерпол€ции
 */
template<typename Type>
class Interpolator
{
public:
	Interpolator():
	_size(0),
	_left_bound(0),
	_right_bound(0),
	_step(0),
	_tab(nullptr),
	_func(nullptr)
	{
	}

        ~Interpolator()
        {
            if(_tab)
            {
                delete[] _tab;
            }
        }

	const Type* GetTab() const { return _tab; }
	unsigned GetSize() const { return _size; }
	Type GetStep() const { return _step; }

	/**
	 * @brief Init
	 * @param a - лева€ граница отрезка
	 * @param b - права€ граница отрезка
	 * @param N - число точек сетки
	 * @param function - интерполируема€ функци€
	 */
	void Init(const Type a, const Type b, const unsigned N, std::function<Type(Type)> function )
	{
		_size = N;
		_left_bound = a;
		_right_bound = b;
                _step = (b-a)/(N-1);
		if(_tab)
		{
			delete[] _tab;
		}
		_tab = new Type[N];
		_func = function;

		for(unsigned i = 0; i < N; i++)
		{
                    _tab[i]=_func(a+i*_step);
		}
	}

	/**
	 * @brief »нтерпол€ци€ по средней точки
	 */
	Type AvgPoint(Type x) const
	{		
		int idx = floor((x-_left_bound)/_step);

                if(idx >= _size-1)
                {
                    return _tab[_size-1];
                }

                if(idx < 0)
                {
                    return _tab[0];
                }

                return _tab[idx];
	}

	/**
	 * @brief Ћинейна€ интерпол€ци€
	 */
	Type Linear(Type x) const
	{
		int idx = floor((x-_left_bound)/_step);

		if(idx >= _size-1)
		{
                        idx = _size-1;
			Type k = (_tab[idx]-_tab[idx-1])/_step;
			Type m = _tab[idx] - k * (_left_bound+ idx * _step);
			return k*x + m;
		}

		if(idx < 0)
		{
			idx = 1;
			Type k = (_tab[idx]-_tab[idx-1])/_step;
			Type m = _tab[idx] - k * (_left_bound+ idx * _step);
			return k*x + m;
		}


		Type k = (_tab[idx+1]-_tab[idx])/_step;
		Type m = _tab[idx] - k * (_left_bound+ idx * _step);
		return k*x + m;
	}


	enum e_norm_type
	{
		e_norm_max, //абсолютна€ норма
		e_norm_l2   //интегральна€ норма
	};

	enum e_interpolation_type
	{
		e_inter_avg,
		e_inter_linear
	};

	/**
	 * @brief ѕроверка качества интерпол€ции
	 * @param dx - шаг по отрезку
	 * @param type_int - тип интерпол€ции
	 * @param norm_type - тип нормы
	 * @return норма ошибки
	 */
	Type CheckQuality(double dx, e_interpolation_type type_int, e_norm_type norm_type) const
	{
		//std::function<Type(Type)> f = (type_int==e_inter_avg) ? AvgPoint() : Linear();
		//Type (*f)(Type) = (type_int==e_inter_avg) ? AvgPoint() : Linear();

		Type x = _left_bound;
		Type norm = 0;
		Type df=0;

		while(x < _right_bound)
		{
			if(type_int==e_inter_linear)
			{
				df = fabs(Linear(x) - _func(x));
			}
			else
			{
				df = fabs(AvgPoint(x) - _func(x));
			}


			if(norm_type == e_norm_max)
			{
				if(df > norm)
				{
					norm = df;
				}
			}
			else
			{
				norm += df*df;
			}
			x += dx;
		}

		if(norm_type == e_norm_l2)
		{
			int N= (_right_bound-_left_bound)/dx;
			norm = sqrt(norm) / N;
		}
		return norm;
	}


private:
	unsigned _size;		///< число узлов сетки
	Type _left_bound;	///< лева€ граница отрезка
	Type _right_bound;	///< права€ граница отрезка
	Type _step;			///< шаг сетки

	Type* _tab;			///< табул€ци€ функци€
	std::function<Type(Type)> _func; ///< интерполируема€ функци€
};


#endif //INTERPOLATION_H
