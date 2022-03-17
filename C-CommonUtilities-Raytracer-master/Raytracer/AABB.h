#pragma once
#include "Vector.h"

using namespace CommonUtilities;

namespace CommonUtilities
{
	template<typename T>
	class AABB3D : public Object<T>
	{
	public:

		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB3D()
		{
			myMin = Vector3<T>();
			myMax = Vector3<T>();
		}

		// Copy constructor.
		AABB3D(const AABB3D<T>& aAABB3D)
		{
			myMin = aAABB3D.myMin;
			myMax = aAABB3D.myMax;
		}

		// Constructor taking the positions of the minimum and maximum corners.
		AABB3D(const Vector3<T>& aMin, const Vector3<T>& aMax)
		{
			myMin = aMin;
			myMax = aMax;

		}

		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector3<T>& aMin, const Vector3<T>& aMax)
		{
			myMin = aMin;
			myMax = aMax;
		}

		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector3<T>& aPosition) const
		{
			//Checks if aPos is inside AABB3D by using Min and Max values.

			//aPosition <= max && aPosition >= min

			bool isWithinX = aPosition.x <= myMax.x && aPosition.x >= myMin.x;
			bool isWithinY = aPosition.y <= myMax.y && aPosition.y >= myMin.y;
			bool isWithinZ = aPosition.z <= myMax.z && aPosition.z >= myMin.z;
			
			return isWithinX && isWithinY && isWithinZ;
			
		}

		bool IsInside(const AABB3D<T>& aAABB3D) const
		{
			return
				!(myMax.x < aAABB3D.myMin.x ||
					myMin.x > aAABB3D.myMax.x ||
					myMax.y < aAABB3D.myMin.y ||
					myMin.y > aAABB3D.myMax.y ||
					myMax.z < aAABB3D.myMin.z ||
					myMin.z > aAABB3D.myMax.z);
		}

		Vector3<T>& GetMin() const
		{
			return myMin;
		}

		Vector3<T>& GetMax() const
		{
			return myMax;
		}

		Vector3<T> GetOrigin() const override
		{
			Vector3<T> diff = (myMax - myMin);
			auto temp = 2.0;
			return  diff / (T&)temp;
		}

		Vector3<T> GetSize() const
		{
			Vector3<T> center = GetOrigin();

			return Vector3<T>(
				myMax.x - center.x,
				myMax.y - center.y,
				myMax.z - center.z
				);
		}

	private:
		Vector3<T> myMin;
		Vector3<T> myMax;
	};



	template<typename T>
	class AABB2D
	{
	public:

		// Default constructor: there is no AABB, both min and max points are the zero vector.
		AABB2D()
		{
			myMin = Vector2<T>();
			myMax = Vector2<T>();
		}

		// Copy constructor.
		AABB2D(const AABB3D<T>& aAABB3D)
		{
			myMin = aAABB3D.myMin;
			myMax = aAABB3D.myMax;
		}

		// Constructor taking the positions of the minimum and maximum corners.
		AABB2D(const Vector2<T>& aMin, const Vector2<T>& aMax)
		{
			myMin = aMin;
			myMax = aMax;

		}

		// Init the AABB with the positions of the minimum and maximum corners, same as
		// the constructor above.
		void InitWithMinAndMax(const Vector2<T>& aMin, const Vector2<T>& aMax)
		{
			myMin = aMin;
			myMax = aMax;
		}

		// Returns whether a point is inside the AABB: it is inside when the point is on any
		// of the AABB's sides or inside of the AABB.
		bool IsInside(const Vector2<T>& aPosition) const
		{
			//Checks if aPos is inside AABB3D by using Min and Max values.

			//aPosition <= max && aPosition >= min

			bool isWithinX = aPosition.x <= myMax.x && aPosition.x >= myMin.x;
			bool isWithinY = aPosition.y <= myMax.y && aPosition.y >= myMin.y;

			return isWithinX && isWithinY;

		}

		bool IsInside(const AABB2D<T>& aAABB2D) const
		{
			return
				!(myMax.x < aAABB2D.myMin.x ||
					myMin.x > aAABB2D.myMax.x ||
					myMax.y < aAABB2D.myMin.y ||
					myMin.y > aAABB2D.myMax.y);
		}

		Vector2<T>& GetMin() const
		{
			return myMin;
		}

		Vector2<T>& GetMax() const
		{
			return myMax;
		}



	private:
		Vector2<T> myMin;
		Vector2<T> myMax;
	};
}