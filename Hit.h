#pragma once

#include "Vec3.h"
#include "Hitable.h"

class Sphere;

class Hit {
public:
	double distance;
	Vec3 hitPos;
	Vec3 hitNormal;
	// const Sphere* hitSphere;
	const Hitable* hitSphere;

	Hit() {
		distance = 10000.0;
	};

	Hit(const double& _d, const Vec3& _hitPos, const Vec3& _hitNormal, const Hitable* _hitSphere)
		: distance(_d), hitPos(_hitPos), hitNormal(_hitNormal), hitSphere(_hitSphere)
	{}

};