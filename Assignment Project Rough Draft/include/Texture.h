// Texture.h
// -- bitmaps and texture coordinates
// cs200 10/17

#ifndef CS200_TEXTURE_H
#define CS200_TEXTURE_H


#include "Affine.h"


class Texture {
  public:
    explicit Texture(unsigned W=64, unsigned H=64);
    Texture(const char *bmp_file);
    ~Texture(void);
    unsigned char* RGBData(void) { return data; }
    unsigned width(void)         { return bmp_width; }
    unsigned height(void)        { return bmp_height; }
    unsigned stride(void)        { return bmp_stride; }
    Hcoord uvToRGB(float u, float v);
    Point UVTouv(float U, float V);
    Point uvToUV(float u, float v);
    enum { R=0, G=1, B=2 };
  private:
    unsigned char *data;
    int bmp_width, bmp_height, bmp_stride;
    Affine uv_to_UV, UV_to_uv;
};


#endif

