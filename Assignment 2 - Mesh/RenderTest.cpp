// RenderTest.cpp
// -- simple test of 2D OpenGL rendering class
// cs200 9/17
//
// Visual Studio command prompt:
//   cl /EHsc RenderTest.cpp Render.cpp SquareMesh.cpp Affine.lib opengl32.lib
//      glew32.lib SDL2.lib SDL2main.lib /link /subsystem:console
// Linux command line:
//   g++ RenderTest.cpp Render.cpp SquareMesh.cpp Affine.cpp -lGL -lGLEW -lSDL2

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Render.h"
#include "SquareMesh.h"
using namespace std;


const float PI = 4.0f*atan(1.0f);
const Point O(0,0);
const Vector EX(1,0),
             EY(0,1);
const Hcoord BLACK(0,0,0),
             WHITE(1,1,1);


class Client {
  public:
    Client(void);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
    void resize(int W, int H);
    void mouseclick(int x, int y);
  private:
    SquareMesh square;
    float rate,
          rscale;
    Point rcenter;
    vector<Affine> objects;
    vector<Hcoord> colors;
    Render *render;
    double time;
};


Client::Client(void) {
  render = new Render();
  time = 0;

  // rotating object parameters
  rate = 2*PI/5.0f;
  rscale = 1.2f;
  rcenter = O + 0.1f*EX + 0.2f*EY;

  // modeling transforms/colors for static objects
  for (int i=0; i < 80; ++i) {
    Affine A = translate((0.4f*(i/16)-0.8f)*EX + (0.14f*(i%16)-1)*EY)
               * scale(0.15f,-0.2f);
    objects.push_back(A);
    colors.push_back(Hcoord((i%16)/16.0f,0.1f,(i/16)*0.5f));
  }
}


Client::~Client(void) {
  delete render;
}


void Client::draw(double dt) {
  render->clearFrame(WHITE);
  render->loadMesh(square);

  // render static objects
  for (unsigned i=0; i < objects.size(); ++i) {
    render->setModelToWorld(objects[i]);
    render->displayFaces(colors[i]);
    render->displayEdges(BLACK);
  }

  // rotating object modeling transform
  Affine M = translate(rcenter-O)
             * rotate(rate*time)
             * scale(0.5f*rscale);

  // render rotating object
  render->setModelToWorld(M);
  render->displayFaces(Hcoord(0,1,0));
  render->displayEdges(BLACK);

  render->unloadMesh();
  time += dt;
}


void Client::resize(int W, int H) {
  // respond to window resize
  int D = min(W,H);
  glViewport(0,0,D,D);
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "Render Class Test";
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
  try {
    Client *client = new Client();

    Uint32 ticks_last = SDL_GetTicks();
    bool done = false;
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

  // clean up
  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}

