#pragma once
#include "Vector3.h"
#include "Object.h"

namespace CommonUtilities
{
	template<typename T>
	class PlaneCollider : public Object<T>
	{
	public:
		// Default constructor.
		PlaneCollider()
		{
			myNormal = Vector3<T>();
			myOrigin = Vector3<T>();
		}

		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2-aPoint0).
		PlaneCollider(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
		{
			InitWith3Points(aPoint0, aPoint1, aPoint2);
		}

		// Constructor taking a point and a normal.
		PlaneCollider(const Vector3<T>& aPoint0, const Vector3<T>& aNormal)
		{
			InitWithPointAndNormal(aPoint0, aNormal);
		}

		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2)
		{
			Vector3<T> dir1 = (aPoint1 - aPoint0).GetNormalized();
			Vector3<T> dir2 = (aPoint2 - aPoint0).GetNormalized();

			myNormal = dir1.Cross(dir2).GetNormalized();

			myOrigin = aPoint0;
		}

		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal)
		{
			myOrigin = aPoint;
			myNormal = aNormal.GetNormalized();
		}

		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		bool IsInside(const Vector3<T>& aPosition) const
		{
			//Function: https://yh.pingpong.se/node.do?id=6251702
			//q = aPosition (x,y,z)
			//n = normal (a,b,c) n = myNormal?
			//a*x + b*y + c * z = k
			//kollar om k <= 0


			Vector3<T> dirToATargetPosition = (aPosition - myOrigin).GetNormalized();
			T dotProd = myNormal.Dot(dirToATargetPosition);
			return  dotProd == 0;
		}

		// Returns the normal of the plane.
		const Vector3<T>& GetNormal() const
		{
			return myNormal;
		}

		Vector3<T> GetOrigin() const override
		{
			return myOrigin;
		}

	private:
		Vector3<T> myNormal;
		Vector3<T> myOrigin;

	};
}
