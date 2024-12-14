#include "claster.h"

#include <iostream>

int main()
{        
    using Type = double;

    ClasterEngine<Point<Type>, Type> A;

    A.Add(Point<Type>({ 0,0,0 }));
            
    A.Add(Point<Type>({ 1, -1,0}));
    A.Add(Point<Type>({ 10,10 ,0}));
    A.Add(Point<Type>({ 1, -2,10}));    
    A.Add(Point<Type>({ 1,9 ,0}));

    A.Add(Point<Type>({ -1,-1 ,0 }));
    A.Add(Point<Type>({ 10,12,0 }));
    
    A.ThresholdAlgorithm(5);

    for (auto &el : A)
    {
        std::cout << el.first << ", ";
    }
    
    
	return 0;
}