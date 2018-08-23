// Raster.h
// -- elementary raster graphics
// jsh 10/17

#ifndef CS200_RASTER_H
#define CS200_RASTER_H

#include <cassert>


class Raster {
  public:
    typedef unsigned char byte;
    Raster(byte *rgb_data, int width, int height, int stride);
    void gotoPoint(int x, int y);
    void setColor(byte r, byte g, byte b);
    void writePixel(void);
    void incrementX(void);
    void decrementX(void);
    void incrementY(void);
    void decrementY(void);
  private:
    byte *buffer;
    int width, height, stride;
    int current_index, current_x, current_y;
    byte red, green, blue;
};


///////////////////////////////////////////////////////////////////////////////
// inline implementations
///////////////////////////////////////////////////////////////////////////////

#ifndef NDEBUG
#define USE_CURRENT
#endif

#ifdef CLIP_PIXELS
#define USE_CURRENT
#endif


inline
Raster::Raster(byte *d, int w, int h, int s)
    : buffer(d), width(w), height(h), stride(s),
      red(0), green(0), blue(0) {
}


inline
void Raster::gotoPoint(int x, int y) {
  #ifdef USE_CURRENT
  current_x = x;
  current_y = y;
  #endif
  current_index = y*stride + 3*x;
}


inline
void Raster::setColor(byte r, byte g, byte b) {
  red = r;
  green = g;
  blue = b;
}


inline
void Raster::writePixel(void) {
  #ifdef CLIP_PIXELS
  if (current_x < 0 || width <= current_x
      || current_y < 0 || height <= current_y)
    return;
  #endif
  #ifndef NDEBUG
  assert(0 <= current_x && current_x < width
         && 0 <= current_y && current_y < height);
  #endif
  int index = current_index;
  buffer[index] = red;
  buffer[++index] = green;
  buffer[++index] = blue;
}


inline
void Raster::incrementX(void) {
  #ifdef USE_CURRENT
  ++current_x;
  #endif
  current_index += 3;
}


inline
void Raster::decrementX(void) {
  #ifdef USE_CURRENT
  --current_x;
  #endif
  current_index -= 3;
}


inline
void Raster::incrementY(void) {
  #ifdef USE_CURRENT
  ++current_y;
  #endif
  current_index += stride;
}


inline
void Raster::decrementY(void) {
  #ifdef USE_CURRENT
  --current_y;
  #endif
  current_index -= stride;
}


#endif

