#include "Camera.h"

Camera::Camera(const Vector3f& aPos, const Vector3f& aRight, const Vector3f& aForward, const Vector3f& aUp)
{
	myPos = aPos;
	myRight = aRight;
	myForward = aForward;
	myUp = aUp;
}

Camera::~Camera()
{
}

Vector3f Camera::GetForward() const
{
	return myForward;
}

Vector3f Camera::GetPos() const
{
	return myPos;
}

Vector3f Camera::GetRight() const
{
	return myRight;
}

Vector3f Camera::GetUp() const
{
	return myUp;
}


