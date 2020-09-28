#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <cmath>
#include "Vec_3.h"
#include "Ray.h"
#include "Hit.h"

class Sphere {
	public:
		Vec3 center; //���S�ʒu
		double radius; //���a

		Sphere(const Vec3& _center, double _radius) : center(_center),
			radius(_radius) {};

		//ray�ƌ������Ă��邩���肷��B�������Ă���ꍇ�͏Փˏ��� res �Ɋi�[����B
		bool intersect(const Ray& ray, Hit& res) const {
			//�񎟕������̌W��
			double b = dot(ray.direction, ray.origin - center);
			double c = (ray.origin - center).length2() - radius * radius;

			//���ʎ�
			double D = b * b - c;

			//���̌�⋗��
			double t1 = -b - std::sqrt(D);
			double t2 = -b + std::sqrt(D);

			if (t1 > 10000 | t2 < 0) return false;
			double t = t1;
			if (t < 0) {
				t = t2;
				if (t2 > 10000) return false;
			}

			//�Փˏ����i�[
			res.t = t;
			res.hitPos = ray
		}

};

#endif

