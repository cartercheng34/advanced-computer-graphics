#include "include.h"
#include <iostream>
#include <vector>
#include "algebra3.h"
#include "kdtree.h"

using namespace std;
// Build KD tree for tris
KDNode* KDNode::build(vector<Triangle*> &tris, int depth){
    KDNode* node = new KDNode();
    node->leaf = false;
    node->triangles = vector<Triangle*>();
    node->left = NULL;
    node->right = NULL;
    node->box = AABBox();

    if (tris.size() == 0) 
        return node;
    /*
    if (tris.size() == 1) {

        node->box = tris[0]->get_bounding_box();
        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = vector<Triangle*>();
        node->right->triangles = vector<Triangle*>();

        return node;
    }
    */

    if (depth > 25 || tris.size() <= 8) {
        node->triangles = tris;
        node->leaf = true;
        node->box = tris[0]->get_bounding_box();

        for (long i=1; i<tris.size(); i++) {
            AABBox tmp_b = tris[i]->get_bounding_box();
            node->box.expand(tmp_b);
        }

        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = vector<Triangle*>();
        node->right->triangles = vector<Triangle*>();

        return node;
    }

    node->box = tris[0]->get_bounding_box();
    vec3 midpt = vec3();
    double weight = 1.0/tris.size();

    for (long i=1; i<tris.size(); i++) {
        AABBox tmp_b = tris[i]->get_bounding_box();
        node->box.expand(tmp_b);
        midpt = midpt + (tris[i]->get_midpoint() * weight);
    }

    vector<Triangle*> left_tris;
    vector<Triangle*> right_tris;
    int axis = node->box.get_longest_axis();

    for (long i=0; i<tris.size(); i++) {
        switch (axis) {
            case 0:
                midpt[0] >= tris[i]->get_midpoint()[0] ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
            case 1:
                midpt[1] >= tris[i]->get_midpoint()[1] ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
            case 2:
                midpt[2] >= tris[i]->get_midpoint()[2] ? right_tris.push_back(tris[i]) : left_tris.push_back(tris[i]);
                break;
        }
    }

    if (tris.size() == left_tris.size() || tris.size() == right_tris.size()) {
        node->triangles = tris;
        node->leaf = true;
        node->box = tris[0]->get_bounding_box();

        for (long i=1; i<tris.size(); i++) {
            AABBox tmp_b = tris[i]->get_bounding_box();
            node->box.expand(tmp_b);
        }

        node->left = new KDNode();
        node->right = new KDNode();
        node->left->triangles = vector<Triangle*>();
        node->right->triangles = vector<Triangle*>();

        return node;
    }

    node->left = build(left_tris, depth+1);
    node->right = build(right_tris, depth+1);

    return node;
}

// Finds nearest triangle in kd tree that intersects with ray.
bool KDNode::hit(KDNode *node, Ray &ray, double &t, double &tmin, vec3 &normal, Material* &m, vec3 &p) {
    double dist;
    if (node->box.intersection(ray, dist)){
        if (dist > tmin) return false;

        bool hit_tri = false;
        bool hit_left = false;
        bool hit_right = false;
        long tri_idx;

        if (!node->leaf) {
            //if ( node->left->triangles.size() > 0 )
            hit_left = hit(node->left, ray, t, tmin, normal, m, p);

            //if ( node->right->triangles.size() > 0 )
            hit_right = hit(node->right, ray, t, tmin, normal, m, p);

            return hit_left || hit_right;
        }
        else {
            long triangles_size = node->triangles.size();
            for (long i=0; i<triangles_size; i++) {
                if (node->triangles[i]->intersect(ray).result){
                    hit_tri = true;
                    tmin = (node->triangles[i]->intersect(ray).intersect_Point - ray.get_origin()).length();
                    tri_idx = i;
                    normal = node->triangles[i]->intersect(ray).unit_normal;
                }
            }
            if (hit_tri) {
                p = ray.get_origin() + ray.get_direction() * tmin;
                m = node->triangles[tri_idx]->material_;
                return true;
            }
        }
    }
    return false;
}