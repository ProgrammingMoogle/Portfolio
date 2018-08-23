/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Utilities.h
Purpose: This file contains function prototypes of the functions
inside of the Utilities.h
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#pragma once

#include "windows.h"
#include "GL/glut.h"
#include "math.h"
#include "stdio.h"
#include <vector>

using namespace std;

#define WIDTH 640
#define HEIGHT 480

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180.0)

class FrameBuffer
{
	public:
		FrameBuffer();
		~FrameBuffer();

		static void Clear(const unsigned char &r, const unsigned char &g, const unsigned char &b);

		static void Init(const unsigned int &w, const unsigned int &h);
		static void SetPixel(const int &x, const int &y, const unsigned char &r, const unsigned char &g, const unsigned char &b);
		static void GetPixel(const int &x, const int &y, unsigned char &r, unsigned char &g, unsigned char &b);

	public:
		static unsigned char *buffer;
		static int width;
		static int height;
};

int Ceiling(const float &p);
int Floor(const float &p);
int Truncate(const float &p);
int Round(const float &p);

unsigned char Clamp(float f);

unsigned char *CreateChessBoardImage();