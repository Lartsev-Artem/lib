/**
 * @file com_port.h
 * @author Artem
 * @brief Обращение к последовательному порту. Чтение, запись
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef COM_PORT_H
#define COM_PORT_H

/*  Example
#include "com_port.h"
int main()
{
	comm_port::SerialPort<void> port(comm_port::e_rate_9600, 8, comm_port::e_no_parity, comm_port::e_ones_stopbit);
	port.comm_open("COM3", 100, 0);

	char word[] = "Hello world\r\n";
	port.comm_write(word, sizeof(word));

	printf("time0= %u\n", port.get_last_event_ms());
	port.comm_read(word, sizeof(word));
	printf("time1= %u\n", port.get_last_event_ms());

	port.comm_close();
	return 0;
}
*/

#include <stdio.h>
#include <windows.h>
#include <chrono>

#if 0
#define log_output(...) do{ printf(__VA_ARGS__);}while(0)
#else
#define log_output(...) do{}while(0)
#endif

namespace comm_port
{


	//! @brief доступные скорости передачи данных	 
	enum e_comm_rate_t
	{
		e_rate_110 = CBR_110,
		e_rate_300 = CBR_300,
		e_rate_600 = CBR_600,
		e_rate_1200 = CBR_1200,
		e_rate_2400 = CBR_2400,
		e_rate_4800 = CBR_4800,
		e_rate_9600 = CBR_9600,
		e_rate_14400 = CBR_14400,
		e_rate_19200 = CBR_19200,
		e_rate_38400 = CBR_38400,
		e_rate_56000 = CBR_56000,
		e_rate_57600 = CBR_57600,
		e_rate_115200 = CBR_115200,
		e_rate_128000 = CBR_128000,
		e_rate_256000 = CBR_256000,
	};

	//! @brief проверка четности
	enum e_parity_t
	{
		e_no_parity = NOPARITY,
		e_odd_parity = ODDPARITY,
		e_even_parity = EVENPARITY,
		e_mark_parity = MARKPARITY,
		e_space_parity = SPACEPARITY,
	};

	//! @brief число стоп-бит 
	enum e_stopbits_t
	{
		e_ones_stopbit = ONESTOPBIT,
		e_one5s_stopbits = ONE5STOPBITS,
		e_two_stopbits = TWOSTOPBITS,
	};

	template<class = void>
	class SerialPort
	{
	public:

		SerialPort(e_comm_rate_t rate = e_rate_9600, uint8_t bytesize = 8, e_parity_t parity = e_no_parity, e_stopbits_t stopbit = e_ones_stopbit) :
			_rate(rate),
			_bytesize(bytesize),
			_parity(parity),
			_stopbit(stopbit),
			_port(INVALID_HANDLE_VALUE)
		{
		}

		inline bool is_opened()
		{
			return (_port != INVALID_HANDLE_VALUE);
		}

		/**
		 * @brief Открыть ком-порт
		 *
		 * @param com_name - имя порта
		 * @param read_timeout  - максимальное время ожидание данных на чтение
		 * @param write_timeout - максимальное время ожидание данных на запись
		 * @return int <0 - ошибка
		 */
		int comm_open(const char* com_name, uint32_t read_timeout = 0, uint32_t write_timeout = 0)
		{
			if (is_opened())
			{
				comm_close();
			}

			_port = CreateFile(
				(LPCSTR)com_name,  // имя открываемого порта.
				GENERIC_WRITE | GENERIC_READ,  // порт открывается в режиме записи.
				0,  // коммуникационные порты нельзя делать разделяемыми, поэтому данный параметр должен быть равен 0.
				NULL,  // задает атрибуты защиты файла, при работе с портами должен быть NULL.
				OPEN_EXISTING,  // для работы с коммуникационными портами должно задаваться OPEN_EXISTING.
				0,//FILE_FLAG_OVERLAPPED,  //0, // нулевое значение используется при синхронной работе с портом.
				NULL);  // дескриптор файла-шаблона, при работе с портами всегда должен быть равен NULL.

			if (_port == INVALID_HANDLE_VALUE) {
				log_output("Error creating port!\n");
				return -1;
			}

			DCB dcb = { 0 };	// параметры последовательного порта.
			if (!GetCommState(_port, &dcb)) {
				log_output("Error get comm state DCB!\n");
				return -1;
			}

			// Структура DCB инициализирована, можно устанавливать свои значения:
			dcb.BaudRate = _rate;  // скорость передачи данных.
			dcb.ByteSize = _bytesize;  // количество информационных бит (от 4 до 8) в байте.
			dcb.Parity = _parity;  // способ контроля чётности.
			dcb.StopBits = _stopbit;  // количество стоповых бит, может принимать значения.

			if (!SetCommState(_port, &dcb)) {
				log_output("Error set comm state\n");
				return -1;
			}

			// установка таймаутов
			COMMTIMEOUTS tm;
			GetCommTimeouts(_port, &tm);
			tm.ReadIntervalTimeout = READ_INTERVAL_TIMEOUT;
			tm.ReadTotalTimeoutConstant = read_timeout;
			tm.ReadTotalTimeoutMultiplier = 0;
			tm.WriteTotalTimeoutConstant = write_timeout;
			tm.WriteTotalTimeoutMultiplier = 0;
			SetCommTimeouts(_port, &tm);

			PurgeComm(_port, PURGE_TXABORT | PURGE_TXCLEAR);  // прекращает все операции записи и очищает очередь передачи в драйвере.

			log_output("port opened\n");

			return 0;

		}

		//! @brief Закрытие порта				 
		void comm_close()
		{
			if (is_opened())
			{
				FlushFileBuffers(_port);
				PurgeComm(_port, PURGE_TXABORT | PURGE_RXABORT);  // прекращает все операции записи и очищает очередь приема в драйвере.
				CloseHandle(_port);
				_port = INVALID_HANDLE_VALUE;
				log_output("close port\n");
			}
		}

		/**
		 * @brief Чтение данных с порта
		 *
		 * @param data - массив для данных
		 * @param size_byte - максимальный размер массива (в байтах)
		 * @return реальное кол-во считанных байт
		 */
		int comm_read(void* data, uint32_t size_byte)
		{
			unsigned long reuslt = 0;

			/* Начинаем чтение данных */
			if (ReadFile(
				_port,			// дескриптор открытого коммуникационного порта.
				data,			// данные из этого буфера будут передаваться в порт.
				size_byte,			// число предназначенных к чтению байт.
				&reuslt,	// число фактически считанных байт.
				NULL))			// для синхронных операций данный параметр должен быть равным NULL.
			{
				log_output("comm_rcv[%d]\n", reuslt);

				if (reuslt)
				{
					set_time_event();
				}
			}
			else
			{
				log_output("error read_port\n");
			}
			return reuslt;
		}

		/**
		 * @brief Запись данных в порт
		 *
		 * @param data - массив для данных
		 * @param size_byte - максимальный размер массива (в байтах)
		 * @return реальное кол-во переданых байт
		 */
		int comm_write(void* data, uint32_t size_byte)
		{
			if (is_opened())
			{
				DWORD dwBytesWrite = size_byte;  // кол-во записанных байтов
				if (!WriteFile(_port, data, size_byte, &dwBytesWrite, NULL))
				{
					log_output("write error\n");
					return 0;
				}
				if (!FlushFileBuffers(_port))
				{
					log_output("write error: FlushFileBuffers\n");
					return 0;
				};

				log_output("comm_send[%d]\n", dwBytesWrite);
				set_time_event();
				return dwBytesWrite;
			}
			else
			{
				log_output("com port is not open!\n");
			}
			return 0;
		}

		//! @brief время с последней успешной операции чтения/записи
		inline uint32_t get_last_event_ms()
		{
			if (is_opened())
			{
				return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time_last_event).count();
			}
			return 0;
		}
	private:

		inline void set_time_event()
		{
			_time_last_event = std::chrono::system_clock::now();
		}

	private:

		enum
		{
			READ_INTERVAL_TIMEOUT = 5	///< время между символами на чтении
		};

		uint8_t _bytesize;		///< количество информационных бит (от 4 до 8) в байте.	
		uint8_t _parity;		///< способ контроля чётности.
		uint8_t _stopbit;		///< количество стоповых бит, может принимать значения.		
		uint32_t _rate;			///< скорость передачи данных.		

		std::chrono::system_clock::time_point _time_last_event;	///< время последней успешной операции чтения/записи
		HANDLE _port;			///< дескриптор порта.		
	};
};

#undef log_output
#endif //COM_PORT_H
