#ifndef HIT_RECORD_H
#define HIT_RECORD_H
#include "../math/ray.h"
#include "../vector/vector.h"
#include "../text/font.h"

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
    virtual bool hit(ray r, double ray_tmin, double ray_tmax, hit_record &rec) const = 0;
};

class hittable_list : public hittable
{
public:
    vector<hittable *> objects;
    ~hittable_list()
    {
        this->clear();
    }
    hittable_list()
    {

        objects = vector<hittable *>();
    }
    void clear()
    {
        objects.clear();
    }
    void add(hittable *o)
    {
        objects.add(o);
    }
    bool hit(ray r, double ray_tmin, double ray_tmax, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        double closest_so_far = ray_tmax;
        for (int i = 0; i < objects.num; ++i)
        {
            hittable *object = (hittable *)objects.get(i);
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};

#endif