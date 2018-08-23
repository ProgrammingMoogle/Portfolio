////////////////////
// Kacey Lei Yeo
// Assignment 5
// CS200
// Fall2017
////////////////////
#include "Texture.h"
#include <fstream>
#include <stdexcept>
using namespace std;

Texture::Texture(unsigned W, unsigned H)
{

  bmp_width = W;
  bmp_height = H;

  Point UV(W, H);

  Affine trans = translate(Vector(.5f, .5f));
  Affine scaling = scale(1.0f / bmp_width, 1.0f / bmp_height);

  UV_to_uv = scaling * trans;

  Affine ToUV = scale(bmp_width, bmp_height);
  ToUV[0].w = -.5f;
  ToUV[1].w = -.5f;

  uv_to_UV = ToUV;

  ////////////////////////////////////////////////////////////////////
  // stride calculation this is for a screen this is not bitmap
  // width = 64, height = 64
  //
  // stride: 

  // min bytes = width * 3 =  64 * 3 = 192
  int min = bmp_width * 3;

  // remainder = min bytes % 4 = 192 % 4 = 48 r 0
  int remainder = min % 4;
  int padding = 0;
  if (remainder != 0)
  {
    padding = 4 - remainder;
    bmp_stride = min + padding;
  }
  // padding  = 4 - r = 4 - 0 = 4
  else
  {
    bmp_stride = min;
  }

  // stride = min bytes + padding


  ////////////////////////////////////////////////////////////////////
  // size of data 
  // 
  // size of data = stride * height
  //
  int dataSize = bmp_stride * bmp_height;

  data = new unsigned char[dataSize];

  for (int y = 0; y < abs(bmp_height); ++y)
  {
    for (int x = 0; x < bmp_width; ++x)
    {
      ////////////////////////////////////////////////////////////////////
      // Convert bitmap coordinates x and y to Texture coordinates u and v
      //
      // UVtouv(x,y)
      Point uv = UVTouv(x, y);

      ////////////////////////////////////////////////////////////////////
      // Calculating what the RGB values should be used
      // 
      // (u - 1/2)^2 + (v - 1/2)^2 
      // 
      // 
      float rgbCalc = ((uv.x - .5)*(uv.x - .5)) + ((uv.y - .5)*(uv.y - .5));

      ////////////////////////////////////////////////////////////////////
      // Calculate the offset for each point in the scan line
      // 
      // offset = bitmap coord y * stride + 3 * bitmap coord x
      //
      int index = bmp_stride*y + 3 * x;

      ////////////////////////////////////////////////////////////////////
      // if the calculations for the RGB values is greater than .16f 
      //
      // 
      //
      if (rgbCalc > .16f)
      {
        data[index + R] = 100.0f + (155.0f * uv.x);
        data[index + G] = 0;
        data[index + B] = 100.0f + (155.0f * uv.y);
      }
      else if (rgbCalc < .09f)
      {
        data[index + R] = 100.0f + (155.0f * uv.y);
        data[index + G] = 0;
        data[index + B] = 100.0f + (155.0f * uv.x);
      }
      else
      {
        data[index + R] = 0;
        data[index + G] = 255;
        data[index + B] = 0;
      }

    }
  }
}

Texture::Texture(const char * bmp_file)
{
  char header[54];

  fstream in(bmp_file, ios_base::binary | ios_base::in);
  in.read(header, 54);
  if (!in)
  {
    throw runtime_error("failed to read this file.");
  }

  if (header[0] != 'B' || header[1] != 'M') 
  {
    throw runtime_error("This isn't a bmp file.");
  }

  unsigned short bits_per_pixel = *reinterpret_cast<unsigned short*>(header + 28);
  if (bits_per_pixel != 24) 
  {
    throw runtime_error("This isn't a 24-bit pic.");
  }

  unsigned data_offset = *reinterpret_cast<unsigned*>(header + 10);
  bmp_width = *reinterpret_cast<int*>(header + 18);
  bmp_height = abs(*reinterpret_cast<int*>(header + 22));
  
  Affine trans = translate(Vector(.5f, .5f));
  Affine scaling = scale(1.0f / abs(bmp_width), 1.0f / abs(bmp_height));

  UV_to_uv = scaling * trans;

  Affine ToUV = scale(abs(bmp_width), abs(bmp_height));
  ToUV[0].w = -.5f;
  ToUV[1].w = -.5f;

  uv_to_UV = ToUV;

  unsigned data_size = *reinterpret_cast<unsigned*>(header + 34);

  int min = bmp_width * 3;

  // remainder = min bytes % 4 = 192 % 4 = 48 r 0
  int remainder = min % 4;
  int padding = 0;

  if (remainder != 0)
  {
    padding = 4 - remainder;
    bmp_stride = min + padding;
  }
  // padding  = 4 - r = 4 - 0 = 4
  else
  {
    bmp_stride = min;
  }

  if (data_size == 0)
  {
    data_size = bmp_stride * bmp_height;
  }

  data = new unsigned char[data_size];
  in.seekg(data_offset, ios_base::beg);
  in.read(reinterpret_cast<char*>(data), data_size);

  for (int j = 0; j < abs(bmp_height); ++j)
  {
    for (int i = 0; i < bmp_width; ++i)
    {
      int index = j*bmp_stride + 3 * i;

      unsigned char temp = data[index + R];
      data[index + R] = data[index + B];
      data[index + G] = data[index + G];
      data[index + B] = temp;
    }
  }

}

Texture::~Texture(void)
{
  delete[] data;
}

Hcoord Texture::uvToRGB(float u, float v)
{
  Hcoord RGBcolor;

  if (data == NULL)
  {
    return RGBcolor;
  }

  // POINTS EVERYWHERE!
  Point uv(u, v);
  
  int temp;

  /////////////////////////////////////
  // Texture Wrapping from [0,1]
  //
  // if greater than -.1f
  //
  // temp force conversion to whole
  if (uv.x < 0.0f)
  {
    temp = ceil(abs(uv.x));
    uv.x += temp;
  }
  //if greater than one
  else if (uv.x > 1)
  {
    temp = trunc(uv.x);
    uv.x -= temp;
  }
  if (uv.y < 0.0f)
  {
    temp = ceil(abs(uv.y));
    uv.y += temp;
  }
  else if (uv.y > 1)
  {
    temp = trunc(uv.y);
    uv.y -= temp;
  }

  // convert to bitmap coordinates
  Point UV = uvToUV(uv.x, uv.y);

  if (UV.x < 0)
  {
    UV.x = 0;
  }
  if (UV.y < 0)
  {
    UV.y = 0;
  }

  int index = (round(UV.y) * bmp_stride) + (3 * round(UV.x));

  RGBcolor.x = data[index + R];
  RGBcolor.y = data[index + G];
  RGBcolor.w = data[index + B];

  return RGBcolor;
}

Point Texture::UVTouv(float U, float V)
{
  Point result;
  Point UV(U, V);

  result = UV_to_uv * UV;

  return result;
}

Point Texture::uvToUV(float u, float v)
{
  Point result;
  Point uv(u, v);

  result = uv_to_UV * uv;

  return result;
}
