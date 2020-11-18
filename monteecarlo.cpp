/*
#include<iostream>
#include<cmath>
#include<random>
#include"Random.h"


//被積分関数1
inline double f(double x, double y) {
	return x * x + y * y;
}

//被積分関数2
inline double g(double x) {
	return(x + 1)*std::cos(x);
}

//確率密度関数
inline double p(double x) {
	return std::asin(x);
}

//累積分布関数の逆関数
inline double Pinv(double x) {
	return std::asin(x);
}

void montecarlo() {
	//サンプリング数
	const int N = 10000;

	//モンテカルロ積分の計算
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
	//重点的サンプリングによるモンテカルロ積分
	const int N = 10000;

	//モンテカルロ積分の計算
	double sum = 0;
	for (int i = 0; i < N; i++) {
		//一様乱数
		double u = rnd();
		//サンプリング点
		double x = Pinv(u);
		//モンテカルロ積分の中の公式
		sum += g(x) / p(x);
	}
	sum /= N;
	std::cout << sum << std::endl;
}
*/