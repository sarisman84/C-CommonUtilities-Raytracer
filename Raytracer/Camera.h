#pragma once

#include "Vector3.h"
#include <iostream>

typedef CommonUtilities::Vector3<float> Vector3f;

class Camera
{

public:

	Camera() = default;
	Camera(const Vector3f& aPos, const Vector3f& aRight, const Vector3f& aForward, const Vector3f& aUp);
	~Camera();

	Vector3f GetPos() const;
	Vector3f GetRight() const;
	Vector3f GetForward() const;
	Vector3f GetUp() const;

	inline static void ParseInformation(const char* anInput, Camera& aCamera);


private:

	Vector3f myPos;
	Vector3f myRight;
	Vector3f myForward;
	Vector3f myUp;

};



inline void Camera::ParseInformation(const char* anInput, Camera& aCamera)
{
	const char* cameraString = "camera";
	if (strncmp(anInput, cameraString, strlen(cameraString)) == 0)
	{
		char* remainder = nullptr;
		float posX = strtof(anInput + strlen(cameraString), &remainder);
		float posY = strtof(remainder, &remainder);
		float posZ = strtof(remainder, &remainder);

		float rightX = strtof(remainder, &remainder);
		float rightY = strtof(remainder, &remainder);
		float rightZ = strtof(remainder, &remainder);

		float upX = strtof(remainder, &remainder);
		float upY = strtof(remainder, &remainder);
		float upZ = strtof(remainder, &remainder);

		float forwardX = strtof(remainder, &remainder);
		float forwardY = strtof(remainder, &remainder);
		float forwardZ = strtof(remainder, nullptr);

		std::cout << "Camera at position [" << posX << ", " << posY << ", " << posZ << "]\n";
		std::cout << "With orientation: \n";
		std::cout << "[" << rightX << ", " << rightY << ", " << rightZ << "]\n";
		std::cout << "[" << upX << ", " << upY << ", " << upZ << "]\n";
		std::cout << "[" << forwardX << ", " << forwardY << ", " << forwardZ << "]\n";
		std::cout << "\n";

		// Add code to store the camera!
		aCamera = Camera(Vector3f(posX, posY, posZ), Vector3f(rightX, rightY, rightZ), Vector3f(forwardX, forwardY, forwardZ), Vector3f(upX, upY, upZ));
	}


}




