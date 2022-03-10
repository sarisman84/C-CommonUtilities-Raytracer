#pragma once
#include "Vector3.h"
#include "UtilityFunctions.h"
#include "Object.h"

namespace CommonUtilities
{
	template<typename T>
	class SphereCollider : public Object<T>
	{
	public:

		// Default constructor: there is no sphere, the radius is zero and the position is
		// the zero vector.
		SphereCollider()
		{
			myOrigin = Vector3<T>();
			myRadius = T();
		}

		// Copy constructor.
		SphereCollider(const SphereCollider<T>& aSphere)
		{
			myOrigin = aSphere.myOrigin;
			myRadius = aSphere.myRadius;
		}

		// Constructor that takes the center position and radius of the sphere.
		SphereCollider(const Vector3<T>& aCenter, T aRadius)
		{
			myOrigin = aCenter;
			myRadius = aRadius;
		}

		// Init the sphere with a center and a radius, the same as the constructor above.
		void InitWithCenterAndRadius(const Vector3<T>& aCenter, T aRadius)
		{
			myOrigin = aCenter;
			myRadius = aRadius;
		}

		// Returns whether a point is inside the sphere: it is inside when the point is on the
		// sphere surface or inside of the sphere.
		bool IsInside(const Vector3<T>& aPosition) const
		{
			//Get Abs Length of (APos - myOrigin) <= myRadius 
			Vector3<T> delta = (aPosition - myOrigin);
			return CommonUtilities::Abs<T>(delta.Length()) <= myRadius;
		}

		T GetRadius() const
		{
			return myRadius;
		}

		Vector3<T> GetOrigin() const override
		{
			return myOrigin;
		}

	private:
		T myRadius;
		Vector3<T> myOrigin;
	};


	
}