#include "camera.h"

camera::camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
{
    lens_radius = aperture / 2.0f;
    float theta = vfov * M_PI / 180.0f;
    float half_height = tan(theta / 2.0f);
    float half_width = aspect * half_height;
    origin = lookfrom;
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
    horizontal = 2.0f * half_width * focus_dist * u;
    vertical = 2.0f * half_height * focus_dist * v;
}

ray camera::get_ray(float s, float t)
{
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();
    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
}

void camera::move_forward(float distance)
{
    origin -= w * distance;
    update_camera();
}

void camera::move_backward(float distance)
{
    origin += w * distance;
    update_camera();
}

void camera::move_left(float distance)
{
    origin -= u * distance;
    update_camera();
}

void camera::move_right(float distance)
{
    origin += u * distance;
    update_camera();
}

void camera::move_up(float distance)
{
    origin += v * distance;
    update_camera();
}

void camera::move_down(float distance)
{
    origin -= v * distance;
    update_camera();
}

void camera::update_camera()
{
    float dist_to_focus = (origin - (origin - w)).length();
    lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - w;
    horizontal = 2.0f * dist_to_focus * u;
    vertical = 2.0f * dist_to_focus * v;
}
