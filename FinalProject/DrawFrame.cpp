//////////////////////
//
// Kacey Lei Yeo
// Final Project
// CS250
// Spring 2018
//
//////////////////////

#include "DrawFrame.h"

DrawFrame::DrawFrame(Raster &r) : mRaster(r)
{
	mDT = 0;
	mFOV = 90.0;
	mRatio = r.width() / r.height();
}

DrawFrame::~DrawFrame(void)
{
}

void DrawFrame::operator()(double dt, int cam_num)
{
	mDT += dt;
	mRaster.setColor(0, 0, 0);
	clearBuffers(mRaster, 1);

	Camera cam1 = Camera(Point(0, 5, -10), Vector(0, -1, 2), mUp, mFOV, mRatio, .1, 20);
	mCam.push_back(cam1);

	Point earthPoint = Point(0, 2, 0);
	Point travelE = Point(sin(mDT) * 4, 2, cos(mDT) * 4);

	mLook = Vector(cos(mDT) * 4, 0, -sin(mDT) * 4);

	Camera cam2 = Camera(travelE, mLook, mUp, mFOV, mRatio, .1, 20);
	mCam.push_back(cam2);

	Camera cam3 = Camera(travelE, earthPoint - travelE, mUp, mFOV, mRatio, .1, 20);
	mCam.push_back(cam3);


	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			Matrix mat2 = scale(2) * translate(Vector(i - (6.0f / 2.0f), -1, j - (6.0f / 2.0f))) * scale(.25);
			mMats.push_back(mat2);
			mMesh.push_back(&mCube);
			mColor.push_back(purple);
		}
	}

	Matrix mat = scale(1) * translate(Vector(0, 2, 0)) * rotate(mDT, Vector(1, 2, 1));
	mMats.push_back(mat);
	mMesh.push_back(&mSnub);
	mColor.push_back(blue);

	Matrix matMoon = translate(Vector(0, 2, 0)) * rotate(mDT / 2, Vector(0, 1, 0))* translate(Vector(2, 1, 0)) * scale(.4);
	mMats.push_back(matMoon);
	mMesh.push_back(&mSnub);
	mColor.push_back(red);

	//Traveling OBJ
	Matrix travelMat = cameraToWorld(mCam[1]) * scale(1, 1, 2);
	mMats.push_back(travelMat);
	mMesh.push_back(&mSnub);
	mColor.push_back(green);

	Matrix DC = scale(mRaster.width(), mRaster.height(), 1.0f)* translate(Vector(0.5f, 0.5f, 0.0f)) *scale(.5f, .5f, 1);
	Camera &cam = mCam[cam_num];
	for (int i = 0; i < mMats.size(); i++)
	{
		for (int j = 0; j < mMesh[i]->vertexCount(); j++)
		{
			mWorld.push_back(mMats[i] * mMesh[i]->getVertex(j));
			mCameraCoords.push_back(worldToCamera(cam) * mWorld[j]);
			mClip.push_back(cameraToNDC(cam) * mCameraCoords[j]);
			mDevice.push_back(DC*mClip[j]);
		}
		//Drawing

		for (int k = 0; k < mMesh[i]->faceCount(); k++)
		{
			Mesh::Face &face = mMesh[i]->getFace(k);
			Point p = mWorld[face.index1];
			Point q = mWorld[face.index2];
			Point r = mWorld[face.index3];

			Point a = mCameraCoords[face.index1];
			Point b = mCameraCoords[face.index2];
			Point c = mCameraCoords[face.index3];

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

			Clip clipCube(Polyhedron(true));

			std::vector<Hcoord> toBeclipped;

			toBeclipped.push_back(mClip[face.index1]);
			toBeclipped.push_back(mClip[face.index2]);
			toBeclipped.push_back(mClip[face.index3]);

			if (clipCube(toBeclipped))
			{
				mRaster.setColor(mu*mColor[i].x * 255, mu*mColor[i].y * 255, mu*mColor[i].z * 255);
				for (int p = 0; p < toBeclipped.size(); p++)
				{
					mDevice.push_back(DC * toBeclipped[p]);
				}

				for (int q = 1; q < toBeclipped.size(); q++)
				{
					fillTriangle(mRaster, mDevice[0], mDevice[q], mDevice[q + 1]);
				}
				mDevice.clear();
			}
		}
		mWorld.clear();
		mCameraCoords.clear();
		mClip.clear();
		mDevice.clear();
	}
	//pop the travel
	mMats.pop_back();
	mMesh.pop_back();
	mColor.pop_back();
	//pop the moon
	mMats.pop_back();
	mMesh.pop_back();
	mColor.pop_back();
	//pop the earth
	mMats.pop_back();
	mMesh.pop_back();
	mColor.pop_back();
	mCam.clear();
	//To prevent frame drops when trying to draw the cubes in the scene
	mMats.clear();
	mMesh.clear();
	mColor.clear();
}
