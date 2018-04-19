#include <iostream>
#include "algebra3.h"
#include "Camera.h"
#include <cmath>

using namespace std;

Camera::Camera(vec3 eyePoint_ , vec3 view_d , vec2 resolution , float fov_){
    eyePoint = eyePoint_;
    view_dir = view_d;
    fov = fov_;
    xResolution = resolution[0];
    yResolution = resolution[1];

    lookAtPoint = eyePoint + view_dir * distance;
    aspectRatio = yResolution / xResolution;

    viewPlaneHalfWidth = tan(fov * M_PI / 2 / 180);
    viewPlaneHalfHeight = aspectRatio*viewPlaneHalfWidth;
    
    vec3 U = (up ^ view_dir).normalize(); //plane x vec
    vec3 V = (view_dir ^ U).normalize(); //plane y vec

    xPixVector = (U*2*viewPlaneHalfWidth)/xResolution;
    yPixVector = (V*2*viewPlaneHalfHeight)/yResolution;
    //cout << "lol:" << aspectRatio <<endl;
    //viewPlaneBottomLeftPoint = lookatPoint - V*viewPlaneHalfHeight - U*viewPlaneHalfWidth
}