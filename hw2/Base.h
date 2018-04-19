#include <iostream>
#include "algebra3.h"

#ifndef BASE_H
#define BASE_H

struct Intersection{    
    vec3 unit_normal;
    bool result;
    vec3 intersect_Point;    
};

struct Light {
    Light(vec3 light_pos_) : light_pos(light_pos_) {}
    vec3 light_pos;
    vec3 color = vec3(1.0, 1.0, 1.0);
    float intensity = 1.0;
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

struct Material {
    Material(vec3 m_color_ , float kAmbient_, float kDiffuse_, float kSpecular_, float specularity_, float reflectionFactor_, float refractionFactor_, float density_) : 
            m_color(m_color_), kAmbient(kAmbient_), kDiffuse(kDiffuse_), kSpecular(kSpecular_), specularity(specularity_), reflectionFactor(reflectionFactor_), refractionFactor(refractionFactor_), density(density_) {}

    vec3 m_color;
    float kAmbient, kDiffuse, kSpecular;
    vec3 ambientColor, diffuseColor, specularColor;   
    float specularity;
    float reflectionFactor;
    float refractionFactor;
    float density;
};

class Object {
    public:
        Material* material_;
        virtual Intersection intersect(Ray ray) = 0;
};



class Triangle : public Object{
    public:
        Triangle(vec3 p0_, vec3 p1_, vec3 p2_, Material* material);
        virtual Intersection intersect (Ray ray);
    private:
        vec3 p0;
        vec3 p1;
        vec3 p2;
};

class Sphere : public Object{
    public:
        Sphere(vec3 center_, float r, Material* material);
        virtual Intersection intersect (Ray ray);
        
    private:
        vec3 center;
        float radius;
};

#endif