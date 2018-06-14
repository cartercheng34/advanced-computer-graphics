#include "include.h"
#include "Base.h"
#include "aabbox.h"

using namespace std;

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

class Primitive
{
public:
    Primitive(){}
    Primitive(Triangle* tri) : tri_(tri) {}

	virtual bool intersect(Ray& ray, float& t_hit, Intersection & in);
    virtual bool intersectP(Ray& ray);
    
	virtual AABBox get_bounding_box();

protected:
    Triangle* tri_;

};

#endif