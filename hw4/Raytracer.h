#include <iostream>
#include "include.h"
#include <fstream>
#include <vector>
#include <string>
#include "algebra3.h"
#include "Base.h"
#include "Camera.h"
#include "Primitive.h"

#ifndef RAY_TRACER_H
#define RAY_TRACER_H

using namespace std;

class Raytracer;

class Raytracer {
    public:
        Raytracer() {};
        bool parseInput(const string&);
        void render();
        vec3 trace(Ray ray, int reflect_count);
        bool findFirstObject(Ray ray, Intersection* intersection);
        Camera* cam;

    private:
        vec3 origin; //camera pos
        float fov; // degree
        vec3 unit_view_d; //view direction
        vec2 resolution; //w x h
        vector<Triangle*> t_buffer;
        vector<Sphere*> s_buffer;
        vector<Object*> o_buffer;
        vector<Light*> l_buffer;
        int counter = 0;
        int max_reflect = 2;
        KDNode* tree;
        
};

#endif // RAY_TRACER_H