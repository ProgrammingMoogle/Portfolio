// SquareMesh.cpp
// -- non-inlines of SquareMesh class
// cs200 9/17

#include "SquareMesh.h"


const Point SquareMesh::vertices[4]
  = { Point(-1,-1),  // 0
      Point(1,-1),   // 1
      Point(1,1),    // 2
      Point(-1,1)    // 3
    };


const Mesh::Face SquareMesh::faces[2]
  = { Face(0,1,2),  Face(0,2,3) };


const Mesh::Edge SquareMesh::edges[4]
  = { Edge(0,1),  Edge(1,2),  Edge(2,3),  Edge(3,0) };

