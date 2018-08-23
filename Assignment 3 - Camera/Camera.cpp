////////////////////
// Kacey Lei Yeo
// Assignment 3
// CS200
// Fall2017
////////////////////
#include "Affine.h"
#include "Camera.h"

Camera::Camera(void)
{
  center_point = Point(0, 0);
  right_vector = Vector(1, 0);
  up_vector = Vector(0, 1);
  rect_width = 2;
  rect_height = 2;
}

Camera::Camera(const Point & C, const Vector & v, float W, float H)
{
  center_point = C;
  up_vector = v;
  up_vector = normalize(v);
  right_vector = Vector(up_vector.y, up_vector.x);
  rect_width = W;
  rect_height = H;
}

Point Camera::center(void) const
{
  return center_point;
}

Vector Camera::right(void) const
{
  return right_vector;
}

Vector Camera::up(void) const
{
  return up_vector;
}

float Camera::width(void) const
{
  return rect_width;
}

float Camera::height(void) const
{
  return rect_height;
}

Camera & Camera::moveRight(float x)
{
  // TODO: insert return statement here
  center_point = center_point + (x *right());
  return *this;
}

Camera & Camera::moveUp(float y)
{
  // TODO: insert return statement here
  center_point = center_point + (y * up());
  return *this;
}

Camera & Camera::rotate(float t)
{
  Point ori(0, 0);
  Affine rot = Affine(right_vector, up_vector, ori) * ::rotate(t);
  right_vector = Vector(rot[0].x, rot[1].x);
  up_vector = Vector(rot[0].y, rot[1].y);
  return *this;
  // TODO: insert return statement here
}

Camera & Camera::zoom(float f)
{
  // TODO: insert return statement here
  rect_width = f * width();
  rect_height = f * height();
  return *this;
}

Affine cameraToWorld(const Camera & cam)
{
  Affine ctw(cam.right(), cam.up(), cam.center());
  return ctw;
}

Affine worldToCamera(const Camera & cam)
{
  Affine wtc(cam.right(), cam.up(), cam.center());
  wtc = inverse(wtc);
  return wtc;
}

Affine cameraToNDC(const Camera & cam)
{
  Affine ctn = scale(2.0f / cam.width(), 2.0f / cam.height());
  return ctn;
}

Affine NDCToCamera(const Camera & cam)
{
  Affine ntc = scale(cam.width() / 2.0f, cam.height());
  return ntc;
}
