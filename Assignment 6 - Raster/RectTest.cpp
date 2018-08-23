// RectTest.cpp
// -- simple test of the Fill/DrawRect functions.
// cs200 10/17

#include <fstream>
#include <cstring>
#include <cmath>
#include "RasterUtilities.h"
using namespace std;


int main(void) {
  // set up frame buffer
  int W = 615, H = 492, S = 3*W+3;
  Raster::byte *data = new Raster::byte[S*H];
  memset(data,0,S*H);
  Raster r(data,W,H,S);

  // write bitmap file header
  char header[54];
  memset(header,0,54);
  unsigned size = S*H;
  header[0] = 'B';  header[1] = 'M';
  *reinterpret_cast<unsigned*>(header+2) = 54u + size;
  *reinterpret_cast<unsigned*>(header+10) = 54u;
  *reinterpret_cast<unsigned*>(header+14) = 40u;
  *reinterpret_cast<int*>(header+18) = W;
  *reinterpret_cast<int*>(header+22) = H;
  *reinterpret_cast<unsigned short*>(header+26) = 1u;
  *reinterpret_cast<unsigned short*>(header+28) = 24u;
  *reinterpret_cast<unsigned*>(header+34) = size;
  fstream out("RectTest.bmp",ios_base::binary|ios_base::out);
  out.write(header,54);

  // blue background (note that colors are in BGR order)
  r.setColor(255,120,100);
  fillRect(r,0,0,W,H);

  // grid of red squares with black borders
  int count = 41,
      wcount = W/count,
      hcount = H/count;
  for (int j=0; j < hcount; ++j)
    for (int i=0; i < wcount; ++i)
      if ((j%2 == 0 && i%2 == 0) || (j%2 == 1 && i%2 == 1)) {
        r.setColor(100,100,255);
        fillRect(r,i*count,j*count,count,count);
        r.setColor(0,0,0);
        drawRect(r,i*count,j*count,count,count);
      }

  // write bitmap file data
  out.write(reinterpret_cast<char*>(data),size);

  delete[] data;
  return 0;
}


