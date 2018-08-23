// HalfPlane.h
// -- utilities for half-planes
// cs200 11/17

#ifndef CS200_HALFPLANE_H
#define CS200_HALFPLANE_H

#include "Affine.h"


struct HalfPlane : Hcoord 
{
  HalfPlane(float X=0, float Y=0, float W=0) : Hcoord(X,Y,W) { }
  HalfPlane(const Vector& n, const Point& C);
  HalfPlane(const Point& A, const Point& B, const Point& P);
};


struct Interval {
  float bgn, end;
  Interval(float a=0, float b=1) : bgn(a), end(b) { }
  bool isEmpty(void) const { return bgn > end; }
};


float dot(const HalfPlane& h, const Point& Q);
Interval intersect(const HalfPlane& h, const Point& P, const Point& Q);


#endif

