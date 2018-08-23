// DPMesh.h
// cs200 5/17

#ifndef CS200_DPMESH_H
#define CS200_DPMESH_H

#include "Mesh.h"
#include "Affine.h"


class DPMesh : public Mesh {
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
    static const Point dp_verts[18];
    static const Face dp_faces[12];
    static const Edge dp_edges[18];
};


inline
int DPMesh::vertexCount(void) {
  return 18;
}


inline
const Point* DPMesh::vertexArray(void) {
  return dp_verts;
}


inline
Vector DPMesh::dimensions(void) {
  return Vector(12,12);
}


inline
Point DPMesh::center(void) {
  return Point(6,6);
}


inline
int DPMesh::faceCount(void) {
  return 12;
}


inline
const Mesh::Face* DPMesh::faceArray(void) {
  return dp_faces;
}


inline
int DPMesh::edgeCount(void) {
  return 18;
}


inline
const Mesh::Edge* DPMesh::edgeArray(void) {
  return dp_edges;
}


#endif

