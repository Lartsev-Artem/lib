#include <iostream>

#include "annealing.h"
#include "queen_problem.h"

int main() {
	
	Annealing test; 

	Board sol;
	if (test.Run((Entity*) & sol))
	{
		sol.print_result();
	}
	else
	{
		std::cout << "solution not found!!!\n";
	}
	
	return 0;
}