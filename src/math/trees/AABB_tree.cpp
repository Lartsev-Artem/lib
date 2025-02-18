#include "AABB_tree.h"

#include <algorithm>
#include <execution>
#include <iostream>



void AABB_tree::Init(const std::vector<Box>::iterator& begin, const std::vector<Box>::iterator& end)
{    
    auto find_max = [](const std::vector<Box>::iterator& begin, const std::vector<Box>::iterator& end, const int idx) {{
        Box maxX = *std::max_element(begin, end,  [idx](const Box& a, Box& b)
            {return (a._max[idx]-a._min[idx]) < (b._max[idx] - b._min[idx]); });
        return maxX._max[idx] - maxX._min[idx];
        }};
            
    for (int i = 0; i < _spatial_scale; i++)
    {
        _max_elem[i] = find_max(begin, end, i);
    }        
      
    sort_domain(begin, end, 0);
    add_next_lvl(begin, end, 0);
}

int AABB_tree::Find(const Box& A, std::list<const std::vector<int>*>& ret) const
{
    int flag = find_next(A, 0, ret);
    if (flag)
    {
        std::cout << "Error finding domain: " << flag << "\n";

        ret.clear(); //на всякий случай стираем все, что успели найти. т.к. результат не полный
    }
    return flag;
}


void AABB_tree::sort_domain(const std::vector<Box>::iterator& begin, const std::vector<Box>::iterator& end, int idx)
{
    //сортировка по максимальной границе
    std::sort(std::execution::par, begin, end, [idx](const Box& a, const Box& b) { return a._max[idx] < b._max[idx]; });
}

void AABB_tree::add_next_lvl(const std::vector<Box>::iterator& begin, const std::vector<Box>::iterator& end, int spatial_level)
{    
    //int idx = spatial_idx;
    auto pos = std::min_element(begin, end, [spatial_level](Box& a, Box& b) { return a._min[spatial_level] < b._min[spatial_level]; });
        
    box._min[spatial_level] = pos->_min[spatial_level];
    box._max[spatial_level] = (end - 1)->_max[spatial_level]; //это упорядоченная величина   
    

    int n = end - begin;

    if ((box._max[spatial_level] - box._min[spatial_level]) < _max_elem[spatial_level] || n < _min_count[spatial_level])
    {
        if (spatial_level + 1 == _spatial_scale)
        {
            static int lvl = 0;           
            //std::cout << lvl << ": " << box._min[spatial_level] << " - " << box._max[spatial_level] << ": ";
           
            ids.resize(n);
            int i = 0;
            for (auto it = begin; it < end; it++, i++)
            {
                ids[i] = it->_id;
                //std::cout << it->id << ",";
            }
            //std::cout << "\n";
            return;
        }
        else
        {            
            NextDim = new AABB_tree;
            sort_domain(begin, end, spatial_level + 1);
            NextDim->add_next_lvl(begin, end, spatial_level + 1);
            return;
            //next level
        }
    }

    left = new AABB_tree(_min_count, _max_elem);
    left->add_next_lvl(begin, begin + n / 2, spatial_level);

    right = new AABB_tree(_min_count, _max_elem);
    right->add_next_lvl(begin + n / 2, end, spatial_level);
}


int AABB_tree::find_next(const Box& A, int spatial_level, std::list<const std::vector<int>*>& ret) const
{
    if (!left || !right)
    {
        if (NextDim)
        {
            NextDim->find_next(A, spatial_level + 1, ret); //спускаемся дальше            
        }
        else
        {            
            //std::cout << "level add: " << level << "\n";
            ret.push_back(&ids);//мы внизу            
        }        
        return 0;
    }

    bool flag = false;
    if (A._max[spatial_level] <= left->box._max[spatial_level] || A._min[spatial_level] <= left->box._max[spatial_level])
    {
        left->find_next(A, spatial_level, ret);
        flag = true;
    }

    if (A._min[spatial_level] >= right->box._min[spatial_level] || A._max[spatial_level] >= right->box._min[spatial_level])
    {
        right->find_next(A, spatial_level, ret);
        flag = true;
    }

    if (!flag)
    {
        return 1;        
    }

    return 0;
}
