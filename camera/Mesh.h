// Mesh.h
// -- 3D triangular mesh interface
// cs250 1/18

#ifndef CS250_MESH_H
#define CS250_MESH_H

#include "Affine.h"


struct Mesh {

  struct Face {
    int index1, index2, index3;
    Face(int i=-1, int j=-1, int k=-1)
        : index1(i), index2(j), index3(k) {}
  };

  struct Edge {
    int index1, index2;
    Edge(int i=-1, int j=-1)
        : index1(i), index2(j) {}
  };

  virtual ~Mesh(void) {}
  virtual int vertexCount(void) = 0;
  virtual Point getVertex(int i) = 0;
  virtual Vector dimensions(void) = 0;
  virtual Point center(void) = 0;
  virtual int faceCount(void) = 0;
  virtual Face getFace(int i) = 0;
  virtual int edgeCount(void) = 0;
  virtual Edge getEdge(int i) = 0;

};


#endif

