#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "random.h"

class Camera {
public:
	Vec3 camPos;
	Vec3 camForward;
	Vec3 camRight;
	Vec3 camUp;

	Camera(const Vec3& _camPos, const Vec3& _camForward)
	{
		camPos = _camPos;
		camForward = _camForward;
		orthonormalBasis(camForward, camRight, camUp);

		std::cout << "CamPos = " << camPos << std::endl;
		std::cout << "CamForward = " << camForward << std::endl;
		std::cout << "CamRight = " << camRight << std::endl;
		std::cout << "CamUP = " << camUp << std::endl;
	}

	virtual Ray getRay(double u, double v) const = 0;
};

class PinholeCamera : public Camera {
public:
	double pinholeDist;

	PinholeCamera(const Vec3& _camPos, const Vec3& _camForward, double _pinholeDist)
		: Camera(_camPos, _camForward), pinholeDist(_pinholeDist)
	{
		// 
	}

	Ray getRay(const double u, const double v) const {
		Vec3 pihholePos = camPos + pinholeDist * camForward;
		Vec3 sensorPos = camPos + u * camRight + v * camUp;
		return Ray(sensorPos, normalize(pihholePos - sensorPos));
	}
};

class ThinLensCamera : public Camera {
public:
	double a;
	double b;
	double f;
	double lensRadius;
	Vec3 lensCenter;

	ThinLensCamera(const Vec3& _camPos, const Vec3& _camForward,
		const Vec3& focusPoint, double _a, double _lensRadius)
		: Camera(_camPos, _camForward), a(_a), lensRadius(_lensRadius)
	{
		double cos = dot(camForward, normalize(focusPoint - camPos));
		b = cos * (focusPoint - camPos).length() - a;
		f = 1.0 / (1.0 / a + 1.0 / b);
		lensCenter = camPos + a * camForward;
	}

	Ray getRay(const double u, const double v) const {
		Vec3 sensorPos = camPos + u * camRight + v * camUp;
		Vec3 r = normalize(lensCenter - sensorPos);
		Vec3 pf = sensorPos + (a + b) / dot(camForward, r) * r;

		double x, y;
		sampleDisk(x, y);
		Vec3 l = lensCenter + lensRadius * (x*camRight + y * camUp);

		return Ray(l, normalize(pf - l));
	}
};