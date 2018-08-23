/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: This file contains the main functions in order for this
program to run.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_7
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 04/18/2017
- End Header --------------------------------------------------------*/
#include "Shape.h"

vector <Line> lines;
vector <Circle> circles;
vector <Ellipses> ellipses;
vector <Triangle> triangles;
vector <Quad> quads;

Line templ;
Circle tempc;
Ellipses tempe;
Triangle tempt;
Quad tempq;

int colorCombineMode = 0;	//0: Color, 1: Decal, 2: Modulate, 3: Replace

Point tempp;
int gCount = 0;

int type = 4;	//0 for line, 1 for circle, 2 for ellipse, 3 for triangle, 4 for quad

int winID;

unsigned char *pTexture = nullptr;

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case VK_ESCAPE:
			glutDestroyWindow(winID);
			exit(0);
			break;

		case 'r':
			lines.clear();
			circles.clear();
			ellipses.clear();
			triangles.clear();
			quads.clear();
			break;

		case 'l':
			type = 0;
			gCount = 0;
			break;

		case 'c':
			type = 1;
			gCount = 0;
			break;

		case 'e':
			type = 2;
			gCount = 0;
			break;

		case 't':
			type = 3;
			gCount = 0;
			break;

		case 'q':
			type = 4;
			gCount = 0;
			break;

		case '0':
			colorCombineMode = 0;
			break;

		case '1':
			colorCombineMode = 1;
			break;

		case '2':
			colorCombineMode = 2;
			break;

		case '3':
			colorCombineMode = 3;
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
				break;

			switch(type)
			{
				case 0: //line
					if (gCount == 0)
					{
						templ.p1 = Point(x, y);
						gCount++;
					}
					else
					if (gCount == 1)
					{
						lines.push_back(Line());
						lines[lines.size() - 1].p1 = templ.p1;
						lines[lines.size() - 1].p2 = Point(x, y);
						gCount = 0;
					}
					break;

				case 1: //circle
					if (gCount == 0)
					{
						tempc.c = Point(x, y);
						gCount++;
					}
					else
					if (gCount == 1)
					{
						circles.push_back(Circle());
						circles[circles.size() - 1].c = tempc.c;
						circles[circles.size() - 1].r = Length(Point(x, y) - tempc.c);
						gCount = 0;
					}
					break;

				case 2: //ellipse
					if (gCount == 0)
					{
						tempe.c = Point(x, y);
						gCount++;
					}
					else
					if (gCount == 1)
					{
						tempe.a = Length(Point(x, y) - tempe.c);
						gCount++;
					}
					else
					if (gCount == 2)
					{
						ellipses.push_back(Ellipses());
						ellipses[ellipses.size() - 1].c = tempe.c;
						ellipses[ellipses.size() - 1].a = tempe.a;
						ellipses[ellipses.size() - 1].b = Length(Point(x, y) - tempe.c);
						gCount = 0;
					}
					break;

				case 3: //triangle
					if (gCount == 0)
					{
						tempt.p1 = Point(x, y, Color(255,0,0), 0.0f, 0.0f);
						gCount++;
					}
					else
					if (gCount == 1)
					{
						tempt.p2 = Point(x, y, Color(0,255,0), 0.0f, 1.0f);
						gCount++;
					}
					else
					if (gCount == 2)
					{
						triangles.push_back(Triangle());
						triangles[triangles.size() - 1].p1 = tempt.p1;
						triangles[triangles.size() - 1].p2 = tempt.p2;
						triangles[triangles.size() - 1].p3 = Point(x, y, Color(0,0,255), 1.0f, 1.0f);
						gCount = 0;
					}
					break;

				case 4: //quad
					if (gCount == 0)
					{
						tempq.p1 = Point(x, y, Color(255,0,0), 0.0f, 0.0f);
						gCount++;
					}
					else
					if (gCount == 1)
					{
						tempq.p2 = Point(x, y, Color(0,255,0), 0.0f, 1.0f);
						gCount++;
					}
					else
					if (gCount == 2)
					{
						tempq.p3 = Point(x, y, Color(0,0,255), 1.0f, 1.0f);
						gCount++;
					}
					else
					if (gCount == 3)
					{
						quads.push_back(Quad());
						quads[quads.size() - 1].p1 = tempq.p1;
						quads[quads.size() - 1].p2 = tempq.p2;
						quads[quads.size() - 1].p3 = tempq.p3;
						quads[quads.size() - 1].p4 = Point(x, y, Color(128,128,128), 1.0f, 0.0f);
						gCount = 0;
					}
					break;
			}
			break;
	}
}

void mouseMove(int x, int y)
{
}

void loop(void)
{
	glutPostRedisplay();
}

void render(void)
{
	FrameBuffer::Clear(255, 255, 255);

	//
	for (int i=0; i<lines.size(); ++i)
		DrawMidpointLine(lines[i].p1, lines[i].p2, 0, 0, 0);
	for (int i=0; i<circles.size(); ++i)
		DrawMidpointCircle(circles[i].c, circles[i].r, 0, 0, 0);
	for (int i=0; i<ellipses.size(); ++i)
		DrawMidpointEllipse(ellipses[i].c, ellipses[i].a, ellipses[i].b, 0, 0, 0);
	for (int i=0; i<triangles.size(); ++i)
		DrawTriangleTopLeftFillRule(triangles[i].p1, triangles[i].p2, triangles[i].p3, pTexture, colorCombineMode);
	for (int i=0; i<quads.size(); ++i)
	{
		DrawTriangleTopLeftFillRule(quads[i].p1, quads[i].p2, quads[i].p3, pTexture, colorCombineMode);
		DrawTriangleTopLeftFillRule(quads[i].p1, quads[i].p3, quads[i].p4, pTexture, colorCombineMode);
	}

	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, FrameBuffer::buffer);
	glutSwapBuffers();
}

void init(void)
{
	FrameBuffer::Init(WIDTH, HEIGHT);
	// Create a signle texture
	pTexture = CreateChessBoardImage();
}

void free(void)
{
	if (nullptr != pTexture)
		delete[] pTexture;
}

int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	winID = glutCreateWindow("CS200");

	glClearColor(0, 0, 0, 1);

	glutKeyboardFunc(keyboard);
	glutDisplayFunc(render);
	glutIdleFunc(loop);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);

	init();
	glutMainLoop();
	free();

	return 0;
}