#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include<iostream>
#include<fstream>
#include<cmath>
#include<string>
#include"Vec_3.h"
#include"util.h"

class Image {
public:
	int width; //����
	int height; //�c��
	Vec3*data;//�s�N�Z���̔z��

	Image(int _width, int _height) : width(_width), height(_height) {
		data = new Vec3[width*height];
	};

	~Image() {
		delete[] data;
	};

	//�s�N�Z��(i,j)�̐F�����o��
	Vec3 getPixel(int i, int j) const {
		return data[i + width * j];
	};

	//�s�N�Z��(i,j)�ɐF���Z�b�g����
	void setPixel(int i, int j, const Vec3& c) {
		data[i + width * j] = c ;
	};

	//���ׂẴs�N�Z�������̒l�Ŋ���
	void divide(double k) {
		for (int i = 0; i < width; i++){
			for (int j = 0; j < height; j++) {
				this->setPixel(i, j,this->getPixel(i,j)/k);
			}
		}
	};

	void gamma_correction() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				Vec3 c = this->getPixel(i, j);
				this->setPixel(i, j, Vec3(std::pow(c.x, 1 / 2.2), std::pow(c.y, 1 / 2.2), std::pow(c.z, 1 / 2.2)));
			}
		}
	};
	//PPM�摜�o��
	void ppm_output(const std::string& filename) const {
		std::ofstream file(filename);
		file << "P3" << std::endl;
		file << width << " " << height << std::endl;
		file << "255" << std::endl;
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				Vec3 c = this->getPixel(i, j);
				int r = clamp(int(255 * c.x), 0, 255);
				int g = clamp(int(255 * c.y), 0, 255);
				int b = clamp(int(255 * c.z), 0, 255);
				file << r << " " << g << " " << b << std::endl;
			}
		}
		file.close();
	};

};

#endif