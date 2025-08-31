#ifndef HIT_RECORD_H
#define HIT_RECORD_H
#include "../math/ray.h"

class hit_record
{
public:
    vector3 p;
    vector3 normal;
    double t;
    bool front_face;

    void set_face_normal(ray r, vector3 outward_normal)
    {
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable
{
public:
    virtual ~hittable() = default;
    virtual bool hit(ray r, double ray_tmin, double ray_tmax, hit_record &rec) const = 0;
};

class hittable_list
{
public:
};

#endif