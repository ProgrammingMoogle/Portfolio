// TextureDemo.cpp
// -- texture map demo program
// cs200 9/17
//
// From Visual Studio command prompt:
//   cl /EHsc TextureDemo.cpp Affine.lib TextureRender.lib
//      SquareTexturedMesh.cpp Texture.cpp opengl32.lib glew32.lib
//      SDL2.lib SDL2main.lib /link /subsystem:console
// From Linux command line:
//   g++ TextureDemo.cpp Affine.cpp TextureRender.cpp
//       SquareTexturedMesh.cpp Texture.cpp -lGL -lGLEW -lSDL2
//
// usage:
//   TextureDemo [<bitmap>]
// where:
//   <bitmap> -- (optional) name of BMP file

#include <iostream>
#include <algorithm>
#include <vector>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Texture.h"
#include "SquareTexturedMesh.h"
#include "TextureRender.h"
using namespace std;


const float PI = 4.0f*atan(1.0f);
const Point O(0,0);
const Hcoord GRAY(0.9f,0.9f,0.9f);


/////////////////////////////////////////////////////////////////
class NewSquareTexturedMesh : public SquareTexturedMesh {
  public:
    NewSquareTexturedMesh(const Affine& M=scale(1));
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
    Client(const char *fname=0);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
    void resize(int W, int H);
  private:
    TextureRender *trender;
    Texture *std_texture,
            *mod_texture;
    double time;
    float rotation_rate;
    bool rotation;
    SquareTexturedMesh tsquare;
    Affine nobj2ndc;
    NewSquareTexturedMesh nsquare;
};


Client::Client(const char *fname) {
  trender = new TextureRender();
  std_texture = (fname == 0) ? new Texture() : new Texture(fname);
  mod_texture = new Texture(300,300);
  time = 0;
  rotation_rate = 2*PI/5.0f;
  rotation = true;
  Affine obj2tex = scale(0.5f*5.0f);
  nsquare = NewSquareTexturedMesh(obj2tex);
  nobj2ndc = translate(Vector(0.125f,0.125f))
             * scale(0.5f*1.5f);
}


Client::~Client(void) {
  delete mod_texture;
  delete std_texture;
  delete trender;
}


void Client::draw(double dt) {
  trender->clearFrame(GRAY);

  trender->loadTexture(mod_texture->RGBData(),mod_texture->width(),
                       mod_texture->height());
  trender->loadMesh(nsquare);
  trender->setModelToWorld(nobj2ndc);
  trender->displayFaces();
  trender->unloadMesh();
  trender->unloadTexture();

  trender->loadTexture(std_texture->RGBData(),std_texture->width(),
                       std_texture->height());
  trender->loadMesh(tsquare);
  Affine obj2ndc = translate(Vector(-0.1f,-0.1f))
                   * rotate(rotation_rate*time)
                   * scale(0.5f*1.2f);
  trender->setModelToWorld(obj2ndc);
  trender->displayFaces();
  trender->unloadMesh();
  trender->unloadTexture();

  if (rotation)
    time += dt;
}


void Client::keypress(SDL_Keycode kc) {
  if (kc == SDLK_SPACE)
    rotation = !rotation;
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
  const char *title = "CS 200: Texture Demo";
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
    Client *client = (argc == 2) ? new Client(argv[1]) : new Client();
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

