////////////////////
// Kacey Lei Yeo
// Assignment 6
// CS200
// Fall2017
////////////////////
#include "RasterUtilities.h"

int roundUP(float num)
{
  return int(num + .5);
}


void topRight(Raster & r, const Point& p, const Point& q, float x1, float y1)
{
  float min = p.x;
  float max = q.x;
  float slope;
  float y;

  if (std::abs(x1) < 1e-8f)
  {
    slope = 0;
  }
  else
  {
    slope = y1 / x1;
  }

  min = roundUP(min);
  max = roundUP(max);

  y = p.y + slope*(min - p.x);

  for (float i = min; i < max; i++)
  {
    float j = y;
    j = roundUP(j);
    r.gotoPoint(i, j);
    r.writePixel();
    y += slope;
  }
  return;
}

void topLeft(Raster & r, const Point& p, const Point& q, float x1, float y1)
{
  float min = p.y;
  float max = q.y;
  float slope;
  float x;

  if (std::abs(y1) < 1e-8f)
  {
    slope = 0;
  }
  else
  {
    slope = x1 / y1;
  }

  min = roundUP(min);
  max = roundUP(max);

  x = p.x + slope*(p.y - min);

  for (float i = min; i < max; i++)
  {
    float j = x;
    j = roundUP(j);
    r.gotoPoint(j, i);
    r.writePixel();
    x += slope;
  }
  return;
}

void bottomLeft(Raster & r, const Point& p, const Point& q, float x1, float y1)
{
  float min = p.x;
  float max = q.x;
  float slope;
  float y;

  if (std::abs(x1) < 1e-8f)
  {
    slope = 0;
  }
  else
  {
    slope = y1 / x1;
  }

  min = roundUP(min);
  max = roundUP(max);

  y = p.y + slope*(min - p.x);

  for (float i = min; i < max; i++)
  {
    float j = y;
    j = roundUP(j);
    r.gotoPoint(i, j);
    r.writePixel();
    y -= slope;
  }
  return;
}

void bottomRight(Raster & r, const Point& p, const Point& q, float x1, float y1)
{
  float min = p.y;
  float max = q.y;
  float slope;
  float x;

  if (std::abs(y1) < 1e-8f)
  {
    slope = 0;
  }
  else
  {
    slope = x1 / y1;
  }

  min = roundUP(min);
  max = roundUP(max);

  x = p.x + slope*(p.y - min);

  for (float i = min; i < max; i++)
  {
    float j = x;
    j = roundUP(j);
    r.gotoPoint(j, i);
    r.writePixel();
    x -= slope;
  }
  return;
}


void drawLine(Raster & r, const Point & P, const Point & Q)
{
  float x1 = 0;
  float y1 = 0;

  x1 = Q.x - P.x;
  y1 = Q.y - P.y;

  if (x1 >= 0)
  {
    topRight(r, P, Q, x1, y1);
  }
  else if (y1 >= 0)
  {
    topLeft(r, P, Q, x1, y1);
  }
  else if (x1 <= 0)
  {
    bottomLeft(r, P, Q, x1, y1);
  }
  else if (y1 <= 0)
  {
    bottomRight(r, P, Q, x1, y1);
  }
  return;
}