// RasterUtilities.h
// -- rasterization functions
// cs200 10/17

#ifndef CS200_RASTERUTILITIES_H
#define CS200_RASTERUTILITIES_H

#include "Raster.h"
#include "Affine.h"


// These 2 functions are to be implemented in 'RasterUtilities.cpp':
void fillRect(Raster& r, int x, int y, int width, int height);
void drawRect(Raster& r, int x, int y, int width, int height);

// This function is to be implemented in 'DrawLine.cpp':
void drawLine(Raster& r, const Point& P, const Point& Q);

// *DO NOT* implement this function in this assignment:
void fillTriangle(Raster& r, const Point& P, const Point& Q, const Point& R);


#endif

