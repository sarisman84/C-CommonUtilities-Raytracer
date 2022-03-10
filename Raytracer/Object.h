#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	template<typename T>
	class Object
	{
	public:
		virtual Vector3<T> GetOrigin() const = 0;
	};
}

