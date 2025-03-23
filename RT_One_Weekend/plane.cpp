#include "plane.h"

bool bounded_plane::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    float denom = dot(normal, r.direction());

    if (fabs(denom) > 0.0001f) {
        float t = dot(point - r.origin(), normal) / denom;

        if (t >= t_min && t <= t_max) {
            vec3 hit_point = r.point_at_parameter(t);
            vec3 diff = hit_point - point;
            float dx = fabs(diff.x());
            float dz = fabs(diff.z());

            if (dx <= width / 2.0f && dz <= height / 2.0f) {
                rec.t = t;
                rec.p = hit_point;
                rec.normal = normal;
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    return false;
}
