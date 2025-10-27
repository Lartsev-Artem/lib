#include "welzl_algorithm.h"

void Welzl::welzl_proc(const Point* m_points, uint32_t size)
{
	if (m_disk_point_id.size() == 3)
	{
		auto ptr = m_disk_point_id.begin();
		uint32_t id[3];
		id[0] = *ptr++;
		id[1] = *ptr++;
		id[2] = *ptr;
		m_circle = circle_from_three(m_points[id[0]], m_points[id[1]], m_points[id[2]]);
		return;
	}

	if (m_disk_point_id.size() == 2)
	{
		auto ptr = m_disk_point_id.begin();
		uint32_t id[2];
		id[0] = *ptr++;
		id[1] = *ptr;
		m_circle = circle_from_two(m_points[id[0]], m_points[id[1]]);
	}

	for (uint32_t i = 0; i < size; i++)
	{
		if (!is_inside(m_circle, m_points[i]))
		{
			m_disk_point_id.push_back(i);
			welzl_proc(m_points, i);
			m_disk_point_id.pop_back();
		}
	}
}


bool Welzl::is_inside(const Circle& c, const Point& p)
{
	return distance(c.center, p) <= c.radius + epsilon;
}

Welzl::type_welzl Welzl::distance(const Point& a, const Point& b)
{
	type_welzl dx = a.x - b.x;
	type_welzl dy = a.y - b.y;
	return sqrtf(dx * dx + dy * dy);
}

// Окружность по двум точкам
Welzl::Circle Welzl::circle_from_two(const Point& a, const Point& b) {
	Point center((a.x + b.x) / 2.0f, (a.y + b.y) / 2.0f);
	return Circle(center, distance(a, b) / 2.0f);
}

// Окружность по трём точкам
// ( описанная окружность треугольника
Welzl::Circle Welzl::circle_from_three(const Point& a, const Point& b, const Point& c)
{
	type_welzl d = 2.0f * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
	if (fabsf(d) < epsilon)
	{
		return Circle(Point(), -1); // коллинеарные точки
	}

	type_welzl ux =
		(a.x * a.x + a.y * a.y) * (b.y - c.y) +
		(b.x * b.x + b.y * b.y) * (c.y - a.y) +
		(c.x * c.x + c.y * c.y) * (a.y - b.y);

	type_welzl uy =
		(a.x * a.x + a.y * a.y) * (c.x - b.x) +
		(b.x * b.x + b.y * b.y) * (a.x - c.x) +
		(c.x * c.x + c.y * c.y) * (b.x - a.x);

	Point center(ux / d, uy / d);
	return Circle(center, distance(center, a));
}
