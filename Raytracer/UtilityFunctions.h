#pragma once
#include <cassert>
#include <random>
namespace CommonUtilities
{
	//Returns the maximum value.
	template <typename T>
	T Max(const T& aMinVal, const T& aMaxVal)
	{
		return aMaxVal > aMinVal ? aMaxVal : aMinVal;
	}

	//Returns the minimum value.
	template <typename T>
	T Min(const T& aMinVal, const T& aMaxVal)
	{
		return aMaxVal > aMinVal ? aMinVal : aMaxVal;
	}

	//Returns an absolute value.
	template <typename T>
	T Abs(const T& aVal)
	{
		if (aVal >= 0)
			return aVal;


		return aVal * -1;
	}

	//Clamps a value between a minimum value and a maximum value.
	template <typename T>
	T Clamp(const T& aMinValue, const T& aMaxValue, const T& aValue)
	{
		assert(aMinValue <= aMaxValue);
		return aValue < aMinValue ? aMinValue : aValue > aMaxValue ? aMaxValue
			: aValue;
	}

	//Lerps between two values by a third percentage value (between 0 and 1).
	template <typename T>
	T Lerp(const T& aLhsVal, const T& aRhsVal, const float& aPercentageVal)
	{
		//Making sure that the inputed percentage value is clamped between 0 and 1.
		float finalValue = Clamp<float>(0, 1, aPercentageVal);

		//Thanks to this post for the contribution: https://stackoverflow.com/questions/4353525/floating-point-linear-interpolation
		return (aLhsVal * (1.0f - finalValue)) + (aRhsVal * finalValue);
	}

	template <typename T>
	void Swap(T& aLhsVal, T& aRhsVal)
	{
		T temp = aLhsVal;
		aLhsVal = aRhsVal;
		aRhsVal = temp;
	}

	template <typename T>
	T RandomRange(const T aMinValue, const T aMaxValue)
	{
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 gen(rd()); // seed the generator
		std::uniform_real_distribution<> distr(aMinValue, aMaxValue); // define the range

		return (T)distr(gen);
	}


}
