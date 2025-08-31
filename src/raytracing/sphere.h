#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"

#include "../lcd/lcd.h"
#include "../text/font.h"
#include <util/delay.h>

class sphere : public hittable
{
public:
    vector3 center;
    double radius;
    sphere(vector3 center, double radius) : center(center), radius(fmax(0, radius))
    {
    }
    bool hit(ray r, double ray_tmin, double ray_tmax, hit_record &rec) const override
    {
        vector3 oc = center - r.origin;
        double a = r.direction.length_squared();
        double h = dot(r.direction, oc);
        double c = oc.length_squared() - radius * radius;

        double discriminant = h * h - a * c;
        if (discriminant < 0)
        {
            return false;
        }
        double sqrtd = sqrt(discriminant);
        double root = (h - sqrtd) / a;
        if (root <= ray_tmin || root >= ray_tmax)
        {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || root >= ray_tmax)
            {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vector3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        return true;
    }
};

#endif