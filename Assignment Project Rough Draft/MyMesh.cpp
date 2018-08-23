////////////////////
// Kacey Lei Yeo
// Project 1
// CS200
// Fall2017
////////////////////
#include "MyMesh.h"

int MyMesh::vertexCount(void)
{
  return 8;
}

const Point * MyMesh::vertexArray(void)
{
  return verts;
}

Vector MyMesh::dimensions(void)
{
  return Vector(4,6);
}

Point MyMesh::center(void)
{
  return Point(2,2);
}

int MyMesh::faceCount(void)
{
  return 4;
}

const Mesh::Face* MyMesh::faceArray(void)
{
  return faces;
}

int MyMesh::edgeCount(void)
{
  return 8;
}

const Mesh::Edge * MyMesh::edgeArray(void)
{
  return edges;
}

const Point MyMesh::verts[8] =
{
  Point(0,0),
  Point(4,0),
  Point(4,3),
  Point(0,3),
  Point(1,4),
  Point(1,-1),
  Point(-1,2),
};

const MyMesh::Face MyMesh::faces[4] = 
{ 
  Face(0,1,2), 
  Face(0,2,3), 
  Face(2,3,4), 
  Face(0,1,5) 
};


const MyMesh::Edge MyMesh::edges[8] = 
{ 
  Edge(1,2), 
  Edge(3,0), 
  Edge(3,4), 
  Edge(4,2), 
  Edge(0,5), 
  Edge(1,5) 
};