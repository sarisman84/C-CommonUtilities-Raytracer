#pragma once
#include "Vector3.h"

using namespace CommonUtilities;

struct SkyInfo
{
	SkyInfo() = default;
	SkyInfo(const Vector3<float> aHorizonColor, const Vector3<float> aStraightUpColor)
	{
		myHorizonColor = aHorizonColor;
		myStraightUpColor = aStraightUpColor;
	}

	static inline void ParseInformation(const char* anInput, SkyInfo& someInfo);

	Vector3<float> myHorizonColor;
	Vector3<float> myStraightUpColor;
};


inline void SkyInfo::ParseInformation(const char* anInput, SkyInfo& someInfo)
{

	const char* skyString = "sky";
	if (strncmp(anInput, skyString, strlen(skyString)) == 0)
	{
		char* remainder = nullptr;

		float hRed = strtof(anInput + strlen(skyString), &remainder);
		float hGreen = strtof(remainder, &remainder);
		float hBlue = strtof(remainder, &remainder);

		float sRed = strtof(remainder, &remainder);
		float sGreen = strtof(remainder, &remainder);
		float sBlue = strtof(remainder, &remainder);

		std::cout << "Sky Info\n";
		std::cout << "With horizon color [" << hRed << ", " << hGreen << ", " << hBlue << "]\n";
		std::cout << "And straight up color [" << sRed << ", " << sGreen << ", " << sBlue << "]\n";
		std::cout << "\n";

		someInfo = SkyInfo({ hRed, hGreen, hBlue }, { sRed, sGreen, sBlue });
	}
}