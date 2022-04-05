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

	inline std::vector<Sphere*>& GetCurrentSpheres();
	inline LightInfo& GetLightSource();
	inline SkyInfo& GetSky();
	inline Camera& GetCamera();
private:
	//Fetches the closest sphere in a given ray.
	//Returns:
	//-A pointer to the closest sphere.
	//-The intersection point where the ray collided with the closest sphere.
	static std::tuple<Sphere*, Vector3<float>> GetClosestSphere(const Ray<float>& aRay, std::vector<Sphere*> someSpheres);


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
	Vector3f camPos = myCamera.GetPos();
	std::sort(myCurrentSpheres.begin(), myCurrentSpheres.end(), [&](Sphere* aLhs, Sphere* aRhs) -> bool
		{
			float lhsDist = Abs((aLhs->GetCollider()->GetOrigin() - camPos).Length() + aLhs->GetCollider()->GetRadius());
			float rhsDist = Abs((aRhs->GetCollider()->GetOrigin() - camPos).Length() + aRhs->GetCollider()->GetRadius());

			return lhsDist > rhsDist;
		});
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

	result = { skyColor.r, skyColor.g, skyColor.b };

	Sphere* closestSphere = nullptr;


	for (int i = 0; i < myCurrentSpheres.size(); i++)
	{






		auto collidedSphere = GetClosestSphere(ray, myCurrentSpheres);
		//std::cout << "Sphere " << i << "at Pixel Pos(x" << x << ",y" << y << ") and at World Pos " << myCurrentSpheres[i].GetCollider().GetOrigin();
		if (collidedSphere._Myfirst._Val)
		{

			int sampleAmm = 100;
			Color color;
			for (size_t i = 0; i < sampleAmm; i++)
			{
				color += collidedSphere._Myfirst._Val->TracePath(collidedSphere._Get_rest()._Myfirst._Val, ray, myCurrentSpheres, myLightSource, mySky);
			}

			auto c = color;
			result = { c.r, c.g, c.b };
		}
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


inline std::tuple<Sphere*, Vector3<float>> CScene::GetClosestSphere(const Ray<float>& aRay, std::vector<Sphere*> someSpheres)
{
	float dist = static_cast<float>(INT_MAX);
	Vector3<float> intesectPoint;
	Sphere* collidedSphere = nullptr;

	for (size_t i = 0; i < someSpheres.size(); i++)
	{
		auto colDist = collidedSphere ? collidedSphere->GetCollider()->GetOrigin() - someSpheres[i]->GetCollider()->GetOrigin() : Vector3<float>(0, 0, 0);
		if ((!collidedSphere || dist > colDist.Length()) && CommonUtilities::IntersectionSphereRay(*(someSpheres[i]->GetCollider()), aRay, intesectPoint))
		{
			dist = colDist.Length();
			collidedSphere = someSpheres[i];
		}
	}





	return std::tuple<Sphere*, Vector3<float>>(collidedSphere, intesectPoint);
}



