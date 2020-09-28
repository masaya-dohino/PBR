#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <cmath>
#include "Vec_3.h"
#include "Ray.h"
#include "Hit.h"

class Sphere {
	public:
		Vec3 center; //’†SˆÊ’u
		double radius; //”¼Œa

		Sphere(const Vec3& _center, double _radius) : center(_center),
			radius(_radius) {};

		//ray‚ÆŒğ·‚µ‚Ä‚¢‚é‚©”»’è‚·‚éBŒğ·‚µ‚Ä‚¢‚éê‡‚ÍÕ“Ëî•ñ‚ğ res ‚ÉŠi”[‚·‚éB
		bool intersect(const Ray& ray, Hit& res) const {
			//“ñŸ•û’ö®‚ÌŒW”
			double b = dot(ray.direction, ray.origin - center);
			double c = (ray.origin - center).length2() - radius * radius;

			//”»•Ê®
			double D = b * b - c;

			//‰ğ‚ÌŒó•â‹——£
			double t1 = -b - std::sqrt(D);
			double t2 = -b + std::sqrt(D);

			if (t1 > 10000 | t2 < 0) return false;
			double t = t1;
			if (t < 0) {
				t = t2;
				if (t2 > 10000) return false;
			}

			//Õ“Ëî•ñ‚ğŠi”[
			res.t = t;
			res.hitPos = ray
		}

};

#endif

