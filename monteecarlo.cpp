/*
#include<iostream>
#include<cmath>
#include<random>
#include"Random.h"


//��ϕ��֐�1
inline double f(double x, double y) {
	return x * x + y * y;
}

//��ϕ��֐�2
inline double g(double x) {
	return(x + 1)*std::cos(x);
}

//�m�����x�֐�
inline double p(double x) {
	return std::asin(x);
}

//�ݐϕ��z�֐��̋t�֐�
inline double Pinv(double x) {
	return std::asin(x);
}

void montecarlo() {
	//�T���v�����O��
	const int N = 10000;

	//�����e�J�����ϕ��̌v�Z
	double sum = 0;
	for (int i = 0; i < N; i++) {
		double x = rnd();
		double y = rnd();
		sum += f(x, y);
	}
	sum /= N;
	std::cout << sum << std::endl;

}

void importance() {
	//�d�_�I�T���v�����O�ɂ�郂���e�J�����ϕ�
	const int N = 10000;

	//�����e�J�����ϕ��̌v�Z
	double sum = 0;
	for (int i = 0; i < N; i++) {
		//��l����
		double u = rnd();
		//�T���v�����O�_
		double x = Pinv(u);
		//�����e�J�����ϕ��̒��̌���
		sum += g(x) / p(x);
	}
	sum /= N;
	std::cout << sum << std::endl;
}
*/