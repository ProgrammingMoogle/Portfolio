////////////////////
// Kacey Lei Yeo
// Assignment 8
// CS200
// Fall2017
////////////////////
#include "PointContainment.h"
#include "Affine.h"
#include "HalfPlane.h"
#include "Mesh.h"

bool pointInTriangle(const Point & P, const Point & A, const Point & B, const Point & C)
{
  Vector aB;
  Vector Bc;
  Vector cA;

  float check;

  aB.x = A.y*B.w - A.w*B.y;
  aB.y = A.w*B.x - A.x*B.w;;
  aB.w = A.x*B.y - A.y*B.x;

  Bc.x = B.y*C.w - B.w*C.y;
  Bc.y = B.w*C.x - B.x*C.w;
  Bc.w = B.x*C.y - B.y*C.x;

  cA.x = C.y*A.w - C.w*A.y;
  cA.y = C.w*A.x - C.x*A.w;
  cA.w = C.x*A.y - C.y*A.x;

  check = dot(aB, P);

  if (check <= 0)
  {
    check = dot(Bc, P);
    if (check <= 0)
    {
      check = dot(cA, P);
      if (check <= 0)
      {
        return true;
      }
    }
  }

  return false;
}

bool pointInMesh(const Point & P, Mesh & mesh)
{
  for (int i = 0; i < mesh.vertexCount(); i++)
  {
    if (mesh.vertexArray() > &P)
    {
      return true;
    }
  }
  return false;
}
