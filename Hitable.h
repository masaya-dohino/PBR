#pragma once
class Ray;
class Hit;
class Material;
class Light;

#include <memory>

class Hitable {
public:
	virtual bool intersect(const Ray& ray, Hit& res) const = 0;

	std::shared_ptr<Material> p_material;
	std::shared_ptr<Light> p_light;
};