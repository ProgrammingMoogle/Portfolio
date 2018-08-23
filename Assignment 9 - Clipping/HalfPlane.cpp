////////////////////
// Kacey Lei Yeo
// Assignment 8
// CS200
// Fall2017
////////////////////
#include "HalfPlane.h"
#include "Affine.h"

HalfPlane::HalfPlane(const Vector & n, const Point & C)
{
  Hcoord::x = n.x;
  Hcoord::y = n.y;
  Hcoord::w = -((n.x*C.x + n.y * C.y));
}

HalfPlane::HalfPlane(const Point & A, const Point & B, const Point & P)
{
  Vector m;

  m.x = A.y*B.w - A.w*B.y;
  m.y = A.w*B.x - A.x*B.w;
  m.w = A.x*B.y - A.y*B.x;

  Hcoord::x = m.x;
  Hcoord::y = m.y;
  Hcoord::w = m.w;
}

float dot(const HalfPlane & h, const Point & Q)
{
  float determine;

  determine = (h.x * Q.x) + (h.y * Q.y) + (h.w * Q.w);

  return determine;
}

Interval intersect(const HalfPlane & h, const Point & P, const Point & Q)
{
  Interval sect;
  float endP;
  float endQ;
  float checkH;

  checkH = h.x*h.y*h.w;

  endP = dot(h, P);
  endQ = dot(h, Q);

  if (checkH == endP && checkH != endQ)
  {
    sect.bgn = 0;
    sect.end = endP / (endP - endQ);
    return sect;
  }
  else if (checkH == endQ && checkH != endP)
  {
    sect.bgn = endP / (endP - endQ);
    sect.end = 1;
    return sect;
  }
  else if (checkH == endP && checkH == endQ)
  {
    sect.bgn = 0;
    sect.end = 1;
    return sect;
  }
  else
  {
    sect.bgn = 0;
    sect.end = 0;
    return sect;
  }
}
