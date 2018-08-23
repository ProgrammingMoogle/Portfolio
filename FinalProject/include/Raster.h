// Raster.h
// -- elementary raster graphics with z-buffer
// cs250 2/18

#ifndef CS250_RASTER_H
#define CS250_RASTER_H

#include <cassert>
#include <cstring>


class Raster {
  public:
    typedef unsigned char byte;
    Raster(byte *rgb_data, float *zbuffer, int width, int height, int stride);
    void gotoPoint(int x, int y);
    void writeZ(float z);
    float getZ(void);
    void setColor(byte r, byte g, byte b);
    void writePixel(void);
    void incrementX(void);
    void decrementX(void);
    void incrementY(void);
    void decrementY(void);
    int width(void) const;
    int height(void) const;
    int stride(void) const;
  private:
    byte *frame_buffer;
    float *zbuffer;
    int frame_width, frame_height, frame_stride;
    int current_index, current_x, current_y;
    int current_indexz;
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
Raster::Raster(byte *d, float *zb, int w, int h, int s)
    : frame_buffer(d), zbuffer(zb),
      frame_width(w), frame_height(h), frame_stride(s),
      red(0), green(0), blue(0) {
}


inline
void Raster::gotoPoint(int x, int y) {
  #ifdef USE_CURRENT
  current_x = x;
  current_y = y;
  #endif
  current_index = y*frame_stride + 3*x;
  current_indexz = y*frame_width + x;
}


inline
void Raster::writeZ(float z) {
  #ifdef CLIP_PIXELS
  if (current_x < 0 || frame_width <= current_x
      || current_y < 0 || frame_height <= current_y)
    return;
  #endif
  assert(0 <= current_x && current_x < frame_width
         && 0 <= current_y && current_y < frame_height);
  zbuffer[current_indexz] = z;
}


inline
float Raster::getZ(void) {
  #ifdef CLIP_PIXELS
  if (current_x < 0 || frame_width <= current_x
      || current_y < 0 || frame_height <= current_y)
    return -1;
  #endif
  assert(0 <= current_x && current_x < frame_width
         && 0 <= current_y && current_y < frame_height);
  return zbuffer[current_indexz];
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
  if (current_x < 0 || frame_width <= current_x
      || current_y < 0 || frame_height <= current_y)
    return;
  #endif
  assert(0 <= current_x && current_x < frame_width
         && 0 <= current_y && current_y < frame_height);
  int index = current_index;
  frame_buffer[index] = red;
  frame_buffer[++index] = green;
  frame_buffer[++index] = blue;
}


inline
void Raster::incrementX(void) {
  #ifdef USE_CURRENT
  ++current_x;
  #endif
  current_index += 3;
  ++current_indexz;
}


inline
void Raster::decrementX(void) {
  #ifdef USE_CURRENT
  --current_x;
  #endif
  current_index -= 3;
  --current_indexz;
}


inline
void Raster::incrementY(void) {
  #ifdef USE_CURRENT
  ++current_y;
  #endif
  current_index += frame_stride;
  current_indexz += frame_width;
}


inline
void Raster::decrementY(void) {
  #ifdef USE_CURRENT
  --current_y;
  #endif
  current_index -= frame_stride;
  current_indexz -= frame_width;
}


inline
int Raster::width(void) const {
  return frame_width;
}


inline
int Raster::height(void) const {
  return frame_height;
}


inline
int Raster::stride(void) const {
  return frame_stride;
}


#endif

