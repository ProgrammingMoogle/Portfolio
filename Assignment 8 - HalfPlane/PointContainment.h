// PointContainment.h
// -- point containment functions
// cs200 11/17

#ifndef CS200_POINTCONTAINMENT_H
#define CS200_POINTCONTAINMENT_H

#include "Affine.h"
#include "Mesh.h"


bool pointInTriangle(const Point& P, const Point& A,
                     const Point& B, const Point& C);

bool pointInMesh(const Point& P, Mesh& mesh);


#endif

