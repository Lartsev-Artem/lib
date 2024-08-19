#include "header_rebuilder.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

#include "file_module.h"

#define USE_ONLY_DOUBLE_TYPE      // все типы приводить к double
//#define LOG_ENABLE              // включить лог

#define MATH_FUNC log       // используемая математическая функция
#define PREFIX_VAR "log_"   // префикс у новых переменных


#ifndef USE_ONLY_DOUBLE_TYPE
#include <map>
std::map<std::string, std::string> type_name_list;
#endif

static bool skip_section(std::ifstream& ifile, std::string& str)
{
    static bool active_section = false;
    bool is_end = ifile.eof();

    if (str.find("#endif") != std::string::npos) {         
        getline(ifile, str);
        return is_end;
    }

    if (str.find("#if 0") != std::string::npos)
    {
        active_section = false;
        do
        {
            getline(ifile, str);
        } while (str.find("#else") == std::string::npos);
        getline(ifile, str);
    }
    if (str.find("#if 1") != std::string::npos)
    {
        active_section = true;
        getline(ifile, str);
    }

    if (active_section && str.find("#else") != std::string::npos)
    {
        do
        {
            getline(ifile, str);
        } while (str.find("#endif") == std::string::npos);
        is_end = ifile.eof();
        getline(ifile, str);
        active_section = false;
    }

    return is_end;
}

void io_api::header::parse_file(const std::string& file_input, Calculator<value_type>& math)
{
    std::ifstream ifile;
    ifile.open(file_input);
    if (!ifile.is_open())
    {
        std::cout << "file: " << file_input << " not opend\n";
        exit(1);
    }
    else
    {
        std::cout << "file: " << file_input << " has been opened!\n";
    }

    bool active_section = false;
    while (!ifile.eof())
    {
        std::string str;        

        if (io_api::get_next_not_empty_line(ifile, str))
        {
            break;
        }

        io_api::erase_spaces(str);

        if (is_comments(str))
        {
            continue;
        }

        if (skip_section(ifile, str))
        {
            break;
        }

        const std::string cexpr = "constexpr";
        size_t pos_expr = str.find(cexpr);
        if (pos_expr == std::string::npos) {
            continue;
        }
        pos_expr += cexpr.length();

        std::string sub_str = str.substr(pos_expr, str.find(";") - pos_expr);

        io_api::erase_spaces(sub_str);

        size_t i = 0;
        while (sub_str[i++] != ' ') {}
        std::string type = sub_str.substr(0, i - 1);
        sub_str.erase(sub_str.begin(), sub_str.begin() + i);

#ifndef USE_ONLY_DOUBLE_TYPE
        io_api::erase_spaces(sub_str);

        i = 0;
        while (sub_str[i++] != ' ') {}
        std::string name = sub_str.substr(0, i - 1);

        type_name_list.insert(std::make_pair(name, type));
#endif

        io_api::header::value_type n = math(sub_str.c_str());
        if (!n && math.was_error())
            std::cout << "Error while counting:\r\n\t" << math.error_message();
        else
        {
            //std::cout << name << "= " << n;
        }
        //std::cout << "\r\n>";

    }
    ifile.close();
}

void io_api::header::write_file(const std::string& output_dir, const std::string& base_file, const Calculator<value_type>& math)
{
    std::string name_f = PREFIX_VAR + base_file;
    std::string file_out = output_dir + name_f + ".h";
    std::ofstream ofile(file_out);
    if (!ofile.is_open())
    {
        std::cout << "file: " << output_dir + name_f << " not opend\n";
        exit(1);
    }
    
    std::transform(name_f.begin(), name_f.end(), name_f.begin(), [](unsigned char c) { return std::toupper(c); });


    ofile << "#ifndef " << name_f << "_H\n";
    ofile << "#define " << name_f << "_H\n";
    
    
    const Calculator<io_api::header::value_type>::itr_range rng = math.list_vars();    
    std::map<const char*, 
        io_api::header::value_type, 
        Calculator<io_api::header::value_type>::compcl>::const_iterator itr;

    for (itr = rng.begin; itr != rng.end; itr++)
    {
#ifndef USE_ONLY_DOUBLE_TYPE        
        ofile << std::setprecision(16) << "constexpr " << type_name_list.find(itr->first)->second<<" " << PREFIX_VAR << itr->first << " = " << (Type)MATH_FUNC(itr->second) << ";\n";
#else
        ofile << std::setprecision(16) << "constexpr double " << PREFIX_VAR << itr->first << " = " << MATH_FUNC(itr->second) << ";\n";
#endif
#ifdef LOG_ENABLE
        std::cout << itr->first << " = " << PREFIX_VAR << "(" << itr->second << ") -> " << (Type)MATH_FUNC(itr->second) << "\n";
#endif
    }

    ofile << "#endif //! " << name_f << "_H\n";
    ofile.close();

    std::cout << "file: " << file_out << " has been created!\n";
}
