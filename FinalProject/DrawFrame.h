//////////////////////
//
// Kacey Lei Yeo
// Final Project
// CS250
// Spring 2018
//
//////////////////////
#include "Affine.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "HalfSpace.h"
#include "Raster.h"
#include "SnubDodecMesh.h"
#include "RasterUtilities.h"
#include "CubeMesh.h"
#include "Projection.h"
#include "Clip.h"
#include "Raster.h"

class DrawFrame 
{
public:
	DrawFrame(Raster &r);
	~DrawFrame(void);
	void operator()(double dt, int cam_num);
private:
	Raster &mRaster;
	std::vector<Vector> mColor;
	std::vector<Matrix> mMats;
	std::vector<Camera> mCam;
	std::vector<Mesh*> mMesh;
	CubeMesh mCube;
	SnubDodecMesh mSnub;
	Polyhedron mPoly;

	std::vector<Point> mWorld;
	std::vector<Point> mCameraCoords;
	std::vector<Hcoord> mClip;
	std::vector<Hcoord> mDevice;
	float mFOV;
	float mRatio;
	float mDT;
	Vector mUp = Vector(0, 1, 0);
	Vector mLook = Vector(0,0,1);
	//Colors
	Vector blue = Vector(0, 0, 1);
	Vector red = Vector(1, 0, 0);
	Vector green = Vector(0, 1, 0);
	Vector purple = Vector(1, 0, 1);

};