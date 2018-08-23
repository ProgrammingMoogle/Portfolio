// Clip.h
// -- clip/cull with respect to a given convex polygon
// cs200 11/17

#ifndef CS200_CLIP_H
#define CS200_CLIP_H

#include <vector>
#include "Affine.h"
#include "HalfPlane.h"


class Clip {
  public:
    Clip(void);
    Clip(const std::vector<Point>& clip_obj_verts);
    bool operator()(Point& P, Point& Q);
    bool operator()(std::vector<Point>& verts);
  private:
    std::vector<HalfPlane> half_planes;
    std::vector<Point> temp_vertices;
    bool clip_to_halfplane(const HalfPlane& h, std::vector<Point>& verts);
};


#endif

