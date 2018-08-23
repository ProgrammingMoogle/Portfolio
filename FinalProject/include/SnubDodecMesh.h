// SnubDodecMesh.h
// -- mesh for a snub dodecahedron
// cs250 1/18

#ifndef CS250_SNUBDODEC_H
#define CS250_SNUBDODEC_H

#include "Mesh.h"


class SnubDodecMesh : public Mesh {
  public:
    int vertexCount(void) { return 60; }
    Point getVertex(int i) { return vertices[i]; }
    Vector dimensions(void) { return Vector(2,2,2); }
    Point center(void) { return Point(0,0,0); }
    int faceCount(void) { return 116; }
    Face getFace(int i) { return faces[i]; }
    int edgeCount(void) { return 150; }
    Edge getEdge(int i) { return edges[i]; }
  private:
    static Point vertices[60];
    static Face faces[116];
    static Edge edges[150];
};


#endif

