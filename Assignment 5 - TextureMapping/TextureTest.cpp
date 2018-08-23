// TextureTest.cpp
// -- simple test of Texture class
// cs200 9/17
//
// usage:
//   TextureTest [<file>]
// where:
//   <file> -- (optional) name of BMP file (uncompressed 24 bit color)
// output:
//   'TextureTest.bmp'

#include <iostream>
#include <fstream>
#include <cstring>
#include "Texture.h"
using namespace std;



int main(int argc, char *argv[]) {
  if (argc != 1 && argc != 2)
    return 0;

  Texture *texture_in = 0;
  if (argc == 2) {
    try {
      texture_in = new Texture(argv[1]);
    }
    catch (...) {
      cout << "failed to load '" << argv[1] << "'" << endl;
      cout << "using default texture" << endl;
    }
  }
  if (texture_in == 0)
    texture_in = new Texture(128,128);

  Texture *texture_out = new Texture(500,500);
  float replication = 4.5,
        aspect = float(texture_in->width())/float(texture_in->height()),
        replicationx = replication,
        replicationy = replicationx*aspect,
        offsetx = 0.5f*fmod(replicationx,1.0f),
        offsety = 0.5f*fmod(replicationy,1.0f);
  Affine A = translate(Vector(-offsetx,-offsety))
             * scale(replicationx,replicationy);
  for (int j=0; j < texture_out->height(); ++j) {
    for (int i=0; i < texture_out->width(); ++i) {
      Point uv = texture_out->UVTouv(i,j);
      uv = A * uv;
      Hcoord color = texture_in->uvToRGB(uv.x,uv.y);
      int index = texture_out->stride()*j + 3*i;
      for (int k=0; k < 3; ++k)
        texture_out->RGBData()[index+k] = (unsigned char)(color[2-k]);
    }
  }

  fstream out("TextureTest.bmp",ios_base::out|ios_base::binary);
  char header[54];
  memset(header,0,54);
  strncpy(header,"BM",2);
  unsigned size = texture_out->height() * texture_out->stride();
  *reinterpret_cast<unsigned*>(header+2) = 54u + size;
  *reinterpret_cast<unsigned*>(header+10) = 54;
  *reinterpret_cast<unsigned*>(header+14) = 40;
  *reinterpret_cast<int*>(header+18) = texture_out->width();
  *reinterpret_cast<int*>(header+22) = texture_out->height();
  *reinterpret_cast<unsigned short*>(header+26) = 1;
  *reinterpret_cast<unsigned short*>(header+28) = 24;
  *reinterpret_cast<unsigned*>(header+34) = size;
  out.write(header,54);
  out.write(reinterpret_cast<char*>(texture_out->RGBData()),size);

  delete texture_out;
  delete texture_in;
  return 0;
}

