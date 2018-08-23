/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Shape.h
Purpose: This file contains the function prototypes for Shapes.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_5
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 03/20/2017
- End Header --------------------------------------------------------*/
#pragma once

#include "Pipeline.h"
#include "Scan Conversion.h"

class Shape
{
	public:
		Shape();

		void Update();
		void Render();

	public:
		vector <Point> points;
		vector <Point> transformed;

		Color color;

		Point position;
		float rotAngle;
		Point scale;
};