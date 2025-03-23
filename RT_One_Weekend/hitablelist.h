#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitablelist : public hitable {
public:
    hitablelist() {}
    hitablelist(hitable** l, int n) { list = l; list_size = n; }
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

    hitable** list;
    int list_size;
};

#endif


