#pragma once
#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "AABB.h"
#include "Ray.h"
#include "UtilityFunctions.h"
#include "Object.h"

namespace CommonUtilities
{

	// If the ray does not intersect the plane aOutIntersectionPoint remains unchanged
	// and false is returned.
	// If the ray intersects the plane the intersection point is stored in
	// aOutIntersectionPoint and true is returned.
	template<typename T>
	bool IntersectionPlaneRay(const PlaneCollider<T>& aPlane, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		if (aPlane.IsInside(aRay.GetOrigin()))
		{
			aOutIntersectionPoint = aRay.GetOrigin();

			return true;
		}

		T normalDotProd = aRay.GetDirection().Dot(aPlane.GetNormal());
		T pointDotProd = aRay.GetOrigin().Dot(aPlane.GetNormal());

		Vector3<T> rayToPlane = aRay.GetOrigin() - aPlane.GetOrigin();

		if (normalDotProd == 0) return false;
		T distance = rayToPlane.Length() - CommonUtilities::Abs(normalDotProd);
		T t = (distance - pointDotProd) / normalDotProd; //(aPlane.distance - pointDotProd)/ normalDotProd;
		aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * t;
		return true; //Big brain, works
	}


	// If the ray intersects the AABB, true is returned, if not, false is returned.
	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		Vector3<T> a = GetClosestPoint(&aAABB, aRay);

		if (!aAABB.IsInside(a)) return false;

		T caAbsLength = CommonUtilities::Abs((aAABB.GetOrigin() - a).LengthSqr());

		T aToBLength = sqrt(CommonUtilities::Abs(aAABB.GetSize().LengthSqr()) - caAbsLength);

		Vector3<T> dir = aRay.GetDirection();

		Vector3<T> ab = dir * aToBLength;

		aOutIntersectionPoint = a - ab;

		return true;

	}


	// If the ray intersects the sphere, true is returned, if not, false is returned.
	template<typename T>
	bool IntersectionSphereRay(const SphereCollider<T>& aSphere, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		Vector3<T> dir = (aSphere.GetOrigin() - aRay.GetOrigin());
		T dirDot = dir.Dot(aRay.GetDirection().GetNormalized());;

		if (!aSphere.IsInside(aRay.GetOrigin() + aRay.GetDirection().GetNormalized() * dirDot)) return false;
		
		T rayDist = dirDot - std::sqrt(aSphere.GetRadius() * aSphere.GetRadius() - dir.LengthSqr() + dirDot * dirDot);
	
		if (rayDist < 0) return false;

		aOutIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection().GetNormalized() * rayDist;

		return true;

	}

	template< typename T>
	Vector3<T> GetClosestPoint(const Object<T>* anObject, const Ray<T>& aRay)
	{

		Vector3<T> objRayDif = (anObject->GetOrigin() - aRay.GetOrigin());
		Vector3<T> dir = aRay.GetDirection();
		//Vector3<T> dirSquared = dir * dir;

		Vector3<T> closestPoint = aRay.GetOrigin() + dir * (objRayDif.Dot(dir));
		return closestPoint;
	}
}