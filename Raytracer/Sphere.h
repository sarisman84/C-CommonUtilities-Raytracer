#pragma once
#include "SphereCollider.h"
#include "Vector3.h"
#include "Ray.h"



using namespace CommonUtilities;
typedef Vector3<float> Color;

struct SkyInfo;
struct LightInfo;


class Sphere
{
public:
	Sphere(const Vector3<float> aCenter, const float aRadius, const Color aColor, const bool anIsSpecularFlag = false);
	SphereCollider<float> GetCollider() const;
	Color GetColor() const;
	const bool IsSpecular() const;
	Color TracePath(Vector3<float> anIntersectionPoint, Ray<float> aRay, std::vector<Sphere>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aRecursionDepth = 10);	Vector3<float> GetNormal(Vector3<float> aSurfacePoint);
	inline static void ParseInformation(const char* anInput, std::vector<Sphere>& aCollection);

private:
	SphereCollider<float> myCollider;
	Color myColor;
	bool myIsSpecularFlag;
};


inline void Sphere::ParseInformation(const char* anInput, std::vector<Sphere>& aCollection)
{
	const char* sphereString = "sphere";
	const char* specularSphereString = "mirror_sphere";

	bool isDiffuse = strncmp(anInput, sphereString, strlen(sphereString)) == 0;
	bool isSpecular = strncmp(anInput, specularSphereString, strlen(specularSphereString)) == 0;

	bool isSphere = isDiffuse || isSpecular;
	if (isSphere)
	{
		char* remainder = nullptr;
		float posX = strtof(anInput + (isDiffuse ? strlen(sphereString) : strlen(specularSphereString)), &remainder);
		float posY = strtof(remainder, &remainder);
		float posZ = strtof(remainder, &remainder);

		float radius = strtof(remainder, &remainder);

		float red = strtof(remainder, &remainder);
		float green = strtof(remainder, &remainder);
		float blue = strtof(remainder, &remainder);

		std::cout << "Sphere at position [" << posX << ", " << posY << ", " << posZ << "]\n";
		std::cout << "With radius: " << radius << "\n";
		std::cout << "and color [" << red << ", " << green << ", " << blue << "]\n";
		if (isDiffuse)
			std::cout << "And it's material is Diffuse" << "\n";
		else
			std::cout << "And it's material is Specular" << "\n";
		std::cout << "\n";

		// Add code to store the scene!
		aCollection.push_back(Sphere({ posX, posY, posZ }, radius, { red, green, blue }, isSpecular));

	}


}
