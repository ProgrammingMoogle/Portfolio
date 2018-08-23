/////////////////////
//
//	Kacey Lei Yeo
//	Assignment 4
//	CS250
//	Spring 2018
//
/////////////////////
#include "Camera.h"
#include "Affine.h"
#include <iostream>

Camera::Camera(void)
{
    eye_point = Point(0, 0, 0);
    up_vector = Vector(0, 1, 0);
    near = .1f;
    far = 10;
    back_vector = Vector(0, 0, -1);
    width = 1;
    height = 1;
    right_vector = Vector(1, 0, 0);
}

Camera::Camera(const Point & E, const Vector & look, const Vector & rel, float fov, float aspect, float N, float F)
{
    float backlook = abs(look);
    Vector temp;

    eye_point = E;
    near = N;
    far = F;
    back_vector = -(1 / backlook) * look;
    temp = cross(look, rel);
    right_vector = (1 / abs(temp)) * temp;
    up_vector = cross(back_vector, right_vector);
    distance = N;
    width = (2 * distance) * tan(fov / 2);
    height = width / aspect;
}

Point Camera::eye(void) const
{
    return eye_point;
}

Vector Camera::right(void) const
{
    return right_vector;
}

Vector Camera::up(void) const
{
    return up_vector;
}

Vector Camera::back(void) const
{
    return back_vector;
}

Vector Camera::viewportGeometry(void) const
{
  float nearW = width;
  float nearH = height;

  float farW = nearW / near*far;
  float farH = nearH / near*far;

  Point pt2 = Point(farW, farH, -far);
  Point pt1 = Point(nearW, nearH, -near);
  Vector result = pt2 - pt1;


  return result;
}

float Camera::nearDistance(void) const
{
    return near;
}

float Camera::farDistance(void) const
{
    return far;
}

Camera & Camera::zoom(float factor)
{

    height *= factor;
    width *= factor;
    return *this;
}

Camera & Camera::forward(float distance)
{
    eye_point = eye_point - (distance*back_vector);

    return *this;
}

Camera & Camera::yaw(float angle)
{
    right_vector = rotate(angle, up_vector) * right_vector;
    back_vector = rotate(angle, up_vector) * back_vector;
    return *this;
}

Camera & Camera::pitch(float angle)
{


    up_vector = rotate(angle, right_vector) *up_vector;
    back_vector = rotate(angle, right_vector) *back_vector;
    return *this;
}

Camera & Camera::roll(float angle)
{


    up_vector = rotate(angle, back_vector) *up_vector;
    right_vector = rotate(angle, back_vector) * right_vector;
    return *this;
}
