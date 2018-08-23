// Projection.h
// -- viewing and perspective transformations
// cs250 2/18

#ifndef CS250_PROJECTION_H
#define CS250_PROJECTION_H

#include "Camera.h"


Affine cameraToWorld(const Camera& cam);
Affine worldToCamera(const Camera& cam);
Matrix cameraToNDC(const Camera& cam);


#endif

