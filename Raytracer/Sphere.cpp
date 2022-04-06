#include "Sphere.h"
#include "LightSource.h"
#include "Intersection.h"
#include "SkyInfo.h"
#include "UtilityFunctions.h"
#include <iomanip>

Sphere::Sphere(const Vector3<float> aCenter, const float aRadius, const Color aColor, const MatType& aMat)
{
	myCollider = new SphereCollider<float>(aCenter, aRadius);


	switch (aMat)
	{
	case MatType::Diffuse:
		myMaterial = std::make_shared<Diffuse>(aColor, this);
		break;
	case MatType::Emissive:
		myMaterial = std::make_shared<Emissive>(aColor, this);
		break;
	case MatType::Specular:
		myMaterial = std::make_shared<Specular>(aColor, this);
		break;

	}



}

//Sphere::Sphere(Sphere& anOther)
//{
//	myCollider = anOther.myCollider;
//	anOther.myCollider = nullptr;
//	myIsSpecularFlag = anOther.myIsSpecularFlag;
//	myMaterial = anOther.myMaterial;
//}
//
//
//void Sphere::operator=(Sphere& anOther)
//{
//	myCollider = anOther.myCollider;
//	myIsSpecularFlag = anOther.myIsSpecularFlag;
//	myMaterial = anOther.myMaterial;
//}

Sphere::~Sphere()
{
	if (myCollider)
	{
		delete myCollider;
	}
	myCollider = nullptr;
}

SphereCollider<float>* Sphere::GetCollider() const
{
	return myCollider;
}

std::shared_ptr<IMaterial>& Sphere::GetMaterial()
{
	return myMaterial;
}



const bool Sphere::IsSpecular() const
{
	return myIsSpecularFlag;
}


Vector3<float> Sphere::RandomUnitVector() //Från Föreläsningen LA09 - Pathtracing (=
{
	float z = RandomFloat() * 2.0f - 1.0f;
	float a = RandomFloat() * 2.0f - std::_Pi;
	float r = sqrtf(1.0f - z * z);
	float x = r * cosf(a);
	float y = r * sinf(a);
	return Vector3<float>(x, y, z);
}


Color Sphere::TracePath(Vector3<float> anIntersectionPoint, Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aRecursionDepth)
{
	if (aRecursionDepth < 0) return Color{};

	return myMaterial->CalculateRayColor(anIntersectionPoint, aRay, someOtherSpheres, someLightInfo, someSkyInfo, aRecursionDepth);;
}

Vector3<float> Sphere::GetNormal(Vector3<float> aSurfacePoint)
{
	return (aSurfacePoint - myCollider->GetOrigin()).GetNormalized();
}



