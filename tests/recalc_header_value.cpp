#include "io_api/header_rebuilder.h"
#include "io_api/file_module.h"

#include <iostream>
#include <fstream>

#include "calculator.h"

static int generate_test_file(const char* file)
{
    std::ofstream ofile(file);
    if (!ofile.is_open())
    {
        std::cout << "file:  " << file << " wasn't opened\n";
        return 1;
    }
    ofile << "#ifndef TEST_H\n";
    ofile << "#define TEST_H\n\n";
    ofile << "#if 1\n";
    ofile << "    constexpr int a = 8; // this- 2.079441541679836\n";
    ofile << "    constexpr int b = 3;   /// это1.09861228866811\n";
    ofile << "    constexpr int c = a / b; /*0.9808292530117262*/\n";
    ofile << "#else\n";
    ofile << "    constexpr int a = 1.5;\n";
    ofile << "    //constexpr double b = 2. / 3.;\n";
    ofile << "    constexpr double b = 1. / 3.;\n";
    ofile << "    constexpr double c = a * b;\n";
    ofile << "#endif\n\n";
    ofile << "#endif //TEST_H\n";
    ofile.close();
    return 0;
}
int main(int argc, char** argv)
{

    const std::string file_input = "test.h";
    const std::string output_dir = "";

    if (generate_test_file(file_input.c_str()))
    {
        return 1;
    }
   
    Calculator<io_api::header::value_type> math;
    io_api::header::parse_file(file_input, math);
    io_api::header::write_file(output_dir, io_api::get_file_name(file_input), math);

    return 0;
}