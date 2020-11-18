#pragma once
#include "Random.h"
#include"Vec3.h"
#define M_PI 3.1415

double absCosTheta(const Vec3& v) {
	return std::abs(v.y);
}

double cosTheta(const Vec3& v) {
	return v.y;
}

bool refract(const Vec3& v, Vec3& r, const Vec3& n, const double n1, const double n2) {
	double cos = absCosTheta(v);
	double sin = std::sqrt(std::max(1.0 - cos * cos, 0.0));
	double alpha = n1 / n2 * sin;

	if (alpha*alpha > 1.0) {
		return false;
	}

	r = n1 / n2 * (-v + dot(v, n)*n) - std::sqrt(1.0 - alpha * alpha)*n;

	return true;
}

double fresnel(const Vec3& v, const Vec3& n, const double n1, const double n2) {
	double f0 = std::pow((n1 - n2) / (n1 + n2), 2.0);
	double cos = absCosTheta(v);
	return f0 + (1.0 - f0)*std::pow(1.0 - cos, 5.0);
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
	return -v + 2.0*dot(v, n)*n;
}

class Material {
public:
	//BRDF から方向をサンプリングすると同時にBRDFの値を計算して返す
	//戻り値：BRDFの値
	// wi：サンプリングされた方向
	// pdf：サンプリングされた方向の確率密度変数
	virtual Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const = 0;
};

class Diffuse : public Material {
public:
	Vec3 rho; //反射率

	Diffuse(const Vec3& _rho) : rho(_rho) {};

	Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
		double u = rnd();
		double v = rnd();

		//θ、Φの計算
		double theta = 0.5 * std::acos(1.0 - 2.0*u);
		double phi = 2 * M_PI * v;
		//（x,y,z）の方向ベクトルの計算
		double x = std::cos(phi)*std::sin(theta);
		double y = std::cos(theta);
		double z = std::sin(phi)*std::sin(theta);
		//サンプリングされた方向
		wi = Vec3(x, y, z);

		//確率密度関数の値
		pdf = std::cos(theta) / M_PI;

		return rho / M_PI;
	}
};

class Mirror : public Material {
public:
	Mirror() {};

	Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
		wi = reflect(wo, Vec3(0.0, 1.0, 0.0));

		pdf = 1.0;

		return 1.0 / cosTheta(wi) * Vec3(1.0);
	}
};

#if 1
class Glass : public Material {
public:
	double ior;
	Glass(double _ior) : ior(_ior) {};
	~Glass() {};

	Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
		bool isEntering = cosTheta(wo) > 0.0;

		double n1, n2;
		Vec3 normal;

		if (isEntering) {
			n1 = 1.0;
			n2 = ior;
			normal = Vec3(0, 1, 0);
		}
		else {
			n1 = ior;
			n2 = 1.0;
			normal = Vec3(0, -1, 0);
		}

		double fr = fresnel(wo, normal, n1, n2);

		if (rnd() < fr) {
			wi = reflect(wo, normal);
			pdf = fr;
			return fr / absCosTheta(wi) * Vec3(1);
		}
		else {
			pdf = 1.0 - fr;
			if (refract(wo, wi, normal, n1, n2)) {
				return std::pow(n1 / n2, 2.0)*(1.0 - fr) / absCosTheta(wi)*Vec3(1);
			}
			else {
				wi = reflect(wo, normal);
				return (1.0 - fr) / absCosTheta(wi)*Vec3(1);
			}
		}
	}
};
#else
class Glass : public Material {
public:
	double ior; //屈折率
	Glass(double _ior) : ior(_ior) {};
	Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
		//ガラスに入射しているかのフラグ
		bool isEntering = cosTheta(wo) > 0;
		//入射と出射で屈折率、法線を入れ替える
		double n1; //入射側屈折率
		double n2; //出射側屈折率
		Vec3 normal; //法線 入射と出射で入れ替える必要がある
		if (isEntering) {
			n1 = 1.0;
			n2 = ior;
			normal = Vec3(0, 1, 0);
		}
		else {
			n1 = ior;
			n2 = 1.0;
			normal = Vec3(0, -1, 0);
		}
		//フレネル反射率
		double fr = fresnel(wo, normal, n1, n2);
		//反射
		if (rnd() < fr) {
			wi = reflect(wo, normal);
			pdf = fr;
			return fr / absCosTheta(wi) * Vec3(1);
		}
		//屈折
		else {
			if (refract(wo, wi, normal, n1, n2)) {
				pdf = 1 - fr;
				return std::pow(n1 / n2, 2.0) * (1 - fr) / absCosTheta(wi) * Vec3(1);
			}
			//全反射
			else {
				wi = reflect(wo, normal);
				pdf = 1 - fr;
				return (1 - fr) / absCosTheta(wi) * Vec3(1);
			}
		}
	};
};
#endif
