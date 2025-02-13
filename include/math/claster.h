#ifndef  CLASTER_H
#define CLASTER_H

#include <vector>
#include <list>
#include <numeric>
#include <functional>
#include <limits>
#include <math.h>

/*
* Класс ClasterEngine должен иметь метод: T Distance(const ObjType&) const
*/
#if __cplusplus < 202002L
#include <type_traits>
template<typename T>
using MetricSpaceObject = std::void_t<decltype(std::declval<T>().Distance(std::declval<T>()))>;

template<typename ObjType, typename T = double, typename = void>
class ClasterEngine;
#else
#include <concepts>
template<typename T>
concept MetricSpaceObject = requires(const T t) {    
    {(t.Distance(t)) <=> 0}; // Проверяем, что существует метод: Distance(T t) const 
};
#endif

#if (__cplusplus < 202002L)
template<typename ObjType, typename T>
class ClasterEngine<ObjType, T, MetricSpaceObject<ObjType>>
#else
template<MetricSpaceObject ObjType, typename T = double>
class ClasterEngine
#endif
{
private:
    int _number_of_clasters;
    std::list<std::pair<int, ObjType>> _objects; //пары "номер кластера"-объект    

public:
    
    ClasterEngine() :
        _number_of_clasters(0) {
    }

    inline void clear() noexcept
    {
        _objects.clear();
        _number_of_clasters = 0;
    }

    inline auto begin()
    {
        return _objects.begin();
    }
    inline auto end()
    {
        return _objects.end();
    }

    inline void Add(const ObjType& x)
    {
        _objects.push_back({ 0, x });
    }

    inline int size() const noexcept
    {
        return _objects.size();
    }

    inline int number_of_clasters() const noexcept
    {
        return _number_of_clasters;
    }

    //Type _threshold;            //допуск к удаленности элементов кластера от центра        
    void ThresholdAlgorithm(const T threshold)
    {
        if (_objects.empty())
        {
            return;
        }

        _number_of_clasters = 1;
        std::list<const ObjType*>claster_centers = { &_objects.front().second };

        auto it = _objects.begin();
        for (it++; it != _objects.end(); ++it)
        {
            int& claster_idx = it->first;
            const ObjType& obj = it->second;

            claster_idx = 0;
            T min_distance = std::numeric_limits<T>::max();
            int j = 0;
            for (const ObjType* center : claster_centers)
            {
                T dist = center->Distance(obj);
                if (dist < min_distance)
                {
                    min_distance = dist;
                    claster_idx = j;
                }
                ++j;
            }

            if (min_distance > threshold)
            {
                claster_idx = _number_of_clasters++;
                claster_centers.push_back(&obj);
            }
        }
    }
};


template<typename T>
class Point
{
public:
    Point(const std::vector<T>& a) :coord(a)
    {
    }

    T Distance(const Point<T>& x) const
    {
        return sqrt(std::inner_product(coord.begin(), coord.end(), x.coord.begin(), 0.0,
            std::plus(), [](T a, T b) {return (a - b) * (a - b); }));
    }

private:
    std::vector<T> coord;
};
#endif // ! CLASTER_H

