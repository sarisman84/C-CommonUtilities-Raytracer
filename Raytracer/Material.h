#pragma once
#include "Vector3.h"
#include <vector>
#include "Color.h"


using namespace CommonUtilities;

class Sphere;
namespace CommonUtilities
{
	template<typename T>
	class Ray;
}
struct LightInfo;
struct SkyInfo;




class IMaterial
{
public:
	IMaterial(const Color& anInitialColor, Sphere* anOwner);
	virtual Color CalculateRayColor(Vector3<float> anIntersectionPoint, CommonUtilities::Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aBounceLimit = 0) = 0;
protected:
	Color myColor;
	Sphere* myOwner;
};



class Specular : public IMaterial
{
public:
	Specular(const Color& anInitialColor, Sphere* anOwner);
	// Inherited via IMaterial
	virtual Color CalculateRayColor(Vector3<float> anIntersectionPoint, CommonUtilities::Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aBounceLimit = 0) override;
	

};


class Diffuse : public IMaterial
{
public:
	Diffuse(const Color& anInitialColor, Sphere* anOwner);
	virtual Color CalculateRayColor(Vector3<float> anIntersectionPoint, CommonUtilities::Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aBounceLimit = 0) override;
};
