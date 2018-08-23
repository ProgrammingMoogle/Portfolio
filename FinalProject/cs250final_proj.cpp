// cs250final_proj.cpp
// -- driver for DrawFrame class
// cs250 3/18
//
// usage:
//   cs200final_proj [<width> <height>]
// where:
//   <width> --- (optional) width of screen
//   <height> -- (optional) height of screen
//
// To compile from the Visual Studio 2015 command prompt:
//   cl /EHsc cs250final_proj.cpp DrawFrame.cpp Affine.lib CubeMesh.lib
//      SnubDodecMesh.lib Camera.lib Projection.lib RasterUtilities.lib
//      HalfSpace.lib Clip.lib opengl32.lib glew32.lib sdl2.lib
//      sdl2main.lib /link /subsystem:console

#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "GL/gl.h"
#include "DrawFrame.h"
using namespace std;


class Client {
  public:
    Client(SDL_Window *w);
    ~Client(void);
    void draw(double dt);
    void keypress(SDL_Keycode kc);
  private:
    SDL_Window *window;
    int width,
        height;
    Raster *raster;
    Raster::byte *frame_buffer;
    float *zbuffer;
    int frame_count;
    double frame_time;
    int camera_number;
    DrawFrame *draw_frame;
};


Client::Client(SDL_Window *w)
    : window(w) {
  SDL_GetWindowSize(window,&width,&height);
  frame_buffer = new Raster::byte[3*width*height];
  zbuffer = new float[width*height];
  raster = new Raster(frame_buffer,zbuffer,width,height,3*width);
  frame_count = 0;
  frame_time = 0;
  camera_number = 0;
  draw_frame = new DrawFrame(*raster);
}


Client::~Client(void) {
  delete raster;
  delete[] zbuffer;
  delete[] frame_buffer;
  delete draw_frame;
}


void Client::draw(double dt) 
{
  // frame rate
  ++frame_count;
  frame_time += dt;
  if (frame_time >= 0.5) {
    double fps = frame_count/frame_time;
    frame_count = 0;
    frame_time = 0;
    stringstream ss;
    ss << "CS 250: Final Project [fps=" << int(fps) << "]";
    SDL_SetWindowTitle(window,ss.str().c_str());
  }

  (*draw_frame)(dt,camera_number);
  glDrawPixels(width,height,GL_RGB,GL_UNSIGNED_BYTE,frame_buffer);
}


void Client::keypress(SDL_Keycode kc) {
  if (kc == SDLK_SPACE)
    camera_number = (camera_number+1)%3;
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  // SDL: initialize and create a window
  SDL_Init(SDL_INIT_VIDEO);
  const char *title = "CS 250: Final Project";
  int width = (argc == 3) ? atoi(argv[1]) : 600,
      height = (argc == 3) ? atoi(argv[2]) : 600;
  SDL_Window *window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,width,height,
                                        SDL_WINDOW_OPENGL);
  SDL_GLContext context = SDL_GL_CreateContext(window);

  // animation loop
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

