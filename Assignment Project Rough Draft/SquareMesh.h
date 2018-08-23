// SquareMesh.h
// -- standard square mesh
// cs200 9/17

#ifndef CS200_SQUAREMESH_H
#define CS200_SQUAREMESH_H

#include "Mesh.h"


class SquareMesh : public Mesh {
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
    static const Point vertices[4];
    static const Face faces[2];
    static const Edge edges[4];
};


inline
int SquareMesh::vertexCount(void) {
  return 4;
}


inline
const Point* SquareMesh::vertexArray(void) {
  return vertices;
}


inline
Vector SquareMesh::dimensions(void) {
  return Vector(2,2);
}


inline
Point SquareMesh::center(void) {
  return Point(0,0);
}


inline
int SquareMesh::faceCount(void) {
  return 2;
}


inline
const Mesh::Face* SquareMesh::faceArray(void) {
  return faces;
}


inline
int SquareMesh::edgeCount(void) {
  return 4;
}


inline
const Mesh::Edge* SquareMesh::edgeArray(void) {
  return edges;
}


#endif

