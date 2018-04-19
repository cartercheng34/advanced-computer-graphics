#include <iostream>
#include "algebra3.h"
#include <vector>
#include <sstream>
#include "Base.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
    public:
        Camera(vec3 eyePoint_ , vec3 view_d , vec2 resolution , float fov_);        
        float viewPlaneHalfWidth;
        float viewPlaneHalfHeight;
        vec3 lookAtPoint;
        vec3 xPixVector;
        vec3 yPixVector;

    private:
        float fov;
        vec3 eyePoint;
        vec3 view_dir;
        vec3 up = vec3(0,1,0);
        double xResolution;
        double yResolution;
        float aspectRatio;
        float distance = 1.0; // distance to plane
};

#endif