#pragma once


#include "Util.h"


struct Color
{
	Color() = default;
	Color(const float& aR, const float& aG, const float& aB)
	{
		r = aR;
		g = aG;
		b = aB;
	}
	float r, g, b;
	inline Color operator*(const Color& someOtherColor)
	{
		r *= someOtherColor.r;
		g *= someOtherColor.g;
		b *= someOtherColor.b;

		return *this;
	}


	inline Color operator*(const float& someConstant)
	{
		r *= someConstant;
		g *= someConstant;
		b *= someConstant;

		return *this;
	}

	inline Color operator +(const Color& someOtherColor)
	{
		r += someOtherColor.r;
		g += someOtherColor.g;
		b += someOtherColor.b;

		return *this;
	}


	inline void operator *=(const float& anConstant)
	{
		r *= anConstant;
		g *= anConstant;
		b *= anConstant;
	}


	inline void operator *=(const Color& aColor)
	{
		r *= aColor.r;
		g *= aColor.g;
		b *= aColor.b;
	}



	inline void operator =(SRGB anRGB)
	{
		r = anRGB.r;
		g = anRGB.g;
		b = anRGB.b;
	}


	inline void operator += (const Color& someOtherColor)
	{
		r += someOtherColor.r;
		g += someOtherColor.g;
		b += someOtherColor.b;
	}

	inline void operator /= (const float& aValue)
	{
		r /= aValue;
		g /= aValue;
		b /= aValue;
	}

	inline Color operator / (const float& aValue)
	{
		r /= aValue;
		g /= aValue;
		b /= aValue;
		return *this;
	}

};



inline Color operator*(float someConstant, Color aColor)
{
	aColor = aColor * someConstant;
	return aColor;
}


