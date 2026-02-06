#ifndef FILE_MODULE_H
#define FILE_MODULE_H

#include <string>
#include <fstream>
#include <ctime>
#include <stdint.h>

namespace io_api
{

bool is_comments(const std::string& str);
void erase_spaces(std::string& str);

bool get_next_not_empty_line(std::ifstream& ifile, std::string& str);
std::string get_file_name(const std::string& file);

int read_file(const char* file, uint32_t byte_size, void* data);
int read_file(const char* filename, int offset, uint32_t byte_size, void* buffer);
int write_file(const char* file, uint32_t byte_size, const void* data, bool app_md = false);
uint32_t get_file_size(const char* file);

#if defined(_WIN32)
std::time_t get_last_write_time(const std::string& filepath);
std::time_t get_file_creation_time(const std::string& filepath);
void remove_files_by_mask(const std::string& directory, const std::string& mask);
#endif
}// io_api

#endif // !FILE_MODULE_H
