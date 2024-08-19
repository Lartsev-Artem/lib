#ifndef HEADER_REBUILDER_H
#define HEADER_REBUILDER_H

/*
@note Файл парсит файл с constexpr константами и формирует на его основе новый файл с вычисленными значениями f(x)
*/
/// \warning значение(int = 0) интерпретируется как внутрення ошибка и значение переменная не сохраняется


#include <string>
#include "calculator.h"

namespace io_api
{
	namespace header
	{
		typedef double value_type;
		void parse_file(const std::string& file_input, Calculator<value_type>& math);
		void write_file(const std::string& output_dir, const std::string& base_file, const Calculator<value_type>& math);
	}//header
}//namespace io_api

#endif // !HEADER_REBUILDER_H