/**
 * @file convex_hull_2d.h
 * @author Artem
 * @brief Обертка для алгоритмов построения выпуклой 2д оболочки
 * @version 0.1
 * @date 2025-02-13
 *
 * @note проверяно и оптимизировано только ядро graham. Jarvis в некоторых случаях выдает пустую оболочку, Chan- не оптимизирован
 * @copyright Copyright (c) 2025
 *
 */
#ifndef CONVEX_HULL_2D_H
#define CONVEX_HULL_2D_H

#include "core_graham.h"
#include "core_jarvis.h"
#include "core_chan.h"

#include <string>

#include <fstream>
#include <iostream>

#include <ctime>  // RandCoord
#include <random> // RandCoord

template<typename TYPE = double> class ConvexHull2D
{
public:

	ConvexHull2D(): _core(nullptr)
	{
	}

	~ConvexHull2D()
	{
		if (_core)
		{
			delete _core;
		}
	}

	enum type_core_t
	{
		e_graham = 0,
		e_jarvis = 1,
		e_chan = 2
	};
	void SetCore(type_core_t type)
	{
		if (_core)
		{
			delete _core;
			_core = nullptr;
		}

		switch (type)
		{
			case e_graham:
				_core = new  GrahamCore<TYPE>;
				break;
			case e_jarvis:
				_core = new  JarvisCore<TYPE>;
				break;
			case e_chan:
				_core = new  ChanCore<TYPE>;
			break;
		}

	}

	void Init( const std::vector<Point<TYPE>>& data)
	{
		_data = std::move(data);
	}

	void Run()
	{
		std::vector<Point<TYPE>> tmp(_data.begin(), _data.end());
		_core->Run(tmp);
	}

	const std::vector<Point<TYPE>>& GetData() const
	{
		return _data;
	}


	int ReadFromFile(std::string nameFile)
	{
		std::ifstream inf(nameFile);

		if (!inf)
		{
			std::cout << "No opened" << std::endl;
			return 1;
		}

		int n = 0;

		if (!(inf >> n) || n <= 0)
		{
			std::cout << "The first element is invalid" << std::endl;
			inf.close();
			return 2;
		}

		_data.reserve(n);
		Point<TYPE> curPoint;

		if (inf >> curPoint)                 // считываем первую точку
		{
			_data.push_back(curPoint);    //добавляем в массив точек
		}

		while (inf >> curPoint)           // пока чтение из файла без ошибок(или до конца)
		{
			_data.push_back(curPoint);

			if (_data[0] > curPoint)      // если текущая точка меньше начальной
			{
				std::swap(_data[0], _data.back());
			}
		}

		inf.close();
		return 0;
	}

	int OutFile(const std::string name)
	{
		std::ofstream ofs(name);

		if (!ofs)
		{
			std::cout << "Opening error file (list)" << std::endl;
			return 1;
		}


		ofs << _core->get_hull();
		ofs.close();
		return 0;
	}// вывод списка


	// генератор координат
	//колв-во точек, радиус точек
	int RandCoord(const int n, const int range)
	{
		if (n <= 0)
		{
			return 1;
		}

		_data.resize(n);
		std::mt19937 gen(time(0));
		std::uniform_real_distribution<> urd(-range, range);

		for (int i = 0; i < n; ++i)
		{
			_data[i] = Point<TYPE>((TYPE)urd(gen), (TYPE)urd(gen));
		}

		return 0;
	};

	void Show()
	{
		std::cout << _data << std::endl;
	}

private:
	HullCore<TYPE>* _core;
	std::vector<Point<TYPE>> _data;
};

#endif
