// CubeMesh.h
// -- standard cube
// cs250 1/18

#ifndef CS250_CUBEMESH_H
#define CS250_CUBEMESH_H

#include "Mesh.h"

class CubeMesh : public Mesh {
  public:
    int vertexCount(void);
    Point getVertex(int i);
    Vector dimensions(void);
    Point center(void);
    int faceCount(void);
    Face getFace(int i);
    int edgeCount(void);
    Edge getEdge(int i);
  private:
    static const Point vertices[8];
    static const Face faces[12];
    static const Edge edges[12];
};

#endif

