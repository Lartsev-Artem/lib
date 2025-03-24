  //Test  Random points cloud. + Matlab code

#include "welzl_algorithm.h"

#include <iostream>
#include <vector>
int main() {

    const int N = 100;
    std::vector<Welzl::Point> points(N);

    std::cout << "points = [";
    for (size_t i = 0; i < N; i++)
    {
        points[i].x = ((double)(rand() % 10000))/100;
        points[i].y = ((double)(rand() % 10000))/100;
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