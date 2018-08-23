// Camera.h
// -- 3D camera class
// cs250 1/18

#ifndef CS250_CAMERA_H
#define CS250_CAMERA_H

#include "Affine.h"


class Camera {
  public:
    Camera(void);
    Camera(const Point& E, const Vector& look, const Vector& rel,
           float fov, float aspect, float N, float F);
    Point eye(void) const;
    Vector right(void) const;
    Vector up(void) const;
    Vector back(void) const;
    Vector viewportGeometry(void) const;
    float nearDistance(void) const;
    float farDistance(void) const;
    Camera& zoom(float factor);
    Camera& forward(float distance);
    Camera& yaw(float angle);
    Camera& pitch(float angle);
    Camera& roll(float angle);
  private:
    Point eye_point;
    Vector right_vector, up_vector, back_vector;
    float width, height, distance,
          near, far;
};


#endif

