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
#include <iomanip>






class CScene
{
public:
	inline CScene(int width, int height);
	inline void Load(const char* filename);
	inline SRGB Raytrace(int x, int y);

	inline std::vector<Sphere*>& GetCurrentSpheres();
	inline LightInfo& GetLightSource();
	inline SkyInfo& GetSky();
	inline Camera& GetCamera();

	inline static std::tuple<Sphere*, Vector3<float>> GetClosestSphere(const Ray<float>& aRay, std::vector<Sphere*> someSpheres, Sphere* aSelf = nullptr);

private:
	//Fetches the closest sphere in a given ray.
	//Returns:
	//-A pointer to the closest sphere.
	//-The intersection point where the ray collided with the closest sphere.



	int myWidth;
	int myHeight;

	// Add member variables to store scene here

	std::vector<Sphere*> myCurrentSpheres;
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


}

static int raycastCount = 0;

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

	result = { skyColor.r, skyColor.g, skyColor.b };

	Sphere* closestSphere = nullptr;


	auto collidedSphere = GetClosestSphere(ray, myCurrentSpheres);
	//std::cout << "Sphere " << i << "at Pixel Pos(x" << x << ",y" << y << ") and at World Pos " << myCurrentSpheres[i].GetCollider().GetOrigin();
	if (collidedSphere._Myfirst._Val)
	{

		int sampleAmm = 800;
		Color color;
		for (size_t i = 0; i < sampleAmm; i++)
		{
			color += collidedSphere._Myfirst._Val->TracePath(collidedSphere._Get_rest()._Myfirst._Val, ray, myCurrentSpheres, myLightSource, mySky, 4);
		}
		color /= sampleAmm;
		result = { color.r, color.g, color.b };
	}


	//If we have, use said object's color.

	return result;
}



inline std::vector<Sphere*>& CScene::GetCurrentSpheres()
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


inline std::tuple<Sphere*, Vector3<float>> CScene::GetClosestSphere(const Ray<float>& aRay, std::vector<Sphere*> someSpheres, Sphere* aSelf)
{
	Vector3<float> tempPoint;
	Vector3<float> closestPoint;
	Sphere* collidedSphere = nullptr;

	for (auto& col : someSpheres)
	{
		if (CommonUtilities::IntersectionSphereRay(*(col->GetCollider()), aRay, tempPoint))
		{
			if (!collidedSphere)
			{
				closestPoint = tempPoint;
				collidedSphere = col;
			}
			else if (Abs((tempPoint - aRay.GetOrigin()).Length()) < Abs((closestPoint - aRay.GetOrigin()).Length()))
			{
				closestPoint = tempPoint;
				collidedSphere = col;
			}
		}

	}


	return std::tuple<Sphere*, Vector3<float>>(collidedSphere, closestPoint);
}



