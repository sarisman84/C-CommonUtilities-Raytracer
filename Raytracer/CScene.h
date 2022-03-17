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
	CScene(int width, int height);
	void Load(const char* filename);
	inline SRGB Raytrace(int x, int y);

	inline std::vector<Sphere>& GetCurrentSpheres();
	inline LightInfo& GetLightSource();
	inline SkyInfo& GetSky();
	inline Camera& GetCamera();
	inline Vector3<float> RandomUnitVector();
private:

	Color RecursiveRay(Vector3f& aIntersectionPoint, int aCurrentBounce = 4);

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

int raycastCount = 0;

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
			raycastCount++;
			//Slumpmässig stråle i en random riktning
			//Result blir summan av de två colors alltså result = color + randomColor
			Color randColor;

			for (int i = 0; i < 100; i++)
			{
				randColor += RecursiveRay(intersectionPoint);
			}
			
			std::cout << "\r" << std::setw(2)  << std::setfill('0') << "Raycast checks thus far: " << std::setw(2) << raycastCount << " " << std::flush;
			result = { color.x + randColor.x, color.y + randColor.y, color.z + randColor.z };
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

inline Vector3<float> CScene::RandomUnitVector() //Från Föreläsningen LA09 - Pathtracing (=
{
	float z = RandomFloat() * 2.0f - 1.0f;
	float a = RandomFloat() * 2.0f - std::_Pi;
	float r = sqrtf(1.0f - z * z);
	float x = r * cosf(a);
	float y = r * sinf(a);
	return Vector3<float>(x, y, z);
}

Color CScene::RecursiveRay(Vector3f& aIntersectionPoint, int aCurrentBounce)
{
	raycastCount++;
	Color result;

	Vector3f randomDir = RandomUnitVector();

	Ray<float> ray = Ray<float>(aIntersectionPoint, randomDir);

	for (int i = 0; i < myCurrentSpheres.size(); i++)
	{

		//std::cout << "Sphere " << i << "at Pixel Pos(x" << x << ",y" << y << ") and at World Pos " << myCurrentSpheres[i].GetCollider().GetOrigin();
		if (CommonUtilities::IntersectionSphereRay(myCurrentSpheres[i].GetCollider(), ray, aIntersectionPoint))
		{
			Color color = myCurrentSpheres[i].TracePath(aIntersectionPoint, ray, myCurrentSpheres, myLightSource, mySky);

			result = { color.x, color.y, color.z };
		}
	}

	if (aCurrentBounce >= 0)
	{
		Color otherResult = RecursiveRay(aIntersectionPoint , aCurrentBounce - 1);
		result = { result.x * otherResult.x, result.y * otherResult.y, result.z * otherResult.z };
	}

	return result;
}