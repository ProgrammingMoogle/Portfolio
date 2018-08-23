////////////////////
// Kacey Lei Yeo
// Project 1
// CS200
// Fall2017
////////////////////

// cs200_project1.empty.cpp
// -- template for rough draft of final project
// cs200 10/17

#include <iostream>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Affine.h"
#include "Camera.h"
#include "Mesh.h"
#include "SquareMesh.h"
#include "TexturedMesh.h"
#include "Texture.h"
#include "SquareTexturedMesh.h"
#include "Render.h"
#include "TextureRender.h"
#include "MyMesh.h"
#include "MyTexturedMesh.h"

using namespace std;

const float PI = 4.0f*atan(1.0f);
const Point origin(0, 0);
const Vector upV(1, 0);
const Vector rightV(0, 1);
float width = 600;
float height = 600;

namespace {
  float frand(float a = 0, float b = 1) {
    return a + (b - a)*float(rand()) / float(RAND_MAX);
  }
}

class Client {
public:
  Client(SDL_Window *win);
  ~Client(void);
  void draw(double dt);
  void keypress(SDL_Keycode kc);
  void resize(int W, int H);
private:
  int frame_count;
  double frame_time;
  SDL_Window *window;
  TextureRender* texRend;
  SquareTexturedMesh texSquare;
  Texture *smallTex;
  float rotateRate;
  Affine nobj2ndc;
  bool amIrotating; // is the object rotating?
  bool amIchangingCams;
  Camera cam[2];
  float ratio;
  Render *normRend;
  SquareMesh normSquare;
  double time;
  Affine obj2ndc;
  Affine objs[80];
  Texture *bigTex;
  Render *camRender;
};


Client::Client(SDL_Window *w)
  : window(w)
{
  frame_count = 0;
  frame_time = 0;
  time = 0;
  // initialization

  normRend = new Render();
  camRender = new Render();
  texRend = new TextureRender();
  smallTex = new Texture(200, 200);
  bigTex = new Texture("MyTexture.bmp");

  rotateRate = 2 * PI / 5.0f;
  Affine objTOtex = scale(.5f * 5.0f);
  nobj2ndc = translate(Vector(0.125f, 0.125f)) * scale(0.5f*1.5f);
  amIrotating = true;
  ratio = width/height;
  cam[0] = Camera(origin, rightV, ratio * 5, 5);
  //cam[1] = Camera(origin, rightV, ratio * 1, 1);

  for (unsigned i = 0; i < 80; ++i)
  {
    Affine A = translate((0.4f*(i / 16) - 0.8f)*upV + (0.14f*(i % 16) - 1)*rightV)
      * scale(0.15f, -0.2f);
    objs[i] = A;
  }

}


Client::~Client(void)
{
  // clean-up code



  delete texRend;
  delete smallTex;
  delete normRend;
  delete camRender;
  delete bigTex;
}


void Client::draw(double dt) {
  // compute and display the frame rate
  ++frame_count;
  frame_time += dt;
  if (frame_time >= 0.5)
  {
    double fps = frame_count / frame_time;
    frame_count = 0;
    frame_time = 0;
    stringstream ss;
    ss << "CS 200: Project #1 [fps=" << int(fps) << "]";
    SDL_SetWindowTitle(window, ss.str().c_str());
  }

  // drawing code here:

  //CAMERA WORK
  Camera ActionOrStation = amIchangingCams ? cam[1] : cam[0];
  Affine world2ndc = cameraToNDC(ActionOrStation) * worldToCamera(ActionOrStation);


  //MESH DRAWING
  normRend->clearFrame(Hcoord(.9f, .9f, .9f));
  normRend->loadMesh(normSquare);
  camRender->loadMesh(normSquare);
  if (!amIchangingCams)
  {
    Affine N = world2ndc * cameraToWorld(cam[1])
      * NDCToCamera(cam[1]);
    N = translate(Vector(time * 1/10,time * 1/10))* N;
    camRender->setModelToWorld(N);
    camRender->displayEdges(Hcoord(0.0f, 0.0f, 1.0f));
    camRender->unloadMesh();
  }

  //BLUE SQUARE BACKGROUND
  for (unsigned i = 0; i < 80; ++i)
  {
    Affine B = world2ndc * objs[i];
    normRend->setModelToWorld(B);
    normRend->displayFaces(Hcoord(0, 0, 1.0f));
    normRend->displayEdges(Hcoord(0, 0, 0));
  }
 
  //RED SQUARE
  Affine M = translate(normSquare.center() - origin)
    * rotate(rotateRate*time)
    * scale(0.5f*.2f);
  M = world2ndc * M;
  normRend->setModelToWorld(M);
  normRend->displayEdges(Hcoord(1.0f, 0.0f, 1.0f));
  normRend->displayFaces(Hcoord(1.0f, 0.0f, 0.0f));
  normRend->unloadMesh();


  

  //DEFAULT TEXTURE DRAWING
  texRend->loadTexture(smallTex->RGBData(), smallTex->width(), smallTex->height());
  texRend->loadMesh(texSquare);
  obj2ndc = translate(Vector(0.1f, 0.1f)) * rotate(rotateRate*time) * scale(0.5f*.2f);
  obj2ndc = world2ndc * obj2ndc;
  texRend->setModelToWorld(obj2ndc);
  texRend->displayFaces();
  texRend->unloadMesh();
  texRend->unloadTexture();

  //NONDEFAULT TEXTURE DRAWING
  texRend->loadTexture(bigTex->RGBData(), bigTex->width(), bigTex->height());
  texRend->loadMesh(texSquare);
  obj2ndc = translate(Vector(0.1f, .5f)) * rotate(rotateRate*time) * scale(0.5f*.5f);
  obj2ndc = world2ndc * obj2ndc;
  texRend->setModelToWorld(obj2ndc);
  texRend->displayFaces();
  texRend->unloadMesh();
  texRend->unloadTexture();

  time += dt;
}


void Client::keypress(SDL_Keycode kc)
{
  // respond to keyboard input
  //   kc: SDL keycode (e.g., SDLK_SPACE, SDLK_a, SDLK_s)

  switch (kc)
  {
  case SDLK_SPACE:
    amIchangingCams = !amIchangingCams;
    break;
  default:
    //cam[1] = Camera(origin, rightV, ratio * 1, 1);
    cam[0] = Camera(origin, rightV, ratio * 5, 5);
  }
}


void Client::resize(int W, int H) {
  // respond to window resize
  //   W,H: window width and height (in pixels)
  glViewport(0, 0, W, H);
  width = W;
  height = H;
  float aspect = float(width) / float(height);
  cam[0] = Camera(cam[0].center(), cam[0].up(),
    aspect*cam[0].height(), cam[0].height());

  //cam[1] = Camera(cam[1].center(), cam[1].up(),
  //  aspect*cam[1].height(), cam[1].height());
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS 200: Project #1";
  int width = 600,
    height = 600;
  SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, width, height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    Client *client = new Client(window);
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
            client->resize(event.window.data1, event.window.data2);
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

