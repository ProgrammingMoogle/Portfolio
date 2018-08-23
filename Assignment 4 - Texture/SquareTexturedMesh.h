// SquareTexturedMesh.h
// -- textured standard square
// cs200 9/17

#ifndef CS200_SQUARETEXTUREDMESH_H
#define CS200_SQUARETEXTUREDMESH_H

#include "TexturedMesh.h"


class SquareTexturedMesh : public TexturedMesh {
  public:
    int vertexCount(void);
    const Point* vertexArray(void);
    Vector dimensions(void);
    Point center(void);
    int faceCount(void);
    const Face* faceArray(void);
    int edgeCount(void);
    const Edge* edgeArray(void);
    const Point* texcoordArray(void);
  private:
    static const Point vertices[4];
    static const Point texcoords[4];
    static const Face faces[2];
    static const Edge edges[4];
};


inline
int SquareTexturedMesh::vertexCount(void) {
  return 4;
}


inline
const Point* SquareTexturedMesh::vertexArray(void) {
  return vertices;
}


inline
Vector SquareTexturedMesh::dimensions(void) {
  return Vector(2,2);
}


inline
Point SquareTexturedMesh::center(void) {
  return Point(0,0);
}


inline
int SquareTexturedMesh::faceCount(void) {
  return 2;
}


inline
const Mesh::Face* SquareTexturedMesh::faceArray(void) {
  return faces;
}


inline
int SquareTexturedMesh::edgeCount(void) {
  return 4;
}


inline
const Mesh::Edge* SquareTexturedMesh::edgeArray(void) {
  return edges;
}


inline
const Point* SquareTexturedMesh::texcoordArray(void) {
  return texcoords;
}


#endif

