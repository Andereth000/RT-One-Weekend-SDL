#ifndef PLANE_H
#define PLANE_H

#include "hitable.h"
#include "material.h"

class bounded_plane : public hitable {
public:
    vec3 point;
    vec3 normal;
    float width;
    float height;
    material* mat_ptr;

    bounded_plane(const vec3& p, const vec3& n, float w, float h, material* m)
        : point(p), normal(n), width(w), height(h), mat_ptr(m) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
};

#endif // PLANE_H
