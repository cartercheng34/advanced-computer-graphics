#include <iostream>
#include "algebra3.h"

#ifndef BASE_H
#define BASE_H

struct Intersection{    
    vec3 unit_normal;
    bool result;
    vec3 intersect_Point;    
};

class Ray{
    public:
        Ray(vec3 origin_ , vec3 direction_unit_vec_) : 
            origin(origin_), direction_unit_vec(direction_unit_vec_) {}
        vec3 get_origin(){ return origin; };
        vec3 get_direction(){ return direction_unit_vec; };
    private:
        vec3 origin;
        vec3 direction_unit_vec;
};

class Triangle {
    public:
        Triangle(vec3 p0_, vec3 p1_, vec3 p2_);
        Intersection intersect (Ray ray);
    private:
        vec3 p0;
        vec3 p1;
        vec3 p2;
};

class Sphere {
    public:
        Sphere(vec3 center_, float r);
        Intersection intersect (Ray ray);
        
    private:
        vec3 center;
        float radius;
};

#endif