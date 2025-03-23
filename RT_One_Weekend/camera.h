#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include "utility.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class camera
{
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist);

    ray get_ray(float s, float t);

    void move_forward(float distance);
    void move_backward(float distance);
    void move_left(float distance);
    void move_right(float distance);
    void move_up(float distance);
    void move_down(float distance);
    void look_around(float dx, float dy);


    void update_camera();

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

#endif
