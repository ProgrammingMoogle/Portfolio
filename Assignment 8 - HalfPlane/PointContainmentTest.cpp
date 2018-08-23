// PointContainmentTest.cpp
// -- test of point containment functions
// cs200 11/17
//
// From the Visual Studio command prompt:
//   cl /EHsc PointContainmentTest.cpp Affine.lib Inverse.lib HalfPlane.lib
//      PointContainment.cpp opengl32.lib glew32.lib sdl2.lib sdl2main.lib
//      /link /subsystem:console
//
// From Linux command line:
//   g++ PointContainmentTest.cpp Affine.cpp Inverse.cpp HalfPlane.cpp
//       PointContainment.cpp -lGL -lGLEW -lSDL2

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "PointContainment.h"
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
          utransform;
    GLuint vertex_buffer,
           edge_buffer,
           face_buffer;

    Point center;     // object center
    Vector offset;    // offset from mouse click point to object center
    Affine ScrToDev;  // screen to world/device coordinates
    bool dragging;    // is object being dragged?
    DPMesh dp;
};


Client::Client(void) {
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

  glGenBuffers(1,&vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER,vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER,dp.vertexCount()*sizeof(Point),
               (float*)dp.vertexArray(),GL_STATIC_DRAW);
  GLint aposition = glGetAttribLocation(program,"position");
  glVertexAttribPointer(aposition,3,GL_FLOAT,false,sizeof(Point),0);
  glEnableVertexAttribArray(aposition);

  glGenBuffers(1,&edge_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,edge_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,dp.edgeCount()*sizeof(Mesh::Edge),
               (unsigned*)dp.edgeArray(),GL_STATIC_DRAW);

  glGenBuffers(1,&face_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,face_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,dp.faceCount()*sizeof(Mesh::Face),
               (unsigned*)dp.faceArray(),GL_STATIC_DRAW);

  center = Point(0,0);
  offset = Vector(0,0);
  ScrToDev = scale(2.0f/600,-2.0f/600)
             * translate(Vector(-0.5f*600,-0.5f*600));
  dragging = false;
}


Client::~Client(void) {
  glUseProgram(0);
  glDeleteProgram(program);

  glDeleteBuffers(1,&face_buffer);
  glDeleteBuffers(1,&edge_buffer);
  glDeleteBuffers(1,&vertex_buffer);
}


void Client::draw(double dt) {
  glClearColor(1,1,1,1);
  glClear(GL_COLOR_BUFFER_BIT);

  // draw figure
  Point O(0,0);
  Affine ObjToDev = translate(center-O)
                    * scale(1.0f/dp.dimensions().x)
                    * translate(O-dp.center());
  glUniformMatrix3fv(utransform,1,true,(float*)&ObjToDev);

  glUniform3f(ucolor,0,1,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,face_buffer);
  glDrawElements(GL_TRIANGLES,3*dp.faceCount(),GL_UNSIGNED_INT,0);

  glUniform3f(ucolor,0,0,0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,edge_buffer);
  glDrawElements(GL_LINES,2*dp.edgeCount(),GL_UNSIGNED_INT,0);
}


void Client::resize(int W, int H) {
  glViewport(0,0,W,H);
  ScrToDev = scale(2.0f/W,-2.0f/H)
             * translate(Vector(-0.5f*W,-0.5f*H));
}


void Client::mouseclick(int x, int y) {
    // screen to object transformation
    Point O(0,0);
    Affine ObjToDev = translate(center-O)
                      * scale(1.0f/dp.dimensions().x)
                      * translate(O-dp.center()),
           DevToObj = inverse(ObjToDev);
    // mouse click point in device/world coordinates
    Point mouse_start = ScrToDev * Point(x,y);
    // offset from mouse click point to object center
    offset = center - mouse_start;
    // convert mouse click point to object coordinates & test containment
    dragging = pointInMesh(DevToObj*mouse_start,dp);
}


void Client::mousedrag(int x, int y) {
  if (dragging) {
    // compute new object center
    Point mouse_end = ScrToDev * Point(x,y);
    center = mouse_end + offset;
  }
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "Point Containment Test";
  int width = 600,
      height = 600;
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
