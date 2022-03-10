#include "Sphere.h"
#include "LightSource.h"
#include "Intersection.h"
#include "SkyInfo.h"
#include "UtilityFunctions.h"

Sphere::Sphere(const Vector3<float> aCenter, const float aRadius, const Color aColor, const bool anIsSpecularFlag)
{
	myCollider = SphereCollider<float>(aCenter, aRadius);
	myColor = aColor;
	myIsSpecularFlag = anIsSpecularFlag;
}

SphereCollider<float> Sphere::GetCollider() const
{
	return myCollider;
}

Color Sphere::GetColor() const
{
	return myColor;
}

const bool Sphere::IsSpecular() const
{
	return myIsSpecularFlag;
}

Color Sphere::TracePath(Vector3<float> anIntersectionPoint, Ray<float> aRay, std::vector<Sphere>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aRecursionDepth)
{
	if (aRecursionDepth <= 0)
		return { 0,0,0 };
	Color result;

	//Varje gång en stråle träffar en diffus sfär
	//skapa en stråle mot ljuskällan
	//Kolla om det är något i vägen
	//Om inte, räkna ut och returnera C-diff
	//  c-diff = (s diff X m diff) * (sphereNormal dot lightDir)
	//	s diff = ljusets färg
	//	m diff = sphärens färg
	//Annars, ränka ut ett skugga



	const auto& lightSource = someLightInfo;
	Vector3<float> sphereNormal = GetNormal(anIntersectionPoint);


	if (myIsSpecularFlag)
	{
		// (V = kamerans direction) (N = normal av punkten där vi träffar.) (R = reflektions vektorn)
		// (R = 2Vo NN - V)

		Vector3<float> curDir = aRay.GetDirection(); //Also known as V

		Vector3<float> reflectedDir = curDir - (2.0f * (curDir.Dot(sphereNormal) * sphereNormal));

		aRay.InitWithOriginAndDirection(anIntersectionPoint, reflectedDir);

		Vector3<float> intersectionPoint;


		Color skyColor = (1 - aRay.GetDirection().y) * someSkyInfo.myHorizonColor + aRay.GetDirection().y * someSkyInfo.myStraightUpColor;

		result = { skyColor.x * myColor.x, skyColor.y * myColor.y, skyColor.z * myColor.z };



		for (int i = 0; i < someOtherSpheres.size(); i++)
		{
			if (&someOtherSpheres[i] == this) continue;
			if (IntersectionSphereRay(someOtherSpheres[i].GetCollider(), aRay, intersectionPoint))
			{
				Color reflectedColor = someOtherSpheres[i].TracePath(intersectionPoint, aRay, someOtherSpheres, someLightInfo, someSkyInfo, aRecursionDepth - 1);
				result = {
					 reflectedColor.x * myColor.x,
					 reflectedColor.y * myColor.y,
					 reflectedColor.z * myColor.z,
				};
			}
		}
	}
	else
	{
		result = {
		lightSource.myColor.x * myColor.x,
		lightSource.myColor.y * myColor.y,
		lightSource.myColor.z * myColor.z
		};

		aRay.InitWithOriginAndDirection(anIntersectionPoint, lightSource.myDirection * -1.0f);
		result *= CommonUtilities::Max(0.0f, (sphereNormal.Dot(aRay.GetDirection())));
	}





	Vector3<float> intersectionPoint;



	if (!myIsSpecularFlag)
		for (auto& sphere : someOtherSpheres)
		{
			if (&sphere == this) continue;

			if (IntersectionSphereRay(sphere.myCollider, aRay, intersectionPoint))
			{

				result *= 0.0f;

			}
		}

	return result;
}

Vector3<float> Sphere::GetNormal(Vector3<float> aSurfacePoint)
{
	return (aSurfacePoint - myCollider.GetOrigin()).GetNormalized();
}



