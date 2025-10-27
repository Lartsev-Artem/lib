#ifndef SYMBOL_VECTOR_H
#define SYMBOL_VECTOR_H

#include <stdint.h>
#include <memory.h>

namespace galois
{
template<typename T>
struct symbol_vector
{
private:
	void operator = (const  symbol_vector& v);
	symbol_vector(const symbol_vector& v);

public:
	symbol_vector(): _data(nullptr), _size(0) {}
	symbol_vector(uint32_t N) : _size(N)
	{
		_data = new T[N];
	}

	~symbol_vector()
	{
		if (_data)
		{
			delete _data;
			_data = nullptr;
		}
	}

	void move(symbol_vector& v)
	{
		if (_data)
		{
			delete _data;
		}

		_data = v._data;
		_size = v._size;

		v._data = nullptr;
		v._size = 0;
	}

	const T* begin() const
	{
		return  _data;
	}

	const T* end() const
	{
		return  _data + _size;
	}

	void resize(uint32_t N, const T& x = 0)
	{
		if (!N)
		{
			_size = 0;
			delete _data;
			_data = nullptr;
			return;
		}

		if (!_size)
		{
			_data = new T[N];
		}
		else
		{
			if (_size < N)
			{
				T* memory  = new T[N];
				memcpy(memory, _data, _size * sizeof(T));
				delete _data;
				_data = memory;
			}
		}

		for (uint32_t i = _size; i < N; ++i)
		{
			_data[i] = x;
		}

		_size = N;
	}

	T* data()
	{
		return _data;
	}

	const T* data() const
	{
		return _data;
	}

	uint32_t size() const
	{
		return _size;
	}

	const T& operator[](const uint32_t i) const
	{
		return _data[i];
	}
	T& operator[](const uint32_t i)
	{
		return _data[i];
	}

private:
	T* _data;
	uint32_t _size;
};

}//namespace galois


#endif ///SYMBOL_VECTOR_H
