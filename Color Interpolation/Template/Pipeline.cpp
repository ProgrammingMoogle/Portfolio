/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Pipeline.cpp
Purpose: This file contains the functions for converting window
coordinates to viewport coordinates.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#include "Pipeline.h"

Camera Pipeline::camera = Camera();
int Pipeline::viewportWidth = 0, Pipeline::viewportHeight = 0;

Pipeline::Pipeline()
{
}

Point Pipeline::Update(const Point &p)
{
	return CameraToViewport(p);
}

Point Pipeline::WorldToCamera(const Point &p)
{
	return (camera.cameraMatrix * p);
}

Point Pipeline::CameraToViewport(const Point &p)
{
	Point worldToViewportRatio((float)viewportWidth / (float)camera.width, (float)viewportHeight / (float)camera.height);
	Point viewport(viewportWidth - 0.5f, viewportHeight - 0.5f);

	Point afterCam = WorldToCamera(p);
	Point afterScale = afterCam * worldToViewportRatio;
	Point result = Point(afterScale.x, -afterScale.y) + viewport;

	return result;
}