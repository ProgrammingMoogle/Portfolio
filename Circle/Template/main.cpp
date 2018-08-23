/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: This file contains the main functions in order for this
program to run.
Language: C++ and Microsoft Compiler
Platform: Microsoft Visual Studio 2015 Community Version
Project: yeo.k_CS200_5
Author: Kacey Lei Yeo, yeo.k, 180008214
Creation date: 03/20/2017
- End Header --------------------------------------------------------*/
#include "Shape.h"

vector <Line> lines;
vector <Circle> circles;
vector <_Ellipse> ellipses;
int count_click = 1;

int type = 1;	//0 for line, 1 for circle, 2 for ellipse

int winID;

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
			break;

		case 'l':
      type = 0;
			break;

		case 'c':
      type = 1;
			break;

		case 'e':
      type = 2;
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
  static Line nLines;
  static Circle nCirlces;
  static _Ellipse nEllipse;
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
				break;
      if (state == GLUT_UP)
      {
        switch (type)
        {
        case 0:			// Line
          if (count_click == 1)
          {
            //get a point and push it back in lines
            nLines.p1.x = x;
            nLines.p1.y = y;
            count_click++;
          }
          else
          {
            nLines.p2.x = x;
            nLines.p2.y = y;
            lines.push_back(nLines);
            count_click = 1;
          }
          break;

        case 1:			// Circle
          if (count_click == 1)
          {
            //get a point and push it back in lines
            nCirlces.c.x = x;
            nCirlces.c.y = y;
            count_click++;
          }
          else
          {
            int dx = x - nCirlces.c.x;
            int dy = y - nCirlces.c.y;
            nCirlces.r = sqrt(dx*dx + dy*dy);
            circles.push_back(nCirlces);
            count_click = 1;
          }
          break;

        case 2:			// Ellipse
          if (count_click == 1)
          {
            //get a point and push it back in lines
            nEllipse.c.x = x;
            nEllipse.c.y = y;
            count_click++;
          }
          else if (count_click == 2)
          {
            int dx_1 = x - nEllipse.c.x;
            int dy_1 = y - nEllipse.c.y;
            nEllipse.a = sqrt(dx_1*dx_1 + dy_1*dy_1);
            count_click++;
          }
          else
          {
            int dx = x - nEllipse.c.x;
            int dy = y - nEllipse.c.y;
            nEllipse.b = sqrt(dx*dx + dy*dy);
            ellipses.push_back(nEllipse);
            count_click = 1;
          }
          break;
        }
        break;
      }
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

	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, FrameBuffer::buffer);
	glutSwapBuffers();
}

void init(void)
{
	FrameBuffer::Init(WIDTH, HEIGHT);
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

	return 0;
}