/**
 * @file ring_buffer.h
 * @author Artem
 * @brief Реализация кольцевого буфера
 * @version 0.1
 * @date 2024-08-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/* Example

#include "ring_buffer.h"
typedef uint8_t Type;
int main()
{
	Type data[10] = { 0,1,2,3,4,5,6,7,8,9 };

	ring_buffer::RingBuffer<Type>* buf = new ring_buffer::RingBuffer<Type>;
	buf->init(5);
	buf->print_hex("init: ");

	buf->put(-1);
	buf->put(-2);
	buf->put(data, 2);
	buf->print_hex("add -1, -2, 0, 1: ");
	buf->put(data+5, 1);
	buf->print_hex("add 5: ");

	Type a[5];
	buf->get(a); printf("get= %d\n", a[0]);
	buf->pop(a, 3); printf("pop= %d, %d ,%d\n", a[0], a[1], a[2]);

	buf->put(data + 6, 3);
	buf->print_hex("pop -1,-2,0. add 6,7,8: ");

	uint32_t n = buf->size(); printf("size= %d\n",n);
	try
	{
		buf->put(data, (5 - n) + 1);
	}
	catch (const ring_buffer::OverflowException& e)
	{
		std::cerr << e.what() << '\n';
	}

	delete buf;
	return 0;
}
*/

#include<stdint.h>
#include <memory>

#include <iostream>
#include <exception>

namespace ring_buffer
{
	// Define a new exception class
	class OverflowException : public std::exception {
	public:
		OverflowException(const char* msg) : _message(msg) {}
		const char* what() const throw() { return _message.c_str(); }
	private:
		std::string _message;
	};

	template<typename _T>
	class RingBuffer
	{
	public:

		RingBuffer() :m_capacity(0), m_size(0), w_ptr(0), r_ptr(0), m_buf(nullptr) {}
		~RingBuffer() { clear(); }

		/**
		 * @brief Инициализация буфера, Выделение памяти
		 *
		 * @param N размер буфера
		 */
		void init(uint32_t N)
		{
			if (m_capacity)
			{
				clear();
			}

			w_ptr = r_ptr = m_size = 0;
			m_capacity = N;
			m_buf = new _T[N];
			memset(m_buf, 0, N * sizeof(_T));
		}

		//! @brief очистка памяти буфера
		void clear()
		{
			if (m_buf != nullptr)
			{
				delete[] m_buf;
				m_buf = nullptr;
				m_capacity = w_ptr = r_ptr = m_size = 0;
			}
		}

		/**
		 * @brief Поместить в буфер новые данные
		 *
		 * @param[in] data данные
		 * @param[in] in_size размер входного массива
		 * @return int - код ошибки
		 */
		int put(const _T* data, uint32_t in_size)
		{
			if (in_size == 0)
			{
				return 0;
			}

			int32_t N = m_capacity - m_size;	//максиальный размер свободной части буфера

			if (in_size > N)
			{
				throw OverflowException("Buffer overflow!!!");
			}
			else
			{
				if (w_ptr + in_size <= m_capacity)
				{
					memcpy(m_buf + w_ptr, data, in_size * sizeof(_T));
				}
				else
				{
					uint32_t n = m_capacity - w_ptr;
					memcpy(m_buf + w_ptr, data, n * sizeof(_T));
					memcpy(m_buf, data + n, (in_size - n) * sizeof(_T));
				}
				w_ptr = (w_ptr + in_size) % m_capacity;
				m_size += in_size;
			}
			return 0;
		}

		/**
		 * @brief Поместить в буфер один элемент
		 * @param[in] val данные
		 */
		int put(const _T val)
		{
			if (m_capacity == m_size)
			{
				throw OverflowException("Buffer overflow!!!");
			}
			m_buf[w_ptr] = val;
			w_ptr = (w_ptr + 1) % m_capacity;
			m_size++;
			return 0;
		}

		/**
		 * @brief Забрать данные из буфера, без удаления их из буфера
		 * @note Если в буфере недостаточно элементов будет забрано макс. возможное кол-во
		 *
		 * @param[out] data выходной массив
		 * @param[in] out_size максимальное число забираемых эл-в
		 * @return число реально забранных элементов
		 */
		int get(_T* data, uint32_t out_size) const
		{
			if (m_size == 0)
			{
				return 0;
			}

			if (m_size < out_size)
			{
				out_size = m_size;
			}

			if (r_ptr + out_size <= m_capacity)
			{
				memcpy(data, m_buf + r_ptr, out_size * sizeof(_T));
			}
			else
			{
				int n = m_capacity - r_ptr;
				memcpy(data, m_buf + r_ptr, n * sizeof(_T));
				memcpy(data + n, m_buf, (out_size - n) * sizeof(_T));
			}
			return out_size;
		}

		/**
		 * @brief Забрать из буфера один элемент (без удаления)
		 * @param[out] val данные
		 * @return число забранных элементов
		 */
		int get(_T* val) const
		{
			if (m_size == 0)
			{
				return 0;
			}
			*val = m_buf[r_ptr];
			return 1;
		}

		/**
		 * @brief Забрать данные из буфера и удалить их из буфера
		 * @note Если в буфере недостаточно элементов будет забрано макс. возможное кол-во
		 *
		 * @param[out] data выходной массив
		 * @param[in] out_size максимальное число забираемых эл-в
		 * @return число реально забранных элементов
		 */
		int pop(_T* data, uint32_t out_size)
		{
			if (m_size == 0)
			{
				return 0;
			}

			if (m_size < out_size)
			{
				out_size = m_size;
			}

			if (r_ptr + out_size <= m_capacity)
			{
				memcpy(data, m_buf + r_ptr, out_size * sizeof(_T));
			}
			else
			{
				int n = m_capacity - r_ptr;
				memcpy(data, m_buf + r_ptr, n * sizeof(_T));
				memcpy(data + n, m_buf, (out_size - n) * sizeof(_T));
			}
			r_ptr = (r_ptr + out_size) % m_capacity;
			m_size -= out_size;
			return out_size;
		}

		/**
		 * @brief Забрать из буфера один элемент (и удалить из буфера)
		 * @param[out] val данные
		 * @return число забранных элементов
		 */
		int pop(const _T* val)
		{
			if (m_size == 0)
			{
				return 0;
			}

			*val = m_buf[r_ptr];
			r_ptr = (r_ptr + 1) % m_capacity;
			m_size--;
			return 1;
		}

		/**
		 * @brief Перенести данные из одного буфера в другой (с удалением из первого)
		 * @note Если размер источника меньше запрошенного размера, будет скопирован буфер целиком
		 *
		 * @param[inout] src входной буфер
		 * @param[in] size число элементов для копирования
		 * @return число скопированных элементов
		 */
		int copy(RingBuffer* src, uint32_t size)
		{
			if (!size)
			{
				return 0;
			}

			int N = src->size();
			if (size > N)
			{
				size = N;
			}

			int n = src->m_capacity - src->r_pt;
			this->put(src->m_buf + src->r_ptr, n);
			this->put(src->m_buf, size - n);
			src->erase(size);
			return size;
		}

		//! @brief удалить из буфера N элментов
		void erase(uint32_t N)
		{
			if (N > m_size)
			{
				N = m_size;
			}
			r_ptr = (r_ptr + N) % m_capacity;
			m_size -= N;
		}

		inline uint32_t size() const
		{
			return m_size;
		}

		// печать всего буфера
		void print_hex(const char* msg="") const
		{
			printf("%s", msg);						
			for (uint32_t i = 0; i < m_capacity * sizeof(_T); i++)
			{
				printf("0x%02x ", ((uint8_t*)m_buf)[i]);
			}
			printf("\n");
		}

	private:
		RingBuffer(const RingBuffer<_T>&); // No copy constructor

		_T* m_buf;				///< данные
		uint32_t m_size;		///< число эл-в в буфере
		uint32_t m_capacity;	///< общий размер выделенной памяти под буфер
		uint32_t r_ptr;			///< указатель на место чтения
		uint32_t w_ptr;			///< указатель на место записи
	};

} // namespace ring_buffer
#endif // RING_BUFFER_H
