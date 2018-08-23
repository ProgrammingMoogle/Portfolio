// Triangle Test.cpp
// -- test harness for triangle filling algorithm (y-axis up)
// cs200 10/17
//
// Using Visual Studio command prompt:
//   cl /EHsc TriangleTest.cpp Affine.lib RasterUtilities.lib Inverse.lib
//       FillTriangle.cpp opengl32.lib glew32.lib SDL2.lib SDL2main.lib
//       /link /subsystem:console
// Using Linux command line:
//   g++ TriangleTest.cpp Affine.cpp RasterUtilities.cpp Inverse.cpp
//       FillTriangle.cpp -lGL -lGLEW -lSDL2

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"
#include "Raster.h"
#include "RasterUtilities.h"
using namespace std;


class Client {
  public:
    Client(void);
    ~Client(void);
    void draw(void);
    void resize(int W, int H);
    void mousedown(int x, int y);
    void mousemove(int x, int y);
  private:
    GLint ucolor,
          utransform,
          aposition;
    GLuint program,
           segment_buffer,
           sq_vert_buffer,
           sq_edge_buffer,
           sq_face_buffer;
    Point points[2];

    int width, height;
    int cell_count_y,
        cell_count_x,
        xmin, ymin, xmax, ymax;
    int vertices[6];
    int vertex_count;
    Raster::byte *grid;
};


Client::Client(void) {
  // compile & link shaders
  const char *fragment_shader_text =
    "uniform vec3 color;\
     void main(void) {\
       gl_FragColor = vec4(color,1);\
     }";
  GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader,1,&fragment_shader_text,0);
  glCompileShader(fshader);

  const char *vertex_shader_text =
  "attribute vec2 position;\
   uniform mat3 transform;\
   void main() {\
     vec3 v = transform * vec3(position,1);\
     gl_Position = vec4(v.x,v.y,0,1);\
   }";
  GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshader,1,&vertex_shader_text,0);
  glCompileShader(vshader);

  program = glCreateProgram();
  glAttachShader(program,fshader);
  glAttachShader(program,vshader);
  glLinkProgram(program);
  glUseProgram(program);

  // get shader parameter locations
  ucolor = glGetUniformLocation(program,"color");
  utransform = glGetUniformLocation(program,"transform");
  aposition = glGetAttribLocation(program,"position");

  // create two-point vertex array buffer (!)
  glGenBuffers(1,&segment_buffer);
  glBindBuffer(GL_ARRAY_BUFFER,segment_buffer);
  glBufferData(GL_ARRAY_BUFFER,sizeof(points),(float*)points,GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(aposition);

  // array buffer for square
  glGenBuffers(1,&sq_vert_buffer);
  float sq_verts[] = { 1,1,  -1,1,  -1,-1,  1,-1 };
  glBindBuffer(GL_ARRAY_BUFFER,sq_vert_buffer);
  glBufferData(GL_ARRAY_BUFFER,sizeof(sq_verts),sq_verts,GL_STATIC_DRAW);

  // edge list buffer for square
  glGenBuffers(1,&sq_edge_buffer);
  unsigned sq_edges[] = { 0,1,  1,2,  2,3,  3,0 };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sq_edge_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(sq_edges),
               sq_edges,GL_STATIC_DRAW);

  // face list buffer for square
  glGenBuffers(1,&sq_face_buffer);
  unsigned sq_faces[] = { 0,1,2,  0,2,3 };
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sq_face_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(sq_faces),
               sq_faces,GL_STATIC_DRAW);

  // hopefully this is large enough to handle most window resizes ...
  SDL_Rect rect;
  SDL_GetDisplayBounds(0,&rect);
  grid = new Raster::byte[3*rect.w*rect.h];

  // grid and mouse stuff
  cell_count_y = 30;
  cell_count_x = cell_count_y;
  xmin = int(0.5f*600/(cell_count_x+1));
  xmax = int((cell_count_x+0.5f)*600/(cell_count_x+1));
  ymin = int(0.5f*600/(cell_count_y+1));
  ymax = int((cell_count_y+0.5f)*600/(cell_count_y+1));
  for (int i=0; i < 6; ++i)
    vertices[i] = 0;
  vertex_count = 0;
  width = 600;
  height = 600;
}


Client::~Client(void) {
  glDeleteBuffers(4,&segment_buffer);
  glUseProgram(0);
  GLuint shaders[2];
  GLsizei count;
  glGetAttachedShaders(program,2,&count,shaders);
  glDetachShader(program,shaders[0]);
  glDetachShader(program,shaders[1]);
  glDeleteProgram(program);
  delete[] grid;
}


void Client::draw(void) {
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT);

  // screen to NDC transform
  Point ScreenCenter(0.5f*width,0.5f*height),
        GridCenter(0.5f*cell_count_x,0.5f*cell_count_y),
        O(0,0);
  Affine N = scale(2.0f/width,-2.0/height) * translate(O-ScreenCenter);

  // screen to grid transform
  Affine G = translate(GridCenter-O)
             * scale(float(cell_count_x+1)/float(width),
                     -float(cell_count_y+1)/float(height))
             * translate(O-ScreenCenter),
         iG = inverse(G);

  // grid to NDC transform
  Affine NG = N * iG;

  // draw grid
  glUniform3f(ucolor,0,0,0);
  glUniformMatrix3fv(utransform,1,true,(float*)&NG);
  glBindBuffer(GL_ARRAY_BUFFER,segment_buffer);
  glVertexAttribPointer(aposition,2,GL_FLOAT,false,sizeof(Point),0);

  for (int i=0; i <= cell_count_y; ++i) {
    points[0] = O + i*Vector(0,1);
    points[1] = points[0] + cell_count_x*Vector(1,0);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(points),(float*)points);
    glDrawArrays(GL_LINES,0,2);
  }
  for (int j=0; j <= cell_count_x; ++j) {
    points[0] = O + j*Vector(1,0);
    points[1] = points[0] + cell_count_y*Vector(0,1);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(points),(float*)points);
    glDrawArrays(GL_LINES,0,2);
  }

  // draw actual edge
  glUniform3f(ucolor,0,0,1);
  if (vertex_count == 1) {
    points[0] = G * Point(vertices[0],vertices[1]);
    points[1] = G * Point(vertices[2],vertices[3]);
    glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(points),(float*)points);
    glDrawArrays(GL_LINES,0,2);
  }

  else {
    Point A[3];
    for (int i=0; i < 3; ++i)
      A[i] = G * Point(vertices[2*i],vertices[2*i+1]);
    for (int i=0; i < 3; ++i) {
      points[0] = A[i];
      points[1] = A[(i+1)%3];
      glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(points),(float*)points);
      glDrawArrays(GL_LINES,0,2);
    }

    // draw pixels on grid
    Vector u(0.4f,0),
           v(0,0.4f);
    int grid_width = cell_count_x + 1,
        grid_height = cell_count_y + 1,
        stride = 3*grid_width;
    Raster raster(grid,grid_width,grid_height,stride);
    fillRect(raster,0,0,grid_width,grid_height);
    raster.setColor(1,1,1);
    fillTriangle(raster,A[0],A[1],A[2]);

    glBindBuffer(GL_ARRAY_BUFFER,sq_vert_buffer);
    glVertexAttribPointer(aposition,2,GL_FLOAT,false,2*sizeof(float),0);
    for (int i=0; i < grid_height; ++i)
      for (int j=0; j < grid_width; ++j)
        if (grid[stride*i+3*j] != 0) {
          Affine sq2grid = translate(Vector(j,i)) * scale(0.2f),
                 sq2ndc = NG * sq2grid;
          glUniform3f(ucolor,1,0,0);
          glUniformMatrix3fv(utransform,1,true,(float*)&sq2ndc);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sq_face_buffer);
          glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
          glUniform3f(ucolor,0,0,0);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sq_edge_buffer);
          glDrawElements(GL_LINES,8,GL_UNSIGNED_INT,0);
        }
  }
}


void Client::resize(int W, int H) {
  glViewport(0,0,W,H);
  width = W;
  height = H;
  float ratio = float(width)/float(height);
  cell_count_x = int(ratio*cell_count_y);
  // grid bounds in screen space
  xmin = int(0.5f*width/(cell_count_x+1));
  xmax = int((cell_count_x+0.5f)*width/(cell_count_x+1));
  ymin = int(0.5f*height/(cell_count_y+1));
  ymax = int((cell_count_y+0.5f)*height/(cell_count_y+1));
  for (int i=0; i < 6; i+=2) {
    vertices[i] = xmin;
    vertices[i+1] = ymin;
  }
  vertex_count = 0;
}


void Client::mousedown(int x, int y) {
  if (xmin <= x && x <= xmax && ymin <= y && y <= ymax) {
    vertices[2*vertex_count] = x;
    vertices[2*vertex_count+1] = y;
    ++vertex_count;
    if (vertex_count < 3) {
      vertices[2*vertex_count] = x;
      vertices[2*vertex_count+1] = y; }
    else
      vertex_count = 0;
  }
}


void Client::mousemove(int x, int y) {
  if (xmin <= x && x <= xmax && ymin <= y && y <= ymax) {
    if (vertex_count != 0) {
      vertices[2*vertex_count] = x;
      vertices[2*vertex_count+1] = y;
    }
  }
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS 200: FillTriangle Test";
  int width = 600,
      height = 600;
  SDL_Window *window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,width,height,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  // GLEW: get function bindings (if possible)
  glewInit();
  if (!GLEW_VERSION_2_0) {
    cout << "needs OpenGL version 2.0 or better" << endl;
    return -1;
  }

  // animation loop
  bool done = false;
  Client *client = new Client();
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
          if (event.button.button == SDL_BUTTON_LEFT)
            client->mousedown(event.button.x,event.button.y);
          break;
        case SDL_MOUSEMOTION:
          client->mousemove(event.button.x,event.button.y);
          break;
      }
    }
    client->draw();
    SDL_GL_SwapWindow(window);
  }

  // clean up
  delete client;
  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}

