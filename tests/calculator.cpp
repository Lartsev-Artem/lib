#include "calculator.h"
#include <iostream>
#include <conio.h>

int main()
{
	std::cout << "Choose type of calculator(0-int, 1-double):  ";
	unsigned type = 0xF;
	std::cin >> type;
	if (type > 1)
	{
		std::cout << "Error type!\n";
		return 1;
	}	
	std::cout << "\npress ESC to exit.\n";
	std::cout << "\press v to print vars list.\n\n";

	Calculator<int> icalc;
	Calculator<double> dcalc;
	std::string str;
	std::cout << ">>  ";
	do
	{
		if (kbhit())
		{
			char ch = getch();			

			if (ch == 27) //esc
			{
				break;
			}
			else if (ch == 13) //enter
			{				
				if (type == 0)
				{
					std::cout << "\n<<  " << icalc(str.c_str()) << std::endl;
				}
				else
				{
					std::cout << "\n<<  " << dcalc(str.c_str()) << std::endl;
				}
				str.clear();
				std::cout << ">>  ";
			}
			else if (ch == 32) //space
			{
				//do nothing
			}
			else if (ch == 'v'|| ch == 'V')
			{
				std::cout<<ch << "\n<<  Vars: \n";
				if (type == 0)
				{
					for (auto it = icalc.list_vars().begin; it != icalc.list_vars().end; it++)
					{
						std::cout << ">>  " << it->first << "= " << it->second << std::endl;
					}
				}
				else
				{
					for (auto it = dcalc.list_vars().begin; it != dcalc.list_vars().end; it++)
					{
						std::cout<<">>  " << it->first << "= " << it->second << std::endl;
					}
				}				
				std::cout << ">>  ";
			}
			else if ((ch >= '0' && ch <= '9') || ch == '(' || ch == ')'
				|| ch == '=' || ch == '*' || ch == '/' || ch == '+' || ch == '-' 
				|| ch == '^' || ch == '%' || ch == '!' ||
				(ch >= 'A' && ch <= 'Z') ||
				(ch >= 'a' && ch <= 'z')|| (ch =='.'))
			{
				std::cout << ch;
				str.push_back(ch);
			}
			else
			{
				std::cout << "\n<<  Undefined symbol!\n";
				str.clear();
				std::cout << ">>  ";
			}
		}		

	} while (1);

	return 0;
}