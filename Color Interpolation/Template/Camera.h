/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.h
Purpose: This file contains function prototypes of the functions
inside of the Camera.cpp
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#pragma once

#include "Structures.h"

class Camera
{
	public:
		Camera();

		void MoveCam(const Vector &v);
		void RotateCam(const float &angle);
		void ZoomIn();
		void ZoomOut();

		void Update();

	public:
		Point worldPosition;
		Vector u, v;
		int width, height;

		Matrix cameraMatrix;
};