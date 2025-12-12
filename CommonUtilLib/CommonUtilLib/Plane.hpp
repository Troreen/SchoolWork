#pragma once
#include "Vector3.hpp"


namespace CommonUtilities
{
	template <typename T>
	class Plane
	{
		// Default constructor.
		Plane();

		// Constructor taking three points where the normal is (aPoint1 - aPoint0) x (aPoint2-aPoint0).
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		// Constructor taking a point and a normal.
		Plane(const Vector3<T>& aPoint0, const Vector3<T>& aNormal);

		// Init the plane with three points, the same as the constructor above.
		void InitWith3Points(const Vector3<T>& aPoint0, const Vector3<T>& aPoint1, const Vector3<T>& aPoint2);

		// Init the plane with a point and a normal, the same as the constructor above.
		void InitWithPointAndNormal(const Vector3<T>& aPoint, const Vector3<T>& aNormal);

		// Returns whether a point is inside the plane: it is inside when the point is on the plane or on the side the normal is pointing away from.
		// Always returns false if used with an uninitialized line
		bool IsInside(const Vector3<T>& aPosition) const;

		// Returns the normal of the plane.
		const Vector3<T>& GetNormal() const;
	};

	
}