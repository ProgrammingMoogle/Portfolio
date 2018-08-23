/////////////////////
//
//	Kacey Lei Yeo
//	Project 1
//	CS250
//	Spring 2018
//
/////////////////////
#include <sstream>
#include <iostream>
#include <ctime>
#include "SDL2\SDL.h"
#include "GL\glew.h"
#include "GL/gl.h"
#include "include\Affine.h"
#include "include\Camera.h"
#include "include\CubeMesh.h"
#include "include\Mesh.h"
#include "include\Projection.h"
#include "include\Render.h"
#include "include\SnubDodecMesh.h"
#include <vector>
#include <utility>
using namespace std;



class Client
{
public:
	Client(SDL_Window *window);
	~Client(void);
	void draw(double dt);
	void keypress(SDL_Keycode kc);
	void resize(int W, int H);
private:
	// variables for frame rate:
	SDL_Window *window;
	int frame_count;
	double frame_time;
	// other variables:
	float time;
	// cam1 : starting cam
	// cam2 :
	// cam3 :
	Camera cam1, cam2, cam3;
	Render *rend;
	int camSwap = 0;
	std::vector<Matrix> mats;
	std::vector<Mesh*> mesh;
	std::vector<Vector> color;
	SnubDodecMesh snub;
	CubeMesh cube;
	float fov = 90;
	float ar = 1;
	Vector blue = Vector(0, 0, 1);
	Vector purple = Vector(1, 0, 1);
	Vector white = Vector(1, 1, 1);
	Vector black = Vector(0, 0, 0);
	Vector red = Vector(1, 0, 0);
	Vector greenBlue = Vector(0, 1, 1);
	Vector look_at = Vector(0, 0, 1);
	Vector rel_up = Vector(0, 1, 0);
	Point travelE = Point(2, 1, 3);
};


Client::Client(SDL_Window *win) : window(win)
{
	time = 0;
	frame_count = 0;
	frame_time = 0;
	// initialization code:
	rend = new Render();

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			Matrix mat2 = scale(2) * translate(Vector(i - (6.0f / 2.0f), -1, j - (6.0f / 2.0f))) * scale(.25);
			mats.push_back(mat2);
			mesh.push_back(&cube);
			color.push_back(purple);
		}
	}
}


Client::~Client(void)
{
	// clean-up code:
	delete rend;

}


void Client::draw(double dt)
{
	std::vector<Point> worldPoint;
	std::vector<Point> camPoint;
	std::vector<Hcoord> clipPoint;

	//   dt: time (in seconds) since last animation frame
	// frame rate:
	++frame_count;
	frame_time += dt;
	time += dt;
	if (frame_time >= 0.5)
	{
		double fps = frame_count / frame_time;
		frame_count = 0;
		frame_time = 0;
		stringstream ss;
		ss << "CS 250: Project #1 [fps=" << int(fps) << "]";
		SDL_SetWindowTitle(window, ss.str().c_str());
	}
	rend->clearBuffers(white);

	cam1 = Camera(Point(0, 5, -10), Vector(0, -1, 2), rel_up, fov, ar, .1, 20);

	Point earthPoint = Point(0, 2, 0);
	travelE = Point(sin(time) * 4, 2, cos(time) * 4);
	look_at = Vector(cos(time) * 4, 0, -sin(time) * 4);
	cam3 = Camera(travelE, earthPoint - travelE, rel_up, fov, ar, .1, 20);
	cam2 = Camera(travelE, look_at, rel_up, fov, ar, .1, 20);

	Matrix mat = scale(1) * translate(Vector(0, 2, 0)) * rotate(time, Vector(1, 2, 1));

	mats.push_back(mat);
	mesh.push_back(&snub);
	color.push_back(blue);
	//First matrix all the way to right
	Matrix matMoon = translate(Vector(0, 2, 0)) * rotate(time / 2, Vector(0, 1, 0))* translate(Vector(2, 1, 0)) * scale(.4);
	mats.push_back(matMoon);
	mesh.push_back(&snub);
	color.push_back(red);

	Matrix travelMat = cameraToWorld(cam2) * scale(.25f, .25f, .5f);
	mats.push_back(travelMat);
	mesh.push_back(&snub);
	color.push_back(greenBlue);
	// drawing code:
	for (int i = 0; i < mats.size(); i++)
	{
		Camera cam;
		switch (camSwap)
		{
		case 0:
			cam = cam1;
			break;
		case 1:
			cam = cam2;
			break;
		case 2:
			cam = cam3;
			break;
		default:
			cam = cam1;
			break;
		}

		for (int j = 0; j < mesh[i]->vertexCount(); j++)
		{
			worldPoint.push_back(mats[i] * mesh[i]->getVertex(j));
			camPoint.push_back(worldToCamera(cam) * worldPoint[j]);
			clipPoint.push_back(cameraToNDC(cam) * camPoint[j]);
		}
		//Drawing

		for (int k = 0; k < mesh[i]->faceCount(); k++)
		{
			Mesh::Face &face = mesh[i]->getFace(k);

			Point p = worldPoint[face.index1];
			Point q = worldPoint[face.index2];
			Point r = worldPoint[face.index3];

			Point a = camPoint[face.index1];
			Point b = camPoint[face.index2];
			Point c = camPoint[face.index3];

			if (a.z > 0 && b.z > 0 && c.z > 0)
			{
				continue;
			}


			Vector norm = cross(q - p, r - p);
			Vector light = p - cam.eye();

			float cp = dot(norm, light);

			if (cp > 0)
			{
				continue;
			}

			float mu = abs(dot(norm, light)) / (abs(norm) * abs(light));


			rend->setColor(mu*color[i]);
			rend->fillTriangle(clipPoint[face.index1], clipPoint[face.index2], clipPoint[face.index3]);
		}
		worldPoint.clear();
		camPoint.clear();
		clipPoint.clear();
	}
	//pop the travel
	mats.pop_back();
	mesh.pop_back();
	color.pop_back();
	//pop the moon
	mats.pop_back();
	mesh.pop_back();
	color.pop_back();
	//pop the earth
	mats.pop_back();
	mesh.pop_back();
	color.pop_back();
}


void Client::keypress(SDL_Keycode kc)
{
	// respond to keyboard input
	//   kc: SDL keycode (e.g., SDLK_SPACE, SDLK_a, SDLK_s)
	switch (kc)
	{
	case SDLK_SPACE:
		camSwap++;
		if (camSwap >= 3)
		{
			camSwap = 0;
		}
	}
}


void Client::resize(int W, int H)
{
	// respond to window resize
	//   W,H: window width and height (in pixels)
	glViewport(0, 0, W, H);
	ar = float(W) / float(H);
}


/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	srand(unsigned(time(0)));

	// SDL: initialize and create a window
	SDL_Init(SDL_INIT_VIDEO);
	const char *title = "CS250: Project #1";
	int width = 600,
		height = 600;
	SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// GLEW: get function bindings (if possible)
	GLenum value = glewInit();
	if (value != GLEW_OK)
	{
		cout << glewGetErrorString(value) << endl;
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		return -1;
	}
	// animation loop
	try
	{
		bool done = false;
		Client *client = new Client(window);
		Uint32 ticks_last = SDL_GetTicks();
		while (!done) {
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = true;
					else
						client->keypress(event.key.keysym.sym);
					break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED)
						client->resize(event.window.data1, event.window.data2);
					break;
				}
			}
			Uint32 ticks = SDL_GetTicks();
			double dt = 0.001*(ticks - ticks_last);
			ticks_last = ticks;
			client->draw(dt);
			SDL_GL_SwapWindow(window);
		}
		delete client;
	}

	catch (exception &e)
	{
		cout << e.what() << endl;
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}

