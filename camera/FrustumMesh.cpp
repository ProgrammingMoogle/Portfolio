/////////////////////
//
//	Kacey Lei Yeo
//	Assignment 4
//	CS250
//	Spring 2018
//
/////////////////////
#include "FrustumMesh.h"

FrustumMesh::FrustumMesh(float fov, float aspect, float N, float F)
{
    float nearW = 0;
    float nearH = 0;
    float farW = 0;
    float farH = 0;
    int hold = 0;
    center_point = Point(0, 0, 0);

    nearW = (2 * N)*(tan(fov / 2));
    nearH = nearW / aspect;

    vertices[0] = Point(nearW / 2, nearH / 2, -N);
    vertices[1] = Point(-nearW / 2, nearH / 2, -N);
    vertices[2] = Point(nearW / 2, -nearH / 2, -N);
    vertices[3] = Point(-nearW / 2, -nearH / 2, -N);

    farW = (2 * F)*(tan(fov / 2));
    farH = farW / aspect;

    vertices[4] = Point(farW / 2, farH / 2, -F);
    vertices[5] = Point(-farW / 2, farH / 2, -F);
    vertices[6] = Point(farW / 2, -farH / 2, -F);
    vertices[7] = Point(-farW/2, -farH / 2, -F);
    viewport_dimensions = Vector(nearW*aspect, nearH*aspect, F - N);
}

int FrustumMesh::vertexCount(void)
{
    return 9;
}

Point FrustumMesh::getVertex(int i)
{
    return vertices[i];
}

Vector FrustumMesh::dimensions(void)
{
    return viewport_dimensions;
}

Point FrustumMesh::center(void)
{
    return center_point;
}

int FrustumMesh::faceCount(void)
{
    return 12;
}

Mesh::Face FrustumMesh::getFace(int i)
{
    
    return faces[i];
}

int FrustumMesh::edgeCount(void)
{
    return 16;
}

Mesh::Edge FrustumMesh::getEdge(int i)
{
    
    return edges[i];
}

const Mesh::Edge FrustumMesh::edges[16]
{
    //Near face DONE
    //TOP
    Mesh::Edge(0,1),
    //Right
    Mesh::Edge(0,2),
    //Left
    Mesh::Edge(1,3),
    //Bot
    Mesh::Edge(2,3),

    //Corners for Pyramid from Near Face
    Mesh::Edge(4,0),
    Mesh::Edge(5,1),
    Mesh::Edge(6,2),
    Mesh::Edge(7,3),


    //Far face
    //Top
    Mesh::Edge(4,5),
    //Right
    Mesh::Edge(4,6),
    //Left
    Mesh::Edge(5,7),
    //Bot
    Mesh::Edge(6,7),
    
    //Tip of Pyramid from  far face
    Mesh::Edge(8,6),
    Mesh::Edge(8,5),
    Mesh::Edge(8,4),
    Mesh::Edge(8,7),
};

const Mesh::Face FrustumMesh::faces[12]
{
    
    Mesh::Face(0,1,2),
    Mesh::Face(2,3,4),
    Mesh::Face(1,3,7),
    Mesh::Face(2,1,6),
    Mesh::Face(4,5,0),
    Mesh::Face(4,6,0),
    Mesh::Face(5,7,1),
    Mesh::Face(6,7,3),
    Mesh::Face(5,1,4),
    Mesh::Face(6,3,2),
    Mesh::Face(4,3,1)
};