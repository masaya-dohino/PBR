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
	//BRDF ����������T���v�����O����Ɠ�����BRDF�̒l���v�Z���ĕԂ�
	//�߂�l�FBRDF�̒l
	// wi�F�T���v�����O���ꂽ����
	// pdf�F�T���v�����O���ꂽ�����̊m�����x�ϐ�
	virtual Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const = 0;
};

class Diffuse : public Material {
public:
	Vec3 rho; //���˗�

	Diffuse(const Vec3& _rho) : rho(_rho) {};

	Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
		double u = rnd();
		double v = rnd();

		//�ƁA���̌v�Z
		double theta = 0.5 * std::acos(1.0 - 2.0*u);
		double phi = 2 * M_PI * v;
		//�ix,y,z�j�̕����x�N�g���̌v�Z
		double x = std::cos(phi)*std::sin(theta);
		double y = std::cos(theta);
		double z = std::sin(phi)*std::sin(theta);
		//�T���v�����O���ꂽ����
		wi = Vec3(x, y, z);

		//�m�����x�֐��̒l
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
	double ior; //���ܗ�
	Glass(double _ior) : ior(_ior) {};
	Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
		//�K���X�ɓ��˂��Ă��邩�̃t���O
		bool isEntering = cosTheta(wo) > 0;
		//���˂Əo�˂ŋ��ܗ��A�@�������ւ���
		double n1; //���ˑ����ܗ�
		double n2; //�o�ˑ����ܗ�
		Vec3 normal; //�@�� ���˂Əo�˂œ���ւ���K�v������
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
		//�t���l�����˗�
		double fr = fresnel(wo, normal, n1, n2);
		//����
		if (rnd() < fr) {
			wi = reflect(wo, normal);
			pdf = fr;
			return fr / absCosTheta(wi) * Vec3(1);
		}
		//����
		else {
			if (refract(wo, wi, normal, n1, n2)) {
				pdf = 1 - fr;
				return std::pow(n1 / n2, 2.0) * (1 - fr) / absCosTheta(wi) * Vec3(1);
			}
			//�S����
			else {
				wi = reflect(wo, normal);
				pdf = 1 - fr;
				return (1 - fr) / absCosTheta(wi) * Vec3(1);
			}
		}
	};
};
#endif
