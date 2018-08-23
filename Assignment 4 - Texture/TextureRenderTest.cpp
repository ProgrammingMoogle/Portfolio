// TextureRenderTest.cpp
// -- simple test of 'TextureRender' class
// cs200 9/17
//
// Visual Studio command prompt:
//   cl /EHsc TextureRenderTest.cpp Affine.lib SquareMesh.cpp TextureRender.cpp
//      opengl32.lib glew32.lib SDL2.lib SDL2main.lib /link /subsystem:console
//
// Linux command line:
//   g++ TextureRenderTest.cpp Affine.cpp SquareMesh.cpp TextureRender.cpp\
//       -lGL -lGLEW -lSDL2
//
// Note: the file 'texture.bmp' is assumed to exist.

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "TextureRender.h"
#include "SquareMesh.h"
using namespace std;


const float PI = 4.0f*atan(1.0f);
const Point O(0,0);
const Vector EX(1,0),
             EY(0,1);
const Hcoord BLUISH(0.4f,0.3f,0.7f);


/////////////////////////////////////////////////////////////////
class TexturedSquare : public TexturedMesh {
  public:
    TexturedSquare(const Affine &M);
    int vertexCount(void)          { return square.vertexCount(); }
    const Point* vertexArray(void) { return square.vertexArray(); }
    Vector dimensions(void)        { return square.dimensions(); }
    Point center(void)             { return square.center(); }
    int faceCount(void)            { return square.faceCount(); }
    const Face* faceArray(void)    { return square.faceArray(); }
    int edgeCount(void)            { return square.edgeCount(); }
    const Edge* edgeArray(void)    { return square.edgeArray(); }
    const Point* texcoordArray(void);
  private:
    SquareMesh square;
    Point texcoords[4];
};


TexturedSquare::TexturedSquare(const Affine &M = scale(1)) {
  for (int i=0; i < vertexCount(); ++i)
    texcoords[i] = M * vertexArray()[i];
}


const Point* TexturedSquare::texcoordArray(void) {
  return texcoords;
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
    bool rotating;
    float rate,
          rscale;
    Point center;
    TexturedSquare sq_mesh1,
                   sq_mesh2;
    vector<Affine> object_maps;
    TextureRender *trender;
};


Client::Client(void) {
  trender = new TextureRender();

  // load bitmap file
  SDL_Surface *bmp_surface = SDL_LoadBMP("texture.bmp");
  if (bmp_surface == 0)
    throw runtime_error("failed to load 'texture.bmp'");
  unsigned char *bmp_data = reinterpret_cast<unsigned char*>(bmp_surface->pixels);
  int bmp_width = bmp_surface->w,
      bmp_height = bmp_surface->h;
  trender->loadTexture(bmp_data,bmp_width,bmp_height);

  // textured square mesh for static objects
  Affine Mt = translate(0.5f*EX+0.5f*EY)
              * scale(0.5f);
  sq_mesh1 = TexturedSquare(Mt);

  // textured square mesh for rotating object
  Mt = rotate(0.25f*PI)
       * scale(1.5f);
  sq_mesh2 = TexturedSquare(Mt);

  // rotating object parameters
  rate = -2*PI/8.0f;
  rscale = 1.25f/sqrt(2.0f);
  center = O;

  // modeling transforms for static objects
  const int COUNT = 12;
  for (int i=0; i < COUNT; ++i) {
    Affine Mo = rotate(2*PI*i/COUNT)
                * translate(0.8f*EX)
                * scale(0.15f);
    object_maps.push_back(Mo);
  }

  time = 0;
  rotating = true;
}


Client::~Client(void) {
  trender->unloadTexture();
  delete trender;
}


void Client::draw(double dt) {
  trender->clearFrame(BLUISH);

  // render static objects
  trender->loadMesh(sq_mesh1);
  for (unsigned i=0; i < object_maps.size(); ++i) {
    trender->setModelToWorld(object_maps[i]);
    trender->displayFaces();
  }
  trender->unloadMesh();

  // rotating object modeling transform
  Affine M = translate(center-O)
             * rotate(rate*time)
             * scale(0.5f*rscale);

  // render rotating object
  trender->loadMesh(sq_mesh2);
  trender->setModelToWorld(M);
  trender->displayFaces();
  trender->unloadMesh();

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

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS 200: TextureRender Test";
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

