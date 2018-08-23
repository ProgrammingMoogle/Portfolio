// TexturedMesh.h
// -- triangular mesh with texture coordinates attached to vertices
// cs200 9/17

#ifndef CS200_TEXTUREDMESH_H
#define CS200_TEXTUREDMESH_H


#include "Mesh.h"


struct TexturedMesh : Mesh {
  virtual const Point* texcoordArray(void) = 0;
};


#endif

