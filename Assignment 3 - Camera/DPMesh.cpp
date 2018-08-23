// DPMesh.cpp
// cs200 5/17

#include "DPMesh.h"


// vertex data for DP mesh
const Point DPMesh::dp_verts[18]
  = { Point(5,3),     Point(5,6),
      Point(2,6),     Point(0,4),
      Point(0,2),     Point(2,0),
      Point(5,0),     Point(5.5f,0),
      Point(6.5f,0),  Point(6.5f,12),
      Point(5.5f,12), Point(7,9),
      Point(7,6),     Point(10,6),
      Point(12,8),    Point(12,10),
      Point(10,12),   Point(7,12) };


// face data for DP mesh
const Mesh::Face DPMesh::dp_faces[12]
   = { Face(0,1,2),    Face(0,2,3),
       Face(0,3,4),    Face(0,4,5),
       Face(0,5,6),    Face(7,8,9),
       Face(7,9,10),   Face(11,12,13),
       Face(11,13,14), Face(11,14,15),
       Face(11,15,16), Face(11,16,17) };


// edge data for DP mesh
const Mesh::Edge DPMesh::dp_edges[18]
  = { Edge(0,1),   Edge(1,2),
      Edge(2,3),   Edge(3,4),
      Edge(4,5),   Edge(5,6),
      Edge(6,0),   Edge(7,8),
      Edge(8,9),   Edge(9,10),
      Edge(10,7),  Edge(11,12),
      Edge(12,13), Edge(13,14),
      Edge(14,15), Edge(15,16),
      Edge(16,17), Edge(17,11) };

