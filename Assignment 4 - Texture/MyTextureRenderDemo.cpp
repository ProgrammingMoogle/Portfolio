// MyTextureRenderDemo.cpp
// -- demo of 'MyTexturedMesh' and 'TextureRender' class,
//    uses multiple renderers, and multiple meshes
// cs200 9/17
//
// Visual Studio command prompt:
//   cl /EHsc MyTextureRenderDemo.cpp Affine.lib Render.lib TextureRender.cpp
//      SquareTexturedMesh.cpp MyTexturedMesh.cpp opengl32.lib glew32.lib
//      SLD2.lib SDL2main.lib /link /subsystem:console
//
// Linux command line:
//   g++ MyTextureRenderDemo.cpp Affine.cpp Render.cpp TextureRender.cpp
//       SquareTexturedMesh.cpp MyTexturedMesh.cpp -lGL -lGLEW -lSDL2
//
// Note: the file 'MyTexture.bmp' is assumed to exist.

#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "TextureRender.h"
#include "SquareTexturedMesh.h"
#include "MyTexturedMesh.h"
#include "Render.h"
using namespace std;


const float PI = 4.0f*atan(1.0f);
const Point O(0,0);
const Vector EX(1,0),
             EY(0,1);
const Hcoord WHITE(1,1,1),
             BLACK(0,0,0);


namespace {
  float frand(float a=0, float b=1) {
    return a + (b-a)*float(rand())/float(RAND_MAX);
  }
}


/////////////////////////////////////////////////////////////////
class NewSquareTexturedMesh : public SquareTexturedMesh {
  public:
    NewSquareTexturedMesh(const Affine& M);
    int vertexCount(void)            { return square.vertexCount(); }
    const Point* vertexArray(void)   { return square.vertexArray(); }
    Vector dimensions(void)          { return square.dimensions(); }
    Point center(void)               { return square.center(); }
    int faceCount(void)              { return square.faceCount(); }
    const Face* faceArray(void)      { return square.faceArray(); }
    int edgeCount(void)              { return square.edgeCount(); }
    const Edge* edgeArray(void)      { return square.edgeArray(); }
    const Point* texcoordArray(void) { return &texcoords[0]; }
  private:
    SquareTexturedMesh square;
    vector<Point> texcoords;
};


NewSquareTexturedMesh::NewSquareTexturedMesh(const Affine& M)
    : texcoords(square.vertexCount()) {
  for (int i=0; i < square.vertexCount(); ++i)
    texcoords[i] = M * square.vertexArray()[i];
}


/////////////////////////////////////////////////////////////////
class Client {
  public:
    Client(void);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
    void resize(int W, int H);
  private:
    double time;
    MyTexturedMesh my_mesh;
    float my_rate;
    Point my_center;
    bool rotating;
    vector<Affine> object_maps;
    vector<Affine> texture_maps;
    TextureRender *my_trender,
                  *sq_trender;
    Render *crender;
};


Client::Client(void) {
  crender = new Render();
  my_trender = new TextureRender();
  sq_trender = new TextureRender();

  // load bitmap file (first texture)
  SDL_Surface *bmp_surface = SDL_LoadBMP("MyTexture.bmp");
  if (bmp_surface == 0)
    throw runtime_error("failed to load 'MyTexture.bmp'");
  unsigned char *bmp_data = reinterpret_cast<unsigned char*>(bmp_surface->pixels);
  int bmp_width = bmp_surface->w,
      bmp_height = bmp_surface->h;
  my_trender->loadTexture(bmp_data,bmp_width,bmp_height);
  SDL_FreeSurface(bmp_surface);

  // create a bitmap (second texture)
  const int SIZE = 256;
  unsigned char *rgbdata = new unsigned char[3*SIZE*SIZE];
  for (int j=0; j < SIZE; ++j) {
    float y = (j + 0.5f)/SIZE - 0.5f;
    for (int i=0; i < SIZE; ++i) {
      float x = (i + 0.5f)/SIZE - 0.5f,
            r = sqrt(x*x + y*y);
      int index = 3*(SIZE*j + i);
      if (r < 0.45f) {
        float t = pow(r/0.45f,3);
        rgbdata[index+0] = (1-t)*255 + t*100;
        rgbdata[index+1] = t*255;
        rgbdata[index+2] = (1-t)*255 + t*100;
      }
      else {
        rgbdata[index+0] = 100;
        rgbdata[index+1] = 255;
        rgbdata[index+2] = 100;
      }
    }
  }
  sq_trender->loadTexture(rgbdata,SIZE,SIZE);
  delete[] rgbdata;

  // load mymesh
  my_trender->loadMesh(my_mesh);

  // rotating object parameters
  my_rate = 2*PI/5.0f;
  my_center = O + 0.1f*EX + 0.2f*EY;
  rotating = true;

  // modeling and texture transforms for static objects
  const int COUNT = 5;
  float d = 2.0f/COUNT;
  Affine obj2tex = rotate(0.25f*PI)
                   * scale(8.0f)
                   * translate(Vector(1,1));
  for (int i=0; i < 2*COUNT*COUNT; ++i) {
    float x = frand(-1+0.5f*d,1-0.5f*d),
          y = frand(-1+0.5f*d,1-0.5f*d);
    Affine Mo = translate(x*EX+y*EY)
                * scale(0.5f*d);
    object_maps.push_back(Mo);
    Affine Mt = obj2tex * Mo;
    texture_maps.push_back(Mt);
  }

  time = 0;
}


Client::~Client(void) {
  my_trender->unloadTexture();
  my_trender->unloadMesh();
  delete my_trender;
  sq_trender->unloadTexture();
  delete sq_trender;
  delete crender;
}


void Client::draw(double dt) {
  crender->clearFrame(WHITE);

  // render static objects
  for (unsigned i=0; i < object_maps.size(); ++i) {
    NewSquareTexturedMesh sq_mesh(texture_maps[i]);
    sq_trender->loadMesh(sq_mesh);
    sq_trender->setModelToWorld(object_maps[i]);
    sq_trender->displayFaces();
    sq_trender->unloadMesh();
    crender->loadMesh(sq_mesh);
    crender->setModelToWorld(object_maps[i]);
    crender->displayEdges(BLACK);
    crender->unloadMesh();
  }

  // render rotating objects
  crender->loadMesh(my_mesh);
  const int COUNT = 10;
  for (int i=0; i < COUNT; ++i) {
    float theta = 2*PI/COUNT,
          radius = 0.5f,
          rscale = 0.25f/max(my_mesh.dimensions().x,my_mesh.dimensions().y);
    Affine M = translate(my_center-O)
               * rotate(theta*i + my_rate*time)
               * translate(radius*EY)
               * scale(rscale)
               * translate(O-my_mesh.center());
    my_trender->setModelToWorld(M);
    my_trender->displayFaces();
    crender->setModelToWorld(M);
    crender->displayEdges(BLACK);
  }
  crender->unloadMesh();

  if (rotating)
    time += dt;
}


void Client::keypress(SDL_Keycode kc) {
  if (kc == SDLK_SPACE)
    rotating = !rotating;
}


void Client::resize(int W, int H) {
  int D = min(W,H);
  glViewport(0,0,D,D);
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  srand(unsigned(time(0)));

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS 200: MyTextureRender Demo";
  int width = 600,
      height = 600;
  SDL_Window *window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,width,height,
                                        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  // GLEW: get function bindings (if possible)
  glewInit();
  if (!GLEW_VERSION_2_0) {
    cout << "needs OpenGL version 3.0 or better" << endl;
    return -1;
  }

  // animation loop
  try {
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
            else
              client->keypress(event.key.keysym.sym);
            break;
          case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
              client->resize(event.window.data1,event.window.data2);
            break;
        }
      }
      Uint32 ticks = SDL_GetTicks();
      double dt = 0.001*(ticks - ticks_last);
      ticks_last = ticks;
      client->draw(dt);
      SDL_GL_SwapWindow(window);
    }

    delete client;
  }

  catch (exception &e) {
    cout << e.what() << endl;
  }

  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}

