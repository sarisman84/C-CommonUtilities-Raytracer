#include "Material.h"
#include "Ray.h"
#include "Sphere.h"
#include "LightSource.h"
#include "SkyInfo.h"
#include "Intersection.h"
#include "CScene.h"

using namespace CommonUtilities;


Specular::Specular(const Color& anInitialColor, Sphere* anOwner) : IMaterial(anInitialColor, anOwner)
{
}


Color Specular::CalculateRayColor(Vector3<float> anIntersectionPoint, CommonUtilities::Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aBounceLimit)
{

	Vector3<float> sphereNormal = IMaterial::myOwner->GetNormal(anIntersectionPoint);

	Vector3<float> curDir = aRay.GetDirection(); //Also known as V

	Vector3<float> reflectedDir = curDir - (2.0f * (curDir.Dot(sphereNormal) * sphereNormal));

	aRay.InitWithOriginAndDirection(anIntersectionPoint + reflectedDir * 0.01f, reflectedDir);



	Color skyColor = (1 - aRay.GetDirection().y) * someSkyInfo.myHorizonColor + aRay.GetDirection().y * someSkyInfo.myStraightUpColor;

	Color result = skyColor * myColor;

	Vector3<float> tempPoint;

	for (auto& col : someOtherSpheres)
	{

		if (col == IMaterial::myOwner) continue;

		if (IntersectionSphereRay(*(col->GetCollider()), aRay, tempPoint))
		{
			Color reflectedColor = col->TracePath(tempPoint, aRay, someOtherSpheres, someLightInfo, someSkyInfo, aBounceLimit - 1);
			result = reflectedColor * myColor;
		}

	}


	return result;
}

IMaterial::IMaterial(const Color& anInitialColor, Sphere* anOwner)
{
	myColor = anInitialColor;
	myOwner = anOwner;
}

Diffuse::Diffuse(const Color& anInitialColor, Sphere* anOwner) : IMaterial(anInitialColor, anOwner)
{
}

Color Diffuse::CalculateRayColor(Vector3<float> anIntersectionPoint, CommonUtilities::Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aBounceLimit)
{
	Vector3<float> sphereNormal = IMaterial::myOwner->GetNormal(anIntersectionPoint);

	Color result = someLightInfo.myColor * myColor;

	aRay.InitWithOriginAndDirection(anIntersectionPoint, someLightInfo.myDirection * -1.0f);

	result *= CommonUtilities::Max(0.0f, (sphereNormal.Dot(aRay.GetDirection())));


	Vector3<float> tempPoint;


	for (auto& col : someOtherSpheres)
	{

		if (col == IMaterial::myOwner) continue;

		if (IntersectionSphereRay(*(col->GetCollider()), aRay, tempPoint))
		{
			result *= 0.0f;
		}

	}

	auto dir = (IMaterial::myOwner->GetNormal(anIntersectionPoint) + IMaterial::myOwner->RandomUnitVector()).GetNormalized();
	aRay.InitWithOriginAndDirection(anIntersectionPoint + dir * 0.1f, dir);


	Color skyColor = (1 - aRay.GetDirection().y) * someSkyInfo.myHorizonColor + aRay.GetDirection().y * someSkyInfo.myStraightUpColor;

	auto sphere = CScene::GetClosestSphere(aRay, someOtherSpheres);

	if (sphere._Myfirst._Val)
	{
		result += sphere._Myfirst._Val->TracePath(sphere._Get_rest()._Myfirst._Val, aRay, someOtherSpheres, someLightInfo, someSkyInfo, aBounceLimit - 1);
	}
	else
	{
		result += skyColor * someLightInfo.myColor;
	}







	return result;
}

Emissive::Emissive(const Color& anInitialColor, Sphere* anOwner) : IMaterial(anInitialColor, anOwner)
{
}

Color Emissive::CalculateRayColor(Vector3<float> anIntersectionPoint, CommonUtilities::Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aBounceLimit)
{
	return IMaterial::myColor;
}
