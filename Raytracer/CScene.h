#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Util.h"
#include "Sphere.h"
#include "Camera.h"
#include "Intersection.h"
#include "LightSource.h"
#include "SkyInfo.h"
#include "UtilityFunctions.h"
#include <functional>


class CScene
{
public:
	CScene(int width, int height);
	void Load(const char* filename);
	inline SRGB Raytrace(int x, int y);

	inline std::vector<Sphere>& GetCurrentSpheres();
	inline LightInfo& GetLightSource();
	inline SkyInfo& GetSky();
	inline Camera& GetCamera();
private:

	int myWidth;
	int myHeight;

	// Add member variables to store scene here

	std::vector<Sphere> myCurrentSpheres;
	LightInfo myLightSource;
	SkyInfo mySky;
	Camera myCamera;
};







CScene::CScene(int width, int height) : myWidth(width), myHeight(height) {}

void CScene::Load(const char* aFilename)
{
	myCurrentSpheres.clear();
	std::ifstream file(aFilename);
	std::string str;
	while (std::getline(file, str))
	{
		const char* commentString = "//";
		if (strncmp(str.c_str(), commentString, strlen(commentString)) == 0)
			continue;

		Camera::ParseInformation(str.c_str(), myCamera);
		Sphere::ParseInformation(str.c_str(), myCurrentSpheres);
		LightInfo::ParseInformation(str.c_str(), myLightSource);
		SkyInfo::ParseInformation(str.c_str(), mySky);
	}
	Vector3f camPos = myCamera.GetPos();
	std::sort(myCurrentSpheres.begin(), myCurrentSpheres.end(), [&](Sphere aLhs, Sphere aRhs) -> bool
		{
			float lhsDist = Abs((aLhs.GetCollider().GetOrigin() - camPos).Length() + aLhs.GetCollider().GetRadius());
			float rhsDist = Abs((aRhs.GetCollider().GetOrigin() - camPos).Length() + aRhs.GetCollider().GetRadius());

			return lhsDist > rhsDist;
		});
	myCurrentSpheres;
}

SRGB CScene::Raytrace(int x, int y)
{
	// Replace this with raytracing code!

	float r = (float)x / (float)myWidth;
	float g = (float)y / (float)myHeight;
	float b = 0.2f;


	SRGB result = { 0,0,0 };

	//Create a ray from the camera
	Vector3f dir;
	Vector3f intersectionPoint;




	Vector3f camPos = myCamera.GetPos();


	float xPos = 2 * (x / (float)myWidth - 0.5f);
	float yPos = 2 * (y / (float)myHeight - 0.5f) * myHeight / (float)myWidth;



	dir = myCamera.GetForward() + (xPos * myCamera.GetRight()) + (yPos * myCamera.GetUp());

	Ray<float> ray = Ray<float>(camPos, dir);

	//Loop through each sphere in the scene and use the intersection to see if we have collided.



	Color skyColor = (1 - dir.y) * mySky.myHorizonColor + dir.y * mySky.myStraightUpColor;

	result = { skyColor.x, skyColor.y, skyColor.z };

	for (int i = 0; i < myCurrentSpheres.size(); i++)
	{

		//std::cout << "Sphere " << i << "at Pixel Pos(x" << x << ",y" << y << ") and at World Pos " << myCurrentSpheres[i].GetCollider().GetOrigin();
		if (CommonUtilities::IntersectionSphereRay(myCurrentSpheres[i].GetCollider(), ray, intersectionPoint))
		{
			Color color = myCurrentSpheres[i].TracePath(intersectionPoint, ray, myCurrentSpheres, myLightSource, mySky);
			result = { color.x, color.y, color.z };




		}
	}

	//If we have, use said object's color.

	return result;
}



inline std::vector<Sphere>& CScene::GetCurrentSpheres()
{
	return myCurrentSpheres;
}

inline LightInfo& CScene::GetLightSource()
{
	return myLightSource;
}

inline SkyInfo& CScene::GetSky()
{
	return mySky;
}

inline Camera& CScene::GetCamera()
{
	return myCamera;
}
