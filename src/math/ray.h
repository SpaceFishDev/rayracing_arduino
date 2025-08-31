#ifndef RAY_H
#define RAY_H
#include "vector3.h"

class ray
{
public:
    ray(const vector3 &origin, const vector3 &direction)
    {
        this->origin = origin;
        this->direction = direction;
    }
    vector3 origin;
    vector3 direction;
    vector3 at(double t)
    {
        return origin + direction * t;
    }
};

#endif
