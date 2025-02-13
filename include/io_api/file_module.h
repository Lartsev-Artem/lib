#ifndef FILE_MODULE_H
#define FILE_MODULE_H

#include <string>
#include <fstream>

namespace io_api
{

bool is_comments(const std::string& str);
void erase_spaces(std::string& str);

bool get_next_not_empty_line(std::ifstream& ifile, std::string& str);
std::string get_file_name(const std::string& file);

int read_file(const char* file, uint32_t byte_size, void* data);
int write_file(const char* file, uint32_t byte_size, const void* data);
uint32_t get_file_size(const char* file);

}// io_api

#endif // !FILE_MODULE_H
