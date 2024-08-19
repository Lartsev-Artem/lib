#ifndef FILE_MODULE_H
#define FILE_MODULE_H

#include <string>

namespace io_api
{
    inline bool is_comments(const std::string& str)
    {
        return(str.substr(0, 2) == "//");
    }

    inline void erase_spaces(std::string& str)
    {
        int i = 0;
        while (isspace(str[i++])) {}
        str.erase(str.begin(), str.begin() + i - 1);
    }

    // str[out] - строка
    inline bool get_next_not_empty_line(std::ifstream& ifile, std::string& str)
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

    inline std::string get_file_name(const std::string& file)
    {
        const char* start = file.c_str();
        const char* end = start + file.length();

        while (*(end) != '.' && end != start)  --end;

        const char* str = end;
        while (*str != '/' && *str != '\\' && (start != str)) --str;

        if (start != str)(++str);

        return std::string(str, end);
    }

}

#endif // !FILE_MODULE_H