#pragma once
#include "Vec3.h"

class Light {
public:
	Vec3 color;//�����̐F

	Light(const Vec3& _color) : color(_color) {};

	//�����_�����O��������Le�ɑΉ����镔��
	//���ׂĂ̕����ɓ��������ˋP�x��Ԃ��̂ŁA�P���ɕێ����Ă���F��Ԃ������ɂȂ�

	Vec3 Le() const {
		return color;
	}
};