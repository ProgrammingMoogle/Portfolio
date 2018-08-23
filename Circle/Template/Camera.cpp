/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: This file contains Camera functions.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_5
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 03/20/2017
- End Header --------------------------------------------------------*/
#include "Camera.h"

Camera::Camera()
{
}

void Camera::MoveCam(const Vector &v)
{
	worldPosition = worldPosition + v;
}

void Camera::RotateCam(const float &angle)
{
	Matrix r = Rotate(angle);

	u = r * u;
	v = r * v;
}

void Camera::ZoomIn()
{
	width /= 2;
	height /= 2;
}

void Camera::ZoomOut()
{
	width *= 2;
	height *= 2;
}

void Camera::Update()
{
	Matrix tr = Translate(-worldPosition);
	Matrix r;

	r.m[0][0] = u.x;	r.m[0][1] = u.y;	r.m[0][2] = 0.0f;
	r.m[1][0] = v.x;	r.m[1][1] = v.y;	r.m[1][2] = 0.0f;
	r.m[2][0] = 0.0f;	r.m[2][1] = 0.0f;	r.m[2][2] = 1.0f;

	cameraMatrix = r * tr;
}