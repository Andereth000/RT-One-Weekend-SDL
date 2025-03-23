#ifndef UTILITYH
#define UTILITYH

#include "vec3.h"

double random_double();

vec3 random_in_unit_sphere();
vec3 random_in_unit_disk();

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted);

float schlick(float cosine, float ref_idx);

#endif 

