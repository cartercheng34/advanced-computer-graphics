#include <iostream>
#include "algebra3.h"
#include "Base.h"
#include <cmath>


Triangle::Triangle(vec3 p0_, vec3 p1_, vec3 p2_, Material* material, vec3 t_normal_){
    p0 = p0_;
    p1 = p1_;
    p2 = p2_;
    material_ = material;
    t_normal = t_normal_;
}

AABBox Triangle::get_bounding_box(){
    vec3 bl = vec3(
        std::min (std::min(p0[0], p1[0]), p2[0] ) ,
        std::min (std::min(p0[1], p1[1]), p2[1] ) ,
        std::min (std::min(p0[2], p1[2]), p2[2] )
    );
    vec3 tr = vec3(
        std::max (std::max(p0[0], p1[0]), p2[0] ) ,
        std::max (std::max(p0[1], p1[1]), p2[1] ) ,
        std::max (std::max(p0[2], p1[2]), p2[2] )
    );

    return AABBox(bl, tr);
}

Intersection Triangle::intersect(Ray &ray){
    Intersection check;
    check.result = false;
    
    const float epsilon = 0.0000001;
    vec3 edge1 = p1 - p0;
    vec3 edge2 = p2 - p0;

    vec3 pvec = ray.get_direction() ^ edge2;
    float det = edge1 * pvec;

    if (det > -epsilon && det < epsilon)
        return check;
    
    float invDet = 1 / det;
    vec3 svec = ray.get_origin() - p0;
    float u = svec * pvec * invDet;

    if (u < 0.0 || u > 1.0) 
        return check;
    
    vec3 qvec = svec ^ edge1;
    float v = invDet * (ray.get_direction() * qvec);

    if (v < 0.0 || u + v > 1.0)
        return check;

    float t = invDet * edge2 * qvec;
    if (t > epsilon) // ray intersection
    {
        check.intersect_Point = ray.get_origin() + ray.get_direction() * t; 
        check.result = true;
        //check.unit_normal = (edge1 ^ edge2).normalize();   
        check.unit_normal = t_normal.normalize();
        check.m = material_;
    }
    return check;

}

Sphere::Sphere(vec3 center_, float r, Material* material){
    center = center_;
    radius = r;
    material_ = material;
}

Intersection Sphere::intersect(Ray &ray){
    Intersection check;
    check.result = false;

    float a = ray.get_direction() * ray.get_direction();
    float b = 2 * ray.get_direction() * (ray.get_origin() - center);
    float c = (ray.get_origin() - center) * (ray.get_origin() - center) - radius * radius;
    float D = b * b - a * c * 4;

    if (D >= 0){
        float t = (-b - sqrt(D))/ 2 / a ;
        if (t >= 0){
            check.intersect_Point = ray.get_origin() + t * ray.get_direction();
            check.result = true;
            check.unit_normal = (check.intersect_Point - center).normalize();
            check.m = material_;
        }
    }

    return check;
}

Mesh::Mesh(std::vector<Triangle*> tris_) {
    tris = tris_;
    node = KDNode().build(tris, 0);
}


Intersection Mesh::intersect(Ray &ray) {
    double t = 0, tmin=INFINITY;
    Intersection output;
    Material* material_;
    vec3 normal = vec3();
    vec3 colour = vec3();
    vec3 p = vec3();
    bool hit = node->hit(node, ray, t, tmin, normal, material_, p);
    //bool hit = bvh.getIntersection(ray, t, tmin, normal);

    output.result = hit;
    output.unit_normal = normal.normalize();
    output.m = material_;
    output.intersect_Point = p;
    return output;
}
