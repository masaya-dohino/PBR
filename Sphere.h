#pragma once
#include <cmath>
#include <memory>
#include "Vec3.h"
#include "Ray.h"
#include "Hit.h"
#include "Material.h"
#include "Light.h"

class Sphere : public Hitable {
public:
	Vec3 center;
	double radius;

	// std::shared_ptr<Material> p_material; //BRDF
	// std::shared_ptr<Light> p_light; //Le

	Sphere(const Vec3& _center, const double& _radius,
		const std::shared_ptr<Material>& _p_material,
		const std::shared_ptr<Light>& _p_light)
		: Hitable()
	{
		center = _center;
		radius = _radius;
		/*Hitable::*/p_material = _p_material;
		/*Hitable::*/p_light = _p_light;
	}

	Sphere(const Vec3& _center, const double& _radius)
	{
		center = _center;
		radius = _radius;
	}

	bool intersect(const Ray& ray, Hit& res) const {
		double a = ray.direction.length2();
		double b = dot(ray.direction, ray.origin - center);
		double c = (ray.origin - center).length2() - radius * radius;

		double D = b * b - a * c;
		if (D < 0.0) return false;

		double t1 = -b - std::sqrt(D);
		double t2 = -b + std::sqrt(D);
		if ((t1 > 10000.0) || (t2 < 0.001)) return false;

		double t = t1;
		if (t < 0.001) {
			t = t2;
			if (t > 10000.0) {
				return false;
			}
		}

		res = Hit(t, ray(t), normalize(ray(t) - center), (Hitable*)this);
		return true;
	}
};