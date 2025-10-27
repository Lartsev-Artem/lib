#ifndef WELZL_ALGORITHM_H
#define WELZL_ALGORITHM_H


#include <stdint.h>
#include <math.h>
#include <list>


class Welzl
{
public:    
    using type_welzl = float;
    static constexpr type_welzl epsilon = 1e-9f;

    struct Point
    {
        type_welzl x;
        type_welzl y;

        Point(type_welzl _x = 0, type_welzl _y = 0) : x(_x), y(_y) {}
        Point(const Point& p) : x(p.x), y(p.y) {}
    };

    struct Circle
    {
        type_welzl radius;
        Point center;
        Circle(const Point& c = Point(), type_welzl r = 0) : center(c), radius(r) {}
    };

    /**
     * @brief Построение описанной окружности вокруг облака точек
     *  рекурсивным методом Велцля
     * @param m_points
     * @param size
     *
     * @note Точки должны иметь случайный порядок для оптимальной работы алгоритма
     */
    void welzl_proc(const Point* m_points, uint32_t size);    
    Circle get_circle() { return m_circle; }

private:

    ///\brief проверка лежит ли точка внутри окружности
    bool is_inside(const Circle& c, const Point& p);

    ///\brief расстояние между двумя точками
    type_welzl distance(const Point& a, const Point& b);

    ///\brief Описанная окружность около отрезка
    Circle circle_from_two(const Point& a, const Point& b);

    ///\brief Описанная окружность около треугольника
    Circle circle_from_three(const Point& a, const Point& b, const Point& c);

private:
    Circle m_circle;
    std::list<uint32_t> m_disk_point_id;

};

/*
  //Test  Random points cloud. + Matlab code
#include <iostream>
#include <vector>
int main() {

    const int N = 100;
    std::vector<Welzl::Point> points(N);

    std::cout << "points = [";
    for (size_t i = 0; i < N; i++)
    {
        points[i].x = ((double)(rand() % 1000))/100;
        points[i].y = ((double)(rand() % 1000))/100;
        std::cout << points[i].x << "," << points[i].y << ";";
    }
    std::cout << "];\n";

    Welzl welzl;
    welzl.welzl_proc(points.data(), points.size());
    Welzl::Circle result = welzl.get_circle();
    std::cout << "Center= [" << result.center.x << ", " << result.center.y << "];\n";
    std::cout << "Radius= [" << result.radius << "];\n";

    std::cout << "\n\nplot(points(:,1), points(:,2), 'b*')\n"
        "hold on\n"
        "t = 0:0.01 : 2 * pi;\n"
        "x = Radius.*cos(t) + Center(1);\n"
        "y = Radius.*sin(t) + Center(2);\n"
        "plot(x, y)\n";

    return 0;
}
*/



#endif //WELZL_ALGORITHM_H
