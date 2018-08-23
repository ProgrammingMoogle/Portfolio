// FrustumMesh.h
// -- 3D mesh for a view frustum
// cs250 1/18

#ifndef CS250_FRUSTUMMESH_H
#define CS250_FRUSTUMMESH_H

#include "Mesh.h"


class FrustumMesh : public Mesh {
  public:
    FrustumMesh(float fov, float aspect, float N, float F);
    int vertexCount(void);
    Point getVertex(int i);
    Vector dimensions(void);
    Point center(void);
    int faceCount(void);
    Face getFace(int i);
    int edgeCount(void);
    Edge getEdge(int i);
  private:
    Point vertices[9];
    Point center_point;
    Vector viewport_dimensions;
    static const Edge edges[16];
    static const Face faces[12];
};


#endif

