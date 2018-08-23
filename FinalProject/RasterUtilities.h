// RasterUtilities.h
// -- clear frame/z-buffer and triangle drawing
// cs250 2/18

#ifndef CS250_RASTERUTILITIES_H
#define CS250_RASTERUTILITIES_H

#include "Raster.h"
#include "Affine.h"


void clearBuffers(Raster& r, float z=1);

void fillTriangle(Raster& r, const Hcoord& P, const Hcoord& Q, const Hcoord& R);


#endif

