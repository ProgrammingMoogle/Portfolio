////////////////////
// Kacey Lei Yeo
// Project 1
// CS200
// Fall2017
////////////////////
#include "MyTexturedMesh.h"

int MyTexturedMesh::vertexCount(void)
{
  return 8;
}

const Point * MyTexturedMesh::vertexArray(void)
{
  return verts;
}

Vector MyTexturedMesh::dimensions(void)
{
  return Vector(4, 6);
}

Point MyTexturedMesh::center(void)
{
  return Point(2, 2);
}

int MyTexturedMesh::faceCount(void)
{
  return 4;
}

const Mesh::Face* MyTexturedMesh::faceArray(void)
{
  return faces;
}

int MyTexturedMesh::edgeCount(void)
{
  return 8;
}

const Mesh::Edge * MyTexturedMesh::edgeArray(void)
{
  return edges;
}

const Point *MyTexturedMesh::texcoordArray(void)
{
  return texcoords;
}

const Point MyTexturedMesh::verts[8] =
{
  Point(0,0),
  Point(4,0),
  Point(4,3),
  Point(0,3),
  Point(1,4),
  Point(1,-1),
  Point(-1,2),
};

const Point MyTexturedMesh::texcoords[8]
{
  Point(0,0),
  Point(4,0),
  Point(4,3),
  Point(0,3),
  Point(1,4),
  Point(1,-1),
  Point(-1,2),
};

const MyTexturedMesh::Face MyTexturedMesh::faces[4] =
{
  Face(0,1,2),
  Face(0,2,3),
  Face(2,3,4),
  Face(0,1,5)
};


const MyTexturedMesh::Edge MyTexturedMesh::edges[8] =
{
  Edge(1,2),
  Edge(3,0),
  Edge(3,4),
  Edge(4,2),
  Edge(0,5),
  Edge(1,5)
};
