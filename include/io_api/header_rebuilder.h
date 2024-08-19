#ifndef HEADER_REBUILDER_H
#define HEADER_REBUILDER_H

/*
@note ���� ������ ���� � constexpr ����������� � ��������� �� ��� ������ ����� ���� � ������������ ���������� f(x)
*/
/// \warning ��������(int = 0) ���������������� ��� ��������� ������ � �������� ���������� �� �����������


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