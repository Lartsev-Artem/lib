#include "file_module.h"
#include <iostream>

bool io_api::is_comments(const std::string& str)
{
	return (str.substr(0, 2) == "//");
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

	while (*(end) != '.' && end != start)
	{
		--end;
	}

	const char* str = end;

	while (*str != '/' && *str != '\\' && (start != str))
	{
		--str;
	}

	if (start != str)
	{
		(++str);
	}

	return std::string(str, end);
}

int io_api::read_file(const char* file, uint32_t byte_size, void* data)
{
	FILE* f;
	f = fopen(file, "rb");

	if (!f)
	{
		printf("%s not opened\n", file);
		return -1;
	}

	int read_bytes = fread(data, 1, byte_size, f);
	fclose(f);
	return read_bytes;
}

int io_api::read_file(const char* filename, int offset, uint32_t byte_size, void* buffer)
{
	if (!filename || !buffer || byte_size == 0)
	{
		return -1;
	}

	FILE* file = fopen(filename, "rb");

	if (!file)
	{
		printf("%s not opened\n", file);
		return -1;
	}

	// Проверяем размер файла
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);

	if (offset >= file_size)
	{
		fclose(file);
		printf("(offset[%ld] >= file_size[%ld]\n", offset, file_size);
		return -1;
	}

	if (offset + byte_size > file_size)
	{
		byte_size = file_size - offset;
	}

	// Позиционируем и читаем
	if (fseek(file, offset, SEEK_SET) != 0)
	{
		printf("Error fseek\n");
		fclose(file);
		return -1;
	}

	size_t bytes_read = fread(buffer, 1, byte_size, file);
	fclose(file);

	if (bytes_read != byte_size && ferror(file))
	{
		printf("Error reading\n");
		return -1;
	}

	return bytes_read;
}

int io_api::write_file(const char* file, uint32_t byte_size, const void* data, bool app_md )
{
	FILE* f;

	if (app_md)
	{
		f = fopen(file, "ab");
	}
	else
	{
		f = fopen(file, "wb");
	}

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
		printf("Opening error: %s\n", file);
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

#if defined(_WIN32)
#include <windows.h>
std::time_t io_api::get_last_write_time(const std::string& filepath)
{
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;

	if (!GetFileAttributesExA(filepath.c_str(),
							  GetFileExInfoStandard,
							  &fileInfo))
	{
		return 0;
	}

	// Преобразование FILETIME в time_t
	ULARGE_INTEGER ull;
	ull.LowPart = fileInfo.ftLastWriteTime.dwLowDateTime;
	ull.HighPart = fileInfo.ftLastWriteTime.dwHighDateTime;

	// FILETIME отсчитывается от 1601-01-01, нужно преобразовать
	// в секунды от 1970-01-01 (Unix epoch)
	const ULONGLONG FILETIME_to_UNIX_epoch = 116444736000000000ULL;
	ull.QuadPart -= FILETIME_to_UNIX_epoch;
	ull.QuadPart /= 10000000ULL; // преобразуем в секунды

	return static_cast<std::time_t>(ull.QuadPart);
}

std::time_t io_api::get_file_creation_time(const std::string& filepath)
{
	// Открываем файл
	HANDLE hFile = CreateFileA(
					   filepath.c_str(),
					   GENERIC_READ,
					   FILE_SHARE_READ,
					   NULL,
					   OPEN_EXISTING,
					   FILE_ATTRIBUTE_NORMAL,
					   NULL
				   );

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cerr << "File not " << filepath << " opened" << std::endl;
		return 0;
	}

	// Получаем информацию о времени
	FILETIME ftCreate, ftAccess, ftWrite;

	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		std::cerr << "Error: GetFileTime" << std::endl;
		CloseHandle(hFile);
		return 0;
	}

	CloseHandle(hFile);

	// Преобразуем FILETIME в SYSTEMTIME
	SYSTEMTIME stCreate, st;

	FileTimeToSystemTime(&ftCreate, &stCreate);
	SystemTimeToTzSpecificLocalTime(NULL, &stCreate, &st);

	struct tm tm_time = {0};
	// Заполняем структуру tm
	tm_time.tm_year = st.wYear - 1900;  // Год с 1900
	tm_time.tm_mon = st.wMonth - 1;     // Месяц 0-11
	tm_time.tm_mday = st.wDay;
	tm_time.tm_hour = st.wHour;
	tm_time.tm_min = st.wMinute;
	tm_time.tm_sec = st.wSecond;
	tm_time.tm_isdst = -1;  // Автоматическое определение летнего времени
	return mktime(&tm_time);
}

void io_api::remove_files_by_mask(const std::string& directory, const std::string& mask)
{
	std::string searchPath = directory + "\\" + mask;

	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		// Пропускаем директории
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::string filePath = directory + "\\" + std::string(findData.cFileName);

			if (DeleteFileA(filePath.c_str()))
			{
				std::cout << "Deleted: " << filePath << std::endl;
			}
			else
			{
				std::cerr << "Failed to delete: " << filePath << std::endl;
			}
		}
	}
	while (FindNextFileA(hFind, &findData));

	FindClose(hFind);
}
#endif //