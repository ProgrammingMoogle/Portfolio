////////////////////
// Kacey Lei Yeo
// Assignment 6
// CS200
// Fall2017
////////////////////
#include "RasterUtilities.h"

void fillRect(Raster & r, int x, int y, int width, int height)
{
  for (int i = 0; i < height; i++)
  {
    r.gotoPoint(x, i + y);
    for (int j = 0; j < width; j++)
    {
      r.writePixel();
      r.incrementX();
    }
    r.incrementY();
  }
  return;
}

void drawRect(Raster & r, int x, int y, int width, int height)
{
  for (int i = 0; i < height; i++)
  {
    r.gotoPoint(x, i + y);
    for (int j = 0; j < width; j++)
    {
      if ((i == 0 || i == height - 1) || (j == 0 || j == width - 1))
      {
        r.writePixel();
      }
      r.incrementX();
    }
    r.incrementY();
  }
  return;
}
