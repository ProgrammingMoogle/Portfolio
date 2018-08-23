// ClipDemo.cpp
// -- visual clipping test
// cs200 11/17
//
// from Visual Studio 2015 command prompt:
//     cl /EHsc ClipDemo.cpp Affine.lib Inverse.lib PointContainment.lib
//        HalfPlane.lib Clip.cpp opengl32.lib glew32.lib sdl2.lib
//        sdl2main.lib /link /subsystem:console
//
// from Linux command line:
//     g++ ClipDemo.cpp Affine.cpp Inverse.cpp HalfPlane.cpp
//         PointContainment.cpp Clip.cpp -lGL -lGLEW -lSDL2

#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"
#include "PointContainment.h"
#include "HalfPlane.h"
#include "Clip.h"
using namespace std;


/////////////////////////////////////////////////////////////////
class DPMesh : public Mesh {
  public:
    int vertexCount(void)          { return 18; }
    const Point* vertexArray(void) { return dp_verts; }
    Vector dimensions(void)        { return Vector(12,12); }
    Point center(void)             { return Point(6,6); }
    int faceCount(void)            { return 12; }
    const Face* faceArray(void)    { return dp_faces; }
    int edgeCount(void)            { return 18; }
    const Edge* edgeArray(void)    { return dp_edges; }
  private:
    static const Point dp_verts[18];
    static const Face dp_faces[12];
    static const Edge dp_edges[18];
};


const Point DPMesh::dp_verts[18]
  = { Point(5,3),     Point(5,6),     Point(2,6),     Point(0,4),
      Point(0,2),     Point(2,0),     Point(5,0),     Point(5.5f,0),
      Point(6.5f,0),  Point(6.5f,12), Point(5.5f,12), Point(7,9),
      Point(7,6),     Point(10,6),    Point(12,8),    Point(12,10),
      Point(10,12),   Point(7,12) };


const Mesh::Face DPMesh::dp_faces[12]
   = { Face(0,1,2),    Face(0,2,3),    Face(0,3,4),    Face(0,4,5),
       Face(0,5,6),    Face(7,8,9),    Face(7,9,10),   Face(11,12,13),
       Face(11,13,14), Face(11,14,15), Face(11,15,16), Face(11,16,17) };


const Mesh::Edge DPMesh::dp_edges[18]
  = { Edge(0,1),   Edge(1,2),   Edge(2,3),   Edge(3,4),
      Edge(4,5),   Edge(5,6),   Edge(6,0),   Edge(7,8),
      Edge(8,9),   Edge(9,10),  Edge(10,7),  Edge(11,12),
      Edge(12,13), Edge(13,14), Edge(14,15), Edge(15,16),
      Edge(16,17), Edge(17,11) };


/////////////////////////////////////////////////////////////////
const float PI = 4.0f*atan(1.0f);
const Point O(0,0);

const int COUNT = 5;   // not enough sides?  change this value!


/////////////////////////////////////////////////////////////////
class Client {
  public:
    Client(void);
    ~Client(void);
    void draw(double dt);
    void resize(int W, int H);
    void mouseclick(int x, int y);
    void mousedrag(int x, int y);
  private:
    GLint program,
          ucolor,
          utransform,
          aposition;
    GLuint mesh_vertex_buffer,
           mesh_face_buffer,
           mesh_edge_buffer,
           polygon_vertex_buffer,
           polygon_edge_buffer,
           triangle_vertex_buffer,
           segment_vertex_buffer;

    DPMesh mesh;
    Point mesh_center;         // center of mesh in world coordinates
    Affine ScrToDev;           // screen to NDC transform
    Vector mesh_offset;        // offset from mesh center to mouse click point
    bool dragging;             // is mesh being dragged?

    vector<Point> polygon_vertices; // pentagon object space vertices
    Affine PolyToDev;               // (current) object to world transform for pentagon
    float poly_rate;                // pentagon rotation rate

    Clip clip;                 // clipping object (pentagon, in object coords)
    vector<Point> temp_verts,
                  clip_verts;
};


Client::Client(void)
    : polygon_vertices(COUNT) {
  const char *fragment_shader_text
    = "#version 130\n\
       uniform vec3 color;\
       out vec4 frag_color;\
       void main(void) {\
         frag_color = vec4(color,1);\
       }";
  GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader,1,&fragment_shader_text,0);
  glCompileShader(fshader);

  const char *vertex_shader_text
    = "#version 130\n\
       attribute vec3 position;\
       uniform mat3 transform;\
       void main() {\
         vec3 v = transform * position;\
         gl_Position = vec4(v.x,v.y,0,1);\
       }";
  GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshader,1,&vertex_shader_text,0);
  glCompileShader(vshader);

  program = glCreateProgram();
  glAttachShader(program,fshader);
  glAttachShader(program,vshader);
  glLinkProgram(program);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  glUseProgram(program);

  ucolor = glGetUniformLocation(program,"color");
  utransform = glGetUniformLocation(program,"transform");
  aposition = glGetAttribLocation(program,"position");
  glEnableVertexAttribArray(aposition);

  // upload mesh vertices to GPU
  glGenBuffers(1,&mesh_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER,mesh_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER,mesh.vertexCount()*sizeof(Point),
               (float*)mesh.vertexArray(),GL_STATIC_DRAW);

  // upload mesh face list to GPU
  glGenBuffers(1,&mesh_face_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_face_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh.faceCount()*sizeof(Mesh::Face),
               (unsigned*)mesh.faceArray(),GL_STATIC_DRAW);

  // upload mesh edge list to GPU
  glGenBuffers(1,&mesh_edge_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_edge_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh.edgeCount()*sizeof(Mesh::Edge),
               (unsigned*)mesh.edgeArray(),GL_STATIC_DRAW);

  // upload polygon vertices to GPU
  glGenBuffers(1,&polygon_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER,polygon_vertex_buffer);
  for (int i=0; i < COUNT; ++i)
    polygon_vertices[i] = Point(cos(i*2*PI/COUNT),sin(i*2*PI/COUNT));
  glBufferData(GL_ARRAY_BUFFER,COUNT*sizeof(Point),
               (float*)&polygon_vertices[0],GL_STATIC_DRAW);

  // upload polygon edge list to GPU
  glGenBuffers(1,&polygon_edge_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,polygon_edge_buffer);
  Mesh::Edge polygon_edges[COUNT];
  for (int i=0; i < COUNT; ++i)
    polygon_edges[i] = Mesh::Edge(i,(i+1)%COUNT);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,COUNT*sizeof(Mesh::Edge),
               (unsigned*)polygon_edges,GL_STATIC_DRAW);

  // make space on GPU for triangle vertices
  glGenBuffers(1,&triangle_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER,triangle_vertex_buffer);
  Point vertices[3];
  glBufferData(GL_ARRAY_BUFFER,3*sizeof(Point),
               (float*)vertices,GL_DYNAMIC_DRAW);

  // make space on GPU for edge vertices
  glGenBuffers(1,&segment_vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER,segment_vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER,2*sizeof(Point),
               (float*)vertices,GL_DYNAMIC_DRAW);

  // clipping object
  clip = Clip(polygon_vertices);

  mesh_center = O;
  ScrToDev = scale(2.0f/700,-2.0f/700)
             * translate(Vector(-0.5f*700,-0.5f*700));
  PolyToDev = scale(0.5f);
  poly_rate = 2*PI/10.0f;
  dragging = false;
}


Client::~Client(void) {
  glUseProgram(0);
  glDeleteProgram(program);

  GLuint buffers[7]
    = { mesh_vertex_buffer, mesh_face_buffer, mesh_edge_buffer,
        polygon_vertex_buffer, polygon_edge_buffer,
        triangle_vertex_buffer, segment_vertex_buffer };
  glDeleteBuffers(7,buffers);
}


void Client::draw(double dt) {
  // clear the screen
  glClearColor(1,1,1,1);
  glClear(GL_COLOR_BUFFER_BIT);

  // draw mesh
  glUniform3f(ucolor,0,1,0);
  Affine ObjToDev = translate(mesh_center-O)
                    * scale(1.0f/mesh.dimensions().x,1.0f/mesh.dimensions().y)
                    * translate(O-mesh.center());
  glUniformMatrix3fv(utransform,1,true,(float*)&ObjToDev);
  glBindBuffer(GL_ARRAY_BUFFER,mesh_vertex_buffer);
  glVertexAttribPointer(aposition,3,GL_FLOAT,false,sizeof(Point),0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_face_buffer);
  glDrawElements(GL_TRIANGLES,3*mesh.faceCount(),GL_UNSIGNED_INT,0);
  glUniform3f(ucolor,0,0,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh_edge_buffer);
  glDrawElements(GL_LINES,2*mesh.edgeCount(),GL_UNSIGNED_INT,0);

  // compute mesh vertices in polygon object space (for clipping)
  PolyToDev = rotate(poly_rate*dt) * PolyToDev;
  Affine ObjToPoly = inverse(PolyToDev) * ObjToDev;
  temp_verts.clear();
  for (int i=0; i < mesh.vertexCount(); ++i)
    temp_verts.push_back(ObjToPoly * mesh.vertexArray()[i]);
  glUniformMatrix3fv(utransform,1,true,(float*)&PolyToDev);

  // compute and draw clipped mesh triangles
  glUniform3f(ucolor,0.9f,0.9f,0.9f);
  glBindBuffer(GL_ARRAY_BUFFER,triangle_vertex_buffer);
  glVertexAttribPointer(aposition,3,GL_FLOAT,false,sizeof(Point),0);
  for (int i=0; i < mesh.faceCount(); ++i) {
    clip_verts.clear();
    const Mesh::Face &face = mesh.faceArray()[i];
    clip_verts.push_back(temp_verts[face.index1]);
    clip_verts.push_back(temp_verts[face.index2]);
    clip_verts.push_back(temp_verts[face.index3]);
    if (clip(clip_verts)) {
      Point triangle_verts[3];
      for (unsigned n=2; n < clip_verts.size(); ++n) {
        triangle_verts[0] = clip_verts[0];
        triangle_verts[1] = clip_verts[n-1];
        triangle_verts[2] = clip_verts[n];
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(triangle_verts),
                        (float*)triangle_verts);
        glDrawArrays(GL_TRIANGLES,0,3);
      }
    }
  }

  // compute and draw clipped mesh triangle edges
  glUniform3f(ucolor,1,0,0);
  glBindBuffer(GL_ARRAY_BUFFER,segment_vertex_buffer);
  glVertexAttribPointer(aposition,3,GL_FLOAT,false,sizeof(Point),0);
  for (int i=0; i < mesh.faceCount(); ++i) {
    const Mesh::Face &face = mesh.faceArray()[i];
    const Point triangle_verts[3] = { temp_verts[face.index1],
                                      temp_verts[face.index2],
                                      temp_verts[face.index3] };
    for (int k=0; k < 3; ++k) {
      Point edge_verts[2] = { triangle_verts[k],
                              triangle_verts[(k+1)%3] };
      if (clip(edge_verts[0],edge_verts[1])) {
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(edge_verts),
                        (float*)edge_verts);
        glDrawArrays(GL_LINES,0,2);
      }
    }
  }

  // draw rotating pentagon
  glUniform3f(ucolor,0,0,0);
  glBindBuffer(GL_ARRAY_BUFFER,polygon_vertex_buffer);
  glVertexAttribPointer(aposition,3,GL_FLOAT,false,sizeof(Point),0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,polygon_edge_buffer);
  glDrawElements(GL_LINES,2*COUNT,GL_UNSIGNED_INT,0);

}


void Client::resize(int W, int H) {
  glViewport(0,0,W,H);
  ScrToDev = scale(2.0f/W,-2.0f/H)
             * translate(Vector(-0.5f*W,-0.5f*H));
}


void Client::mouseclick(int x, int y) {
  // screen to object transformation
  Affine ObjToDev = translate(mesh_center-O)
                    * scale(1.0f/mesh.dimensions().x)
                    * translate(O-mesh.center()),
         DevToObj = inverse(ObjToDev);
  // mouse click point in device/world coordinates
  Point mouse_start = ScrToDev * Point(x,y);
  // offset from mouse click point to object center
  mesh_offset = mesh_center - mouse_start;
  // convert mouse click point to object coordinates & test containment
  dragging = pointInMesh(DevToObj*mouse_start,mesh);
}


void Client::mousedrag(int x, int y) {
  if (dragging) {
    // compute new object center
    Point mouse_end = ScrToDev * Point(x,y);
    mesh_center = mouse_end + mesh_offset;
  }
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS200: Clip Demo";
  int width = 700,
      height = 700;
  SDL_Window *window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,width,height,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  // GLEW: get function bindings (if possible)
  GLenum value = glewInit();
  if (value != GLEW_OK) {
    cout << glewGetErrorString(value) << endl;
    SDL_GL_DeleteContext(context);
    SDL_Quit();
    return -1;
  }

  // animation loop
  bool done = false;
  Client *client = new Client();
  Uint32 ticks_last = SDL_GetTicks();
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          done = true;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
            done = true;
          break;
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            client->resize(event.window.data1,event.window.data2);
          break;
        case SDL_MOUSEBUTTONDOWN:
          client->mouseclick(event.button.x,event.button.y);
          break;
        case SDL_MOUSEMOTION:
          if ((event.motion.state&SDL_BUTTON_LMASK) != 0
              || (event.motion.state&SDL_BUTTON_RMASK) != 0)
            client->mousedrag(event.motion.x,event.motion.y);
          break;
      }
    }
    Uint32 ticks = SDL_GetTicks();
    double dt = 0.001*(ticks - ticks_last);
    ticks_last = ticks;
    client->draw(dt);
    SDL_GL_SwapWindow(window);
  }

  // clean up
  delete client;
  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}

