#ifndef VECTOR3_H
#define VECTOR3_H

#include <vector>

class Vector3
{
public:

  Vector3()
  {
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
  }

  Vector3(double x, double y, double z)
  {
    data[0] = x;
    data[1] = y;
    data[2] = z;
  }

  Vector3(const Vector3 & x) 
  {
    data[0] = x.data[0];
    data[1] = x.data[1];
    data[2] = x.data[2];
  }

  double dot(const Vector3 & x) const
  {
    return data[0] * x.data[0] + data[1] * x.data[1] + data[2] * x.data[2];
  }

  Vector3 cross(const Vector3 & x) const
  {
      return Vector3(
          data[1] * x.data[2] - data[2] * x.data[1],
          data[2] * x.data[0] - data[0] * x.data[2],
          data[0] * x.data[1] - data[1] * x.data[0]
      );
  }

  void operator+=(const Vector3 & x)
  {
    data[0] += x.data[0];
    data[1] += x.data[1];
    data[2] += x.data[2];
  }

  void operator=(const Vector3 & x)
  {
    data[0] = x.data[0];
    data[1] = x.data[1];
    data[2] = x.data[2];
  }

  Vector3 operator-(const Vector3 & x) const
  {
    return Vector3(data[0] - x.data[0], data[1] - x.data[1], data[2] - x.data[2]);
  }

  Vector3 operator+(const Vector3 & x) const
  {
    return Vector3(data[0] + x.data[0], data[1] + x.data[1], data[2] + x.data[2]);
  }

  Vector3 operator*(double x) const
  {
    return Vector3(data[0] * x, data[1] * x, data[2] * x);
  }

  double operator[](int i) const
  {
    return data[i];
  }

  double& operator[](int i)
  {
    return data[i];
  }

private:
  double data[3];
};

Vector3 operator*(double x, const Vector3 & v)
{
  return Vector3(v[0] * x, v[1] * x, v[2] * x);
}
#endif // #define VECTOR3_H

