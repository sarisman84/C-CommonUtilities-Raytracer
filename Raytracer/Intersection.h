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
	// Any ray starting on the inside is considered to intersect the AABB
	template<typename T>
	bool IntersectionAABBRay(const AABB3D<T>& aAABB, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		if (aAABB.IsInside(aRay.GetOrigin()))
		{
			aOutIntersectionPoint = aRay.GetOrigin();

			return true;
		}

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
	// Any ray starting on the inside is considered to intersect the sphere
	template<typename T>
	bool IntersectionSphereRay(const SphereCollider<T>& aSphere, const Ray<T>& aRay, Vector3<T>& aOutIntersectionPoint)
	{
		/*
		1. Hitta punkten på strålen som är närmast  sfären/cirkeln
			-Projicera (aSphere.myOrigin - aRay.myOrigin) på aRay.myDirection för att hitta a.
		2. Kolla om a är i sfären/cirkeln
		3. If yes: hitta skärningspunkten, b.
			-Pythagoras sats Abs(ca)^2 + Abs(ab)^2 = r^2
			-Abs(ab) = +- sqrt(r^2 - abs(ca)^2
			-Hitta sen b med a, abs(ab) och d.

		*/

		if (aSphere.IsInside(aRay.GetOrigin()))
		{

			aOutIntersectionPoint = aRay.GetOrigin();
			return true;
		}

		
		Vector3<T> dirToSphere = (aSphere.GetOrigin() - aRay.GetOrigin());
		
	

		T directionProd = aRay.GetDirection().Dot(dirToSphere.GetNormalized());

		//std::cout << "- Direction Prod: " << directionProd << std::endl;

		if (directionProd <= 0) return false;

		
		Vector3<T> a = GetClosestPoint(&aSphere, aRay);


		if (!aSphere.IsInside(a)) return false;

		T caAbsLength = CommonUtilities::Abs((aSphere.GetOrigin() - a).LengthSqr());

		T aToBLength = sqrt((aSphere.GetRadius() * aSphere.GetRadius()) - caAbsLength);

		Vector3<T> dir = aRay.GetDirection();

		Vector3<T> ab = dir * aToBLength;

		aOutIntersectionPoint = a - ab;



		return true;

		// Big brain time
	}

	template< typename T>
	Vector3<T> GetClosestPoint(const Object<T>* anObject, const Ray<T>& aRay)
	{
		/* Very good math
		v = p.dir;
		u = c.o - p.o;

		a = (u * (v^2)) / v.LengthSqr
		*/

		Vector3<T> objRayDif = (anObject->GetOrigin() - aRay.GetOrigin());
		Vector3<T> dir = aRay.GetDirection();
		//Vector3<T> dirSquared = dir * dir;


		Vector3<T> closestPoint = aRay.GetOrigin() + dir * (objRayDif.Dot(dir));
		return closestPoint;
	}
}