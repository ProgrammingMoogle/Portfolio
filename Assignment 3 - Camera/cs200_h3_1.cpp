// cs200boilerplate.cpp
// -- using SDL2 and OpenGL template
// cs200 9/17

////////////////////
// Kacey Lei Yeo
// Assignment 3
// CS200
// Fall2017
////////////////////
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"
#include "DPMesh.h"
#include "SquareMesh.h"
#include "Render.h"
using namespace std;


class Client {
  public:
    Client(void);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
    void resize(int W, int H);
    void mouseclick(int x, int y);
  private:
    // variables needed
    const float PI = 4.0f*atan(1.0f);
    Vector mouPoint;
    Hcoord white;
    Hcoord black;
    Hcoord red;
    Hcoord blue;
    float rate;
    double time;

    //Diamonds
    Render *diaRend;
    Vector diaPosition;
    float diaScale;
    float diaRot = (45 * PI/180);
    float condiaRot = (90 * PI / 180);
    SquareMesh sqMesh;
    Affine diaTrans;
    Affine diaRotation;
    Affine dianewPos;

    //Logo
    Render *lRend;
    DPMesh dpMesh;
    Vector lPosition;
    float lScale;
    float lRot = (120 * PI/180);
    Affine lTrans;
};

Client::Client(void)
{
  diaRend = new Render();
  diaRend->loadMesh(sqMesh);
  mouPoint = Vector(0, 0);
  // initialization code
  diaPosition = Vector(1, 1);
  diaScale = .25;
  diaTrans[0].x = 1;
  diaTrans[1].y = 1;
  diaRotation = rotate(condiaRot);
  

  lRend = new Render();
  lRend->loadMesh(dpMesh);
  lPosition = Vector(0, 0);
  lScale = .0625;
  lTrans[0].x = 1;
  lTrans[1].y = 1;
  time = 0;

  
  white = Hcoord(1, 1, 1);
  black = Hcoord(0, 0, 0);
  red = Hcoord(1, 0, 0);
  blue = Hcoord(0, 0, 1);
  rate = 120 * PI / 180;
}


Client::~Client(void)
{
  // clean-up code
  diaRend->unloadMesh();
  delete diaRend;

  lRend->unloadMesh();
  delete lRend;
}


void Client::draw(double dt) 
{
  dianewPos = translate(mouPoint);

  // drawing code
  //   dt: time (in seconds) since last animation frame
  diaRend->clearFrame(white);
  lRend->clearFrame(white);

  //base DP logo
  lTrans = translate(Vector(-6, -6));
  lTrans = scale(lScale) * lTrans;

  lTrans = rotate(rate*time) * lTrans;

  ////Render logo
  lRend->setModelToWorld(lTrans);
  lRend->displayEdges(black);
  lRend->displayFaces(blue);

  //Base diamond
  diaTrans = translate(diaPosition);
  diaTrans = rotate(diaRot) * diaTrans;
  diaTrans = scale(.125) * diaTrans;
  diaTrans = scale(diaScale, .5) * diaTrans;
  
  //Render diamonds
  for (int i = 0; i < 4; i++)
  {
    diaRend->setModelToWorld(diaTrans);
    diaRend->displayEdges(black);
    diaRend->displayFaces(red);
    diaTrans = diaRotation * diaTrans;
    diaTrans = dianewPos * diaTrans;
  }
  time += dt;
}


void Client::keypress(SDL_Keycode kc) {
  // respond to keyboard input
  //   kc: SDL keycode (e.g., SDLK_SPACE, SDLK_a, SDLK_s)
}


void Client::resize(int W, int H) {
  // respond to window resize
  //   W,H: window width and height (in pixels)
  glViewport(0, 0, W, H);
}


void Client::mouseclick(int x, int y) {
  // respond to mouse click
  //   (x,y): click location (in window coordinates)
  //normalize the x and y
  
  Affine temp = translate(Vector(1.0f-(.5f*499), 1.0f-(.5f*499))) * scale(2.0f / 499, -2.0f / 499);
  mouPoint = temp * Vector(x,y);
}



/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "Window Title";
  int width = 500,
      height = 500;
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
          else
            client->keypress(event.key.keysym.sym);
          break;
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            client->resize(event.window.data1,event.window.data2);
          break;
        case SDL_MOUSEBUTTONDOWN:
          client->mouseclick(event.button.x,event.button.y);
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

