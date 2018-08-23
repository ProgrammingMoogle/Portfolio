////////////////////
// Kacey Lei Yeo
// Project 1
// CS200
// Fall2017
////////////////////
#include "TexturedMesh.h"

struct MyTexturedMesh : public TexturedMesh 
{
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
  static const Point verts[8];
  static const Face faces[4];
  static const Edge edges[8];
  static const Point texcoords[8];
};
