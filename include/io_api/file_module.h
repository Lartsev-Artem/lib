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

    // str[out] - ˜˜˜˜˜˜
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

    inline int read_file(const char* file, uint32_t byte_size, void* data)
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

    inline int write_file(const char* file, uint32_t byte_size, const void* data)
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

    uint32_t get_file_size(const char* file)
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
}

#endif // !FILE_MODULE_H