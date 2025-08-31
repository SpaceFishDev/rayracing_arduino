#ifndef VECTOR3_H
#define VECTOR3_H
#include <math.h>

class vector3
{
public:
    double x, y, z;
    vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    vector3(double _x, double _y, double _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    vector3 operator-()
        const
    {
        return vector3(-x, -y, -z);
    }
    vector3 &operator+=(const vector3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    vector3 &operator*=(double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    vector3 &operator/=(double t)
    {
        *this *= 1 / t;
        return *this;
    }
    double length_squared()
    {
        return x * x + y * y + z * z;
    }
    double length()
    {
        return sqrt(length_squared());
    }
};

static inline vector3 operator+(const vector3 &u, const vector3 &v)
{
    return vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}
static inline vector3 operator-(const vector3 &u, const vector3 &v)
{
    return u + (-v);
}
static inline vector3 operator*(const vector3 &u, const vector3 &v)
{
    return vector3(u.x * v.x, u.y * v.y, u.z * v.z);
}
static inline vector3 operator*(const vector3 &u, double t)
{
    return vector3(u.x * t, u.y * t, u.z * t);
}
static inline vector3 operator/(const vector3 &u, double t)
{
    return vector3(u.x / t, u.y / t, u.z / t);
}
static inline vector3 operator+(vector3 &u, double t)
{
    return vector3(u.x + t, u.y + t, u.z + t);
}
static inline double dot(const vector3 &u, const vector3 &v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
static inline vector3 cross(const vector3 &u, const vector3 &v)
{
    return vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}
static inline vector3 unit_vector(vector3 u)
{
    return u / u.length();
}

#endif