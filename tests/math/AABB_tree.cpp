#include "AABB_tree.h"
#include <iostream>

#include <chrono>
namespace ch = std::chrono;

/*
Building: 6648404 us
Finding: 161 us
Number of the intersecting cell: 474
*/
int main()
{

    constexpr int size_grid_x = 100;
    constexpr int size_grid_y = 100;
    constexpr int size_grid_z = 100;

    std::vector<AABB_tree::Box> bbox(size_grid_x* size_grid_y* size_grid_z);

    constexpr double dx = 0.1;
    constexpr double dy = 0.1;
    constexpr double dz = 0.1;
    for (size_t i = 0; i < size_grid_x; i++)
    {
        for (size_t j = 0; j < size_grid_y; j++)
        {
            for (size_t k = 0; k < size_grid_z; k++)
            {
                size_t idx = i * (size_grid_y * size_grid_z) + j * size_grid_z + k;
                bbox[idx] = AABB_tree::Box(idx, 
                    Vector3d(dx * i, dy * j, dz * k), 
                    Vector3d(dx * (i + 1), dy * (j + 1), dz * (k + 1))
                );                
            }
        }
    }
    
    AABB_tree tree(Vector3i(8,8,8));

    auto t = ch::system_clock::now();
    tree.Init(bbox.begin(), bbox.end());
    std::cout << "Building: " << ch::duration_cast<ch::microseconds>(ch::system_clock::now() - t).count() << " us\n";

    Vector3d left(dx / 2 * (size_grid_x / 2), dy / 2 * (size_grid_y / 2), dz / 2 * (size_grid_z / 2));
    AABB_tree::Box elemnt(0, left, left + 0.5 * Vector3d(dx, dy, dz));    

    std::list<const std::vector<int>*> ret;

    t = ch::system_clock::now();
    tree.Find(elemnt, ret);
    std::cout << "Finding: " << ch::duration_cast<ch::microseconds>(ch::system_clock::now() - t).count() << " us\n";

    
    std::cout << std::endl;
    size_t number_of_cells = 0;
    for (auto& ls : ret)
    {
        number_of_cells += ls->size();        
    }
    std::cout << "Number of the intersecting cell: " << number_of_cells;
    std::cout << std::endl;

    return 0;
}