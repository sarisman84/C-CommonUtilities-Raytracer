#pragma once
#include "SphereCollider.h"
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include <memory>

using namespace CommonUtilities;


struct SkyInfo;
struct LightInfo;


class Sphere
{
public:
	Sphere() = default;
	Sphere(const Vector3<float> aCenter, const float aRadius, const Color aColor, const bool& anIsSpecular);
	//Sphere(Sphere& anOther);
	//void operator=(Sphere& anOther);
	~Sphere();
	SphereCollider<float>* GetCollider() const;
	std::shared_ptr<IMaterial>& GetMaterial();
	const bool IsSpecular() const;
	Color TracePath(Vector3<float> anIntersectionPoint, Ray<float> aRay, std::vector<Sphere*>& someOtherSpheres, LightInfo someLightInfo, SkyInfo someSkyInfo, const int aRecursionDepth = 5, const int someAmmOfAdditionalCasts = 5);
	Vector3<float> GetNormal(Vector3<float> aSurfacePoint);
	Vector3<float> RandomUnitVector();



	inline static void ParseInformation(const char* anInput, std::vector<Sphere*>& aCollection);

private:
	SphereCollider<float>* myCollider;
	std::shared_ptr<IMaterial> myMaterial;
	bool myIsSpecularFlag;
};


inline void Sphere::ParseInformation(const char* anInput, std::vector<Sphere*>& aCollection)
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


		aCollection.push_back(new Sphere(Vector3<float>{ posX, posY, posZ }, radius, Color(red, green, blue), isSpecular));

	}


}
