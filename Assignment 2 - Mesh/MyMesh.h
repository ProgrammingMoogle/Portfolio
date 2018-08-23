////////////////////
// Kacey Lei Yeo
// Assignment 2
// CS200
// Fall2017
////////////////////
#ifndef CS200_MYMESH_H
#define CS200_MYMESH_H

#include "Mesh.h"
#include "Affine.h"

class MyMesh : public Mesh
{
public:
  int vertexCount(void);
  const Point* vertexArray(void);
  Vector dimensions(void);
  Point center(void);
  int faceCount(void);
  const Face* faceArray(void);
  int edgeCount(void);
  const Edge* edgeArray(void);


private:
  static const Point verts[8];
  static const Face faces[4];
  static const Edge edges[8];
};

#endif // !CS200_MYMESH_H
