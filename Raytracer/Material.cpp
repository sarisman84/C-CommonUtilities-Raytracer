#include "Material.h"
#include "Ray.h"
#include "Sphere.h"
#include "LightSource.h"
#include "SkyInfo.h"
#include "Intersection.h"

using namespace CommonUtilities;


Specular::Specular(const Color& anInitialColor, Sphere* anOwner) : IMaterial(anInitialColor, anOwner)
{
}


Color Specular::CalculateRayColor(Vector3<float> anIntersectionPoint, CommonUtilities::Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aBounceLimit)
{

	Vector3<float> sphereNormal = IMaterial::myOwner->GetNormal(anIntersectionPoint);

	Vector3<float> curDir = aRay.GetDirection(); //Also known as V

	Vector3<float> reflectedDir = curDir - (2.0f * (curDir.Dot(sphereNormal) * sphereNormal));

	aRay.InitWithOriginAndDirection(anIntersectionPoint, reflectedDir);

	Vector3<float> intersectionPoint;


	Color skyColor = (1 - aRay.GetDirection().y) * someSkyInfo.myHorizonColor + aRay.GetDirection().y * someSkyInfo.myStraightUpColor;

	Color result = skyColor * myColor;


	for (int i = 0; i < someOtherSpheres.size(); i++)
	{
		if (someOtherSpheres[i] == IMaterial::myOwner) continue;
		if (IntersectionSphereRay(*someOtherSpheres[i]->GetCollider(), aRay, anIntersectionPoint))
		{
			Color reflectedColor = someOtherSpheres[i]->TracePath(anIntersectionPoint, aRay, someOtherSpheres, someLightInfo, someSkyInfo, 0);
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


	for (auto& sphere : someOtherSpheres)
	{
		if (sphere == IMaterial::myOwner || !sphere->GetCollider()) continue;

		if (IntersectionSphereRay(*sphere->GetCollider(), aRay, anIntersectionPoint))
		{

			result *= 0.1f;

		}
	}

	return result;
}
