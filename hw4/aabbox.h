#ifndef AABBOX_H
#define AABBOX_H

#include "include.h"
#include <cfloat>
#include <algorithm>
#include "algebra3.h"
#include "Base.h"


// Axis-aligned bounding box
struct AABBox {
    vec3 bl;     // Bottom left (min)
    vec3 tr;     // Top right   (max)

    AABBox (vec3 bl_=vec3(), vec3 tr_=vec3()){
        bl=bl_, tr=tr_;
    }

    // Expand to fit box
    void expand(AABBox &box) {
        if (box.bl[0] < bl[0]) bl[0] = box.bl[0];
        if (box.bl[1] < bl[1]) bl[1] = box.bl[1];
        if (box.bl[2] < bl[2]) bl[2] = box.bl[2];

        if (box.tr[0] > tr[0]) tr[0] = box.tr[0];
        if (box.tr[1] > tr[1]) tr[1] = box.tr[1];
        if (box.tr[2] > tr[2]) tr[2] = box.tr[2];
    }

    // Expand to fit point
    void expand(vec3 &vec) {
        if (vec[0] < bl[0]) bl[0] = vec[0];
        if (vec[1] < bl[1]) bl[1] = vec[1];
        if (vec[2] < bl[2]) bl[2] = vec[2];
    }

    // Return longest axis: 0, 1, 2 for x, y, z respectively
    int get_longest_axis() {
        vec3 diff = tr - bl;
        if (diff[0] > diff[1] && diff[0] > diff[2]) 
            return 0;
        if (diff[1] > diff[0] && diff[1] > diff[2]) 
            return 1;

        return 2;
    }

    // Check if ray intersects with box. Returns true/false and stores distance in t
    bool intersection(Ray &r, double &t) {
        vec3 direction_inv = vec3(1 / r.get_direction()[0] , 1 / r.get_direction()[1] , 1 / r.get_direction()[2]);
        double tx1 = (bl[0] - r.get_origin()[0]) * direction_inv[0];
        double tx2 = (tr[0] - r.get_origin()[0]) * direction_inv[0];

        double tmin = std::min(tx1, tx2);
        double tmax = std::max(tx1, tx2);

        double ty1 = (bl[1] - r.get_origin()[1]) * direction_inv[1];
        double ty2 = (tr[1] - r.get_origin()[1]) * direction_inv[1];

        tmin = std::min(tmin, std::min(ty1, ty2));
        tmax = std::max(tmax, std::max(ty1, ty2));

        double tz1 = (bl[1] - r.get_origin()[2]) * direction_inv[2];
        double tz2 = (tr[1] - r.get_origin()[2]) * direction_inv[2];

        tmin = std::min(tmin, std::min(tz1, tz2));
        tmax = std::max(tmax, std::max(tz1, tz2));
        t = tmin;

        return tmax >= tmin;
    }
};

#endif // AABBOX_H