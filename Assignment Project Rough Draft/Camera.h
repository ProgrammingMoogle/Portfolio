// Camera.h
// cs200 9/17

#ifndef CS200_CAMERA_H
#define CS200_CAMERA_H

#include "Affine.h"


class Camera {
  public:
    Camera(void);
    Camera(const Point& C, const Vector& v, float W, float H);
    Point center(void) const;
    Vector right(void) const;
    Vector up(void) const;
    float width(void) const;
    float height(void) const;
    Camera& moveRight(float x);
    Camera& moveUp(float y);
    Camera& rotate(float t);
    Camera& zoom(float f);
  private:
    Point center_point;
    Vector right_vector, up_vector;
    float rect_width, rect_height;
};


Affine cameraToWorld(const Camera& cam);
Affine worldToCamera(const Camera& cam);
Affine cameraToNDC(const Camera& cam);
Affine NDCToCamera(const Camera& cam);

#endif

