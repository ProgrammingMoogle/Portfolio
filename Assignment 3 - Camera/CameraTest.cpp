// CameraTest.cpp
// -- world-to-camera, normalized device coordinates
// cs200 9/17
//
// Visual Studio command prompt:
//   cl /EHsc CameraTest.cpp Affine.lib Render.lib Camera.cpp Inverse.lib SquareMesh.cpp
//      opengl32.lib glew32.lib SDL2.lib SDL2main.lib /link /subsystem:console
// Linux command line:
//   g++ CameraTest.cpp Affine.cpp Render.cpp Camera.cpp Inverse.cpp SquareMesh.cpp\
//       -lGL -lGLEW -lSDL2

#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"
#include "Render.h"
#include "SquareMesh.h"
#include "Camera.h"
using namespace std;


const float PI = 4.0f*atan(1.0f);
const Point O(0,0);
const Vector EX(1,0),
             EY(0,1);
const Hcoord BLACK(0,0,0),
             BLUE(0,0,1),
             WHITE(1,1,1),
             RED(1,0,0);


namespace {
  // random floating point numbers
  float frand(float a=0, float b=1) {
    return a + (b-a)*float(rand())/float(RAND_MAX);
  }
}


class Client {
  public:
    Client(void);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
    void resize(int W, int H);
  private:
    vector<Affine> buildings;
    vector<Hcoord> building_colors;
    Affine square_map;
    float square_rrate;
    Camera camera0,  // stationary camera
           camera1;  // moving camera
    bool moving_mode;
    float aspect;
    Render *render;
};


Client::Client(void) {
  render = new Render();
  SquareMesh square_mesh;
  render->loadMesh(square_mesh);

  for (int i=0; i < 200; ++i) {
    float w = frand(0.5f,2),
          h = frand(0,5);
    Affine M = translate(frand(-50,50)*EX + 0.5f*h*EY)
               * scale(0.5f*w,0.5f*h);
    buildings.push_back(M);
    Hcoord color(frand(),frand(),frand());
    building_colors.push_back(color);
  }

  square_map = scale(2);
  square_rrate = 2*PI/5.0f;

  aspect = 700.0f/500.0f;
  camera0 = Camera(O,EY,aspect*10,10);
  camera1 = Camera(O,EY,aspect*5,5);
}


Client::~Client(void) {
  render->unloadMesh();
  delete render;
}


void Client::draw(double dt) {
  render->clearFrame(WHITE);

  // camera transform
  Camera camera = moving_mode ? camera1 : camera0;
  Affine world2ndc = cameraToNDC(camera)
                     * worldToCamera(camera);

  // draw buildings
  for (int i=0; i < buildings.size(); ++i) {
    Affine obj2ndc = world2ndc * buildings[i];
    render->setModelToWorld(obj2ndc);
    render->displayFaces(building_colors[i]);
    render->displayEdges(BLACK);
  }

  // update and draw rotating square
  square_map = rotate(square_rrate*dt) * square_map;
  Affine obj2ndc = world2ndc * square_map;
  render->setModelToWorld(obj2ndc);
  render->displayFaces(BLUE);
  render->displayEdges(BLACK);

  // draw camera rectangle?
  if (!moving_mode) {
    obj2ndc = world2ndc
              * cameraToWorld(camera1)
              * NDCToCamera(camera1);
    render->setModelToWorld(obj2ndc);
    render->displayEdges(RED);
  }
}


void Client::keypress(SDL_Keycode kc) {
  const float distance_increment = 0.05f,
              angle_increment = 2*PI/100.0f,
              zoom_increment = 0.95f;
  switch (kc) {
    case SDLK_d:
      camera1.moveRight(distance_increment*camera1.height());
      break;
    case SDLK_a:
      camera1.moveRight(-distance_increment*camera1.height());
      break;
    case SDLK_w:
      camera1.moveUp(distance_increment*camera1.height());
      break;
    case SDLK_x:
      camera1.moveUp(-distance_increment*camera1.height());
      break;
    case SDLK_s:
      camera1.rotate(angle_increment);
      break;
    case SDLK_1:
      camera1.zoom(zoom_increment);
      break;
    case SDLK_2:
      camera1.zoom(1.0f/zoom_increment);
      break;
    case SDLK_SPACE:
      moving_mode = !moving_mode;
      break;
    default:
      camera1 = Camera(O,EY,aspect*5,5);
  }
}


void Client::resize(int W, int H) {
  glViewport(0,0,W,H);
  aspect = float(W)/float(H);
  camera0 = Camera(camera0.center(),camera0.up(),
                   aspect*camera0.height(),camera0.height());
  camera1 = Camera(camera1.center(),camera1.up(),
                   aspect*camera1.height(),camera1.height());
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  srand(unsigned(time(0)));

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS 250: Camera Test";
  int width = 700,
      height = 500;
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

