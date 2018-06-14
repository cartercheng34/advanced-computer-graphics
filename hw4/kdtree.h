#ifndef KDTREE_H
#define KDTREE_H

#include "include.h"
#include <vector>
#include "aabbox.h"
#include "algebra3.h"
#include "Base.h"

class KDNode {
public:
    AABBox box;
    KDNode* left;
    KDNode* right;
    std::vector<Triangle*> triangles;
    bool leaf;

    KDNode(){};
    KDNode* build(std::vector<Triangle*> &tris, int depth);
    bool hit (KDNode* node, Ray &ray, double &t, double &tmin, vec3 &normal, Material* &m, vec3 &p);
};

#endif // KDTREE_H