/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Pipeline.h
Purpose: This file contains function prototypes of the functions
inside of the Pipeline.cpp
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#pragma once

#include "Camera.h"

class Pipeline
{
	public:
		Pipeline();

		static Point Update(const Point &p);

		static Point WorldToCamera(const Point &p);
		static Point CameraToViewport(const Point &p);

	public:
		static Camera camera;
		static int viewportWidth, viewportHeight;
};

