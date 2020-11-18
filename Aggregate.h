#pragma once

#include <memory>
#include <vector>
#include "Ray.h"
#include "Hit.h"
#include "Sphere.h"

class Aggregate {
public:
	std::vector<std::shared_ptr<Sphere>> vp_spheres;

	Aggregate() {};
	Aggregate(const std::vector<std::shared_ptr<Sphere>>& _spheres)
		: vp_spheres(_spheres)
	{

	}

	void add(const std::shared_ptr<Sphere>& p_s) {
		vp_spheres.push_back(p_s);
		return;
	}

	bool intersect(const Ray& ray, Hit& res) const {
		bool is_hit = false;
		for (auto p_s : vp_spheres) {
			Hit res_temp;
			if (p_s->intersect(ray, res_temp)) {
				if (res_temp.distance < res.distance) {
					is_hit = true;
					res = res_temp;
				}
			}
		}
		return is_hit;
	}
};