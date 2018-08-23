// Render.h
// -- OpenGl 2D rendering interface
// cs200 9/17


#ifndef CS200_RENDER_H
#define CS200_RENDER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"
#include "Mesh.h"


class Render {
  public:
    Render(void);
    ~Render(void);
    void clearFrame(const Hcoord& c);
    void setModelToWorld(const Affine &M);
    void loadMesh(Mesh &m);
    void unloadMesh(void);
    void displayEdges(const Hcoord& c);
    void displayFaces(const Hcoord& c);
  private:
    GLint ucolor,
          utransform,
          aposition;
    GLuint program,
           vertex_buffer,
           edge_buffer,
           face_buffer;
    int mesh_edge_count,
        mesh_face_count;
};


#endif

