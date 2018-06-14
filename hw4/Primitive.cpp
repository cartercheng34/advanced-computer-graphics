#include "Primitive.h"


bool Primitive::intersect(Ray& ray, float& t_hit, Intersection& in)
{
	//temp test
	//in = new Intersection();
    in = tri_->intersect(ray);	

    t_hit = (in.intersect_Point - ray.get_origin()).length();

	return in.result;
}

bool Primitive::intersectP(Ray& ray)
{
	return tri_->intersect(ray).result;
}

AABBox Primitive::get_bounding_box()
{
	return tri_->get_bounding_box();
}