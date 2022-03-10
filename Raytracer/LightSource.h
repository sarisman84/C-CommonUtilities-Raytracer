#pragma once

#include "Vector3.h"

using namespace CommonUtilities;

struct LightInfo
{
	LightInfo() = default;
	LightInfo(const Vector3<float> anPosition, const Vector3<float> anColor)
		: myDirection(anPosition), myColor(anColor)
	{
	}

	static inline void ParseInformation(const char* anInput, LightInfo& someInfo);

	Vector3<float> myDirection;
	Vector3<float> myColor;

};


inline void LightInfo::ParseInformation(const char* anInput, LightInfo& someInfo)
{
	const char* dirLightString = "directional_light";
	if (strncmp(anInput, dirLightString, strlen(dirLightString)) == 0)
	{
		char* remainder = nullptr;

		float posX = strtof(anInput + strlen(dirLightString), &remainder);
		float posY = strtof(remainder, &remainder);
		float posZ = strtof(remainder, &remainder);

		float red = strtof(remainder, &remainder);
		float green = strtof(remainder, &remainder);
		float blue = strtof(remainder, &remainder);

		std::cout << "LightSource at position [" << posX << ", " << posY << ", " << posZ << "]\n";
		std::cout << "and color [" << red << ", " << green << ", " << blue << "]\n";
		std::cout << "\n";

		someInfo = LightInfo({ posX, posY, posZ }, { red,green,blue });
	}


}