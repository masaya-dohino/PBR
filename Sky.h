#pragma once
#ifndef SKY_H
#define SKY_H
#include <string>
#include "Vec3.h"
#include "Ray.h"
#endif


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

class Sky {
public:
	//受け取ったレイの方向から来る空の放射輝度の値を返す。
	//戻り値：空の放射輝度
	//ray：放射輝度を取る方向のレイ
	Sky() {};
	virtual Vec3 getRadiance(const Ray& ray) const = 0;
};

class UniformSky : public Sky {
public:
	Vec3 color;
	UniformSky(const Vec3& _color) : color(_color) {};

	Vec3 getRadiance(const Ray& ray) const {
		return color;
	}
};

class SimpleSky : public Sky {
public:
	SimpleSky() {};

	Vec3 getRadiance(const Ray& ray) const {
		double t = (ray.direction.y + 1.0) / 2.0;
		return (1.0 - t)*Vec3(1.0) + t * Vec3(0.7, 0.8, 1.0);
	}
};

class IBL : public Sky {
public:
	int width;
	int height;
	float HDR[262145];
	float *hdr_image;
	

	IBL(const std::string& filename) {
		int n;
		hdr_image = stbi_loadf(filename.c_str(), &width, &height, &n, 0);
		std::cout << hdr_image << std::endl;
	};

	~IBL() {
		stbi_image_free(hdr_image);
	};

	Vec3 getRadiance(const Ray& ray) const {
		double theta = std::acos(ray.direction.y);
		double phi = std::atan2(ray.direction.z, ray.direction.x);

		if (phi < 0) {
			phi += 2.0*M_PI;
		}

		int i = phi / (2.0*M_PI) * width;
		int j = theta / M_PI * height;

		int index = 3.0 * i + 3.0*width*j;
		return Vec3(0,0,0);
		//return Vec3(hdr_image[index], hdr_image[index+1], hdr_image[index+2]);
	}

};