////////////////////
// Kacey Lei Yeo
// Assignment 3
// CS200
// Fall2017
////////////////////
#include "Affine.h"

Affine inverse(const Affine & A)
{
  Affine ans;
  Affine trans;
  Affine L;

  float det = 0;
  float indet = 0;

  det = (A[0].x * A[1].y) - (A[0].y * A[1].x);
  indet = 1 / det;

  L[0].x = indet * A[1].y;
  L[1].y = indet * A[0].x;
  L[0].y = indet * -1 * A[0].y;
  L[1].x = indet * -1 * A[1].x;

  trans[0].x = 1;
  trans[1].y = 1;
  trans[0].w = A[0].w * -1;
  trans[1].w = A[1].w * -1;
  trans[2].w = A[2].w;

  ans = L * trans;

  return ans;
}