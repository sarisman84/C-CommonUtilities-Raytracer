#pragma once
#include "Color.h"
#include "Vector3.h"

using namespace CommonUtilities;

struct LightInfo
{
	LightInfo() = default;
	LightInfo(const Vector3<float> aDirection, const Color anColor)
		: myDirection(aDirection), myColor(anColor)
	{
	}

	static inline void ParseInformation(const char* anInput, LightInfo& someInfo);

	Vector3<float> myDirection;
	Color myColor;

};


inline void LightInfo::ParseInformation(const char* anInput, LightInfo& someInfo)
{
	const char* dirLightString = "directional_light";
	if (strncmp(anInput, dirLightString, strlen(dirLightString)) == 0)
	{
		char* remainder = nullptr;

		float dirX = strtof(anInput + strlen(dirLightString), &remainder);
		float dirY = strtof(remainder, &remainder);
		float dirZ = strtof(remainder, &remainder);

		float red = strtof(remainder, &remainder);
		float green = strtof(remainder, &remainder);
		float blue = strtof(remainder, &remainder);

		std::cout << "LightSource at position [" << dirX << ", " << dirY << ", " << dirZ << "]\n";
		std::cout << "and color [" << red << ", " << green << ", " << blue << "]\n";
		std::cout << "\n";

		someInfo = LightInfo({ dirX, dirY, dirZ }, Color(red, green, blue));
	}


}