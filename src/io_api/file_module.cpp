#include "file_module.h"


bool io_api::is_comments(const std::string& str)
{
	return(str.substr(0, 2) == "//");
}

void io_api::erase_spaces(std::string& str)
{
	unsigned int i = 0;
	while (isspace(str[i++])) {}
	str.erase(str.begin(), str.begin() + (int)(i - 1));
}

// str[out] - ������
bool io_api::get_next_not_empty_line(std::ifstream& ifile, std::string& str)
{
	str = "";
	bool is_end = false;
	while (str == "" && !is_end)
	{
		std::getline(ifile, str);
		is_end = ifile.eof();
	}
	return is_end;
}

std::string io_api::get_file_name(const std::string& file)
{
	const char* start = file.c_str();
	const char* end = start + file.length();

	while (*(end) != '.' && end != start)  --end;

	const char* str = end;
	while (*str != '/' && *str != '\\' && (start != str)) --str;

	if (start != str)(++str);

	return std::string(str, end);
}

int io_api::read_file(const char* file, uint32_t byte_size, void* data)
{
	FILE* f;
	f = fopen(file, "rb");

	if (!f)
	{
		printf("%s not opened\n", file);
		return 1;
	}

	fread(data, 1, byte_size, f);
	fclose(f);
	return 0;
}

int io_api::write_file(const char* file, uint32_t byte_size, const void* data)
{
	FILE* f;
	f = fopen(file, "wb");

	if (!f)
	{
		printf("%s not opened\n", file);
		return 1;
	}

	fwrite(data, 1, byte_size, f);
	fclose(f);
	return 0;
}

uint32_t io_api::get_file_size(const char* file)
{
	int64_t size = 0;

	FILE* f = fopen(file, "rb");

	if (!f)
	{
		printf("Opening file error");
		return 0;
	}
	if (fseek(f, 0, SEEK_END) == 0)
	{
		size = ftell(f);
	}
	if (size < 0)
	{
		printf("ftell() error.");
		fclose(f);
		return 0;
	}
	if (!size)
	{
		printf("File is empty");
	}
	if (size > INT32_MAX)
	{
		printf("Need less file size: < %db", INT32_MAX);
	}

	fclose(f);
	//printf("size=%d B\n", size);
	return (uint32_t)size;
}
