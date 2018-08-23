////////////////////
// Kacey Lei Yeo
// Assignment 7
// CS200
// Fall2017
////////////////////
#include "RasterUtilities.h"

void PointSort(Point &P, Point &Q, Point &R)
{
  if (P.y > Q.y)
  {
    Point& swap = P;
    P = Q;
    Q = swap;
  }

  if (Q.y > R.y)
  {
    Point& swap = Q;
    Q = R;
    R = swap;
  }

  if (P.y > Q.y)
  {
    Point& swap = P;
    P = Q;
    Q = swap;
  }
}

void fillTriangle(Raster & r, const Point & P, const Point & Q, const Point & R)
{
  Point vertex1 = P;
  Point vertex2 = Q;
  Point vertex3 = R;

  PointSort(vertex1, vertex2, vertex3);

  float inverSlope12 = (vertex1.x - vertex2.x) / (vertex1.y - vertex2.y);
  float inverSlope13 = (vertex1.x - vertex3.x) / (vertex1.y - vertex3.y);


  if (inverSlope12 < inverSlope13)
  {
    for (int j = (int)(vertex1.y + 0.5); j < vertex2.y; j++)
    {
      int xMin = vertex1.x + (inverSlope12*(j - vertex1.y));
      int xMax = vertex1.x + (inverSlope13*(j - vertex1.y));

      for (int i = (xMin + 0.5); i < xMax; i++)
      {
        r.gotoPoint(i, j);
        r.writePixel();
      }
    }

    float inverseM23 = (vertex3.x - vertex2.x) / (vertex3.y - vertex2.y);

    for (int j = (int)(vertex2.y + 0.5); j < vertex3.y; j++)
    {
      int xMin = vertex2.x + (inverSlope12*(j - vertex2.y));
      int xMax = vertex2.x + (inverseM23*(j - vertex2.y));

      for (int i = (int)(xMin + 0.5); i < xMax; i++)
      {
        r.gotoPoint(i, j);
        r.writePixel();
      }
    }
  }

  if (inverSlope13 < inverSlope12)
  {
    for (int j = (int)(vertex1.y + 0.5); j < vertex2.y; j++)
    {
      int xMin = vertex1.x + (inverSlope12*(j - vertex1.y));
      int xMax = vertex2.x + (inverSlope13*(j - vertex1.y));

      for (int i = (int)(xMin + 0.5); i < (int)(xMax + 0.5); i++)
      {
        r.gotoPoint(i, j);
        r.writePixel();
      }
    }


    for (int j = (int)(vertex2.y + 0.5); j < vertex3.y; j++)
    {
      int xMin = vertex2.x + (inverSlope12*(j - vertex2.y));
      int xMax = vertex3.x + (inverSlope13*(j - vertex2.y));

      for (int i = (int)(xMin + 0.5); i < (int)(xMax + 0.5); i++)
      {
        r.gotoPoint(i, j);
        r.writePixel();
      }
    }
  }
}
