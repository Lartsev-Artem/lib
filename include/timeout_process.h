/**
 * @file timeout_process.h
 * @author Artem
 * @brief Последовательное выполнение набора асинхрнонных процессов по таймауту
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 */
/*
Example
#include "timeout_process.h"
int main()
{
	auto proc0{ []() { printf("proc 0\n"); } };
	auto proc1{ []() { printf("proc 1\n"); throw  timeout_proc::TimeOutExсeption("Test exception"); } };
	auto proc2{ []() { printf("proc 2\n"); } };

	timeout_proc::TimeOutProcess<3>* t = new timeout_proc::TimeOutProcess<3>;

	t->start_process(0, 100, proc0, timeout_proc::e_timeout_unlimit);
	t->start_process(1, 250, proc1, timeout_proc::e_timeout_unlimit);
	t->start_process(2, 0, proc2, timeout_proc::e_timeout_oneshot);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	delete t;

	return 0;
}
*/

#ifndef TIMEOUT_PROCESS_H
#define TIMEOUT_PROCESS_H

#include <chrono>
#include <functional>
#include <mutex>
#include <thread>

#include <iostream>
#include <exception>

namespace timeout_proc
{
	/// \brief тип процесса
	enum e_type_timeout_t
	{
		e_timeout_oneshot = 0,	///< однократное выполнение с указанной задержкой
		e_timeout_unlimit,		///< бесконечное выполнение
	};

	/// \brief коды ошибок	
	enum e_code_error
	{
		e_success = 0,	///< 
		e_err_overflow,		///< превышено максимальное число процессов
		e_err_id_exist,		///< идентификатор процееса уже используется
	};

	/**
	 * @brief Класс организует вызов функций с заданой периодичностью
		Реализуется блокирующее выполнение
		т.е. одновременно может быть выполнена только одна функция переданная в очередь,
		но они будут выполнены независимо от управляющего процесса (параллельно с main())
	 *
	 * @tparam _Size максимальное число процессов
	 */
	 //template<int _Size>
	template<int _Size>
	class TimeOutProcess {
	public:

		/**
		 * @brief Запустить процесс

		 * @param[in] id - уникальный номер процесса
		 * @param[in] timeout_ms - интервал исполнения
		 * @param[in] callback - исполняемая функция
		 * @param[in] type - тип таймаута
		 * @return int - ::e_code_error
		 *
		 * \note Включает отлавливатель исключений БЕЗ ОБРАБОТКИ.
		 */
		int start_process(int id, uint32_t timeout_ms, std::function<void()> callback, const e_type_timeout_t type = e_timeout_unlimit)
		{
			if (id >= _Size)
			{
				return e_err_overflow;
			}

			if (_proc_state[id])
			{
				return e_err_id_exist;
			}

			_proc_state[id] = e_state_running;
			std::thread([=]()
				{
					do
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
						std::lock_guard<std::mutex> lock(m_changing_mutex);
						{
							try
							{
								callback();
							}
							catch (const TimeOutExсeption& e)
							{
								/// \warning здесь не определным образом снимается блокировка мьютекса
								_proc_state[id] = e_state_interrupted;
								std::cerr << "Error!!!  Procces " << id << " was interrupted" << std::endl;
								std::cerr << "My exception caught: " << e.what() << '\n';
							}
							catch (...)
							{
								_proc_state[id] = e_state_interrupted;
								std::cerr << "Error!!!  Procces " << id << " was interrupted" << std::endl;
								std::cerr << "Undefined exception!!!" << std::endl;
								std::rethrow_exception(std::current_exception());
							}
						}

					} while ((_proc_state[id] == e_state_running) && (type == e_timeout_unlimit));

					_proc_state[id] = e_state_completed;
				}).detach();

				return e_success;
		}

		/**
		 * @brief Остановить запущенный процесс
		 *
		 * @param[in] id - номер процесса для остановки
		 */
		void stop_process(int id)
		{
			if (id >= _Size)
			{
				return;
			}

			if (_proc_state[id] == e_state_running)
			{
				_proc_state[id] = e_state_interrupted;
				while (_proc_state[id] != e_state_completed);
				_proc_state[id] = e_state_idle;
			}
		}

		TimeOutProcess()
		{
			for (int i = 0; i < _Size; i++)
			{
				_proc_state[i] = e_state_idle;
			}
		}

		~TimeOutProcess()
		{
			for (int i = 0; i < _Size; i++)
			{
				stop_process(i);
			}
		}

	private:

		/// \brief код состояния процесса
		enum
		{
			e_state_idle,			///< процесс не занят
			e_state_running,		///< процесс запущен
			e_state_interrupted,	///< процесс прерван
			e_state_completed		///< процесс завершен
		};
		std::atomic<uint8_t> _proc_state[_Size];
		std::mutex m_changing_mutex;
	};


	// Define a new exception class
	class TimeOutExсeption : public std::exception {
	private:
		std::string _message;

	public:
		TimeOutExсeption(const char* msg) : _message(msg) {}

		// Override the what() method to return our message 
		const char* what() const throw()
		{
			return _message.c_str();
		}
	};
}//namespace TimeOut

#endif //TIMEOUT_PROCESS_H