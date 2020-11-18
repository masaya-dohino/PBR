#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <array>

#include "Vec3.h"
#include "Util.h"

class Image {
public:
	const int height;
	const int width;

	Vec3* data;

	Image(int _h, int _w) : height(_h), width(_w) {
		data = new Vec3[height*width];
	}

	~Image() {
		delete[] data;
	}


	Vec3 getPixel(int r, int c) const {
		try {
			if (r < 0 || r >= height) {
				throw "Invalid Index for Col in setPixel";
			}
			else if (c < 0 || c >= width) {
				throw "Invalid Index for Row in setPixel";
			}
			return data[width*r + c];
		}
		catch (const char* e) {
			throw e;
			return Vec3(0.0, 0.0, 0.0);
		}
	}

	Image& setPixel(int r, int c, const Vec3& v) {
		try {
			if (r < 0 || r >= height) {
				throw "Invalid Index for Col in setPixel";
			}
			else if (c < 0 || c >= width) {
				throw "Invalid Index for Row in setPixel";
			}
			data[width*r + c] = v;
			return *this;
		}
		catch (const char* e) {
			throw e;
			return *this;
		}
	}

	void divide(const double& k) {
		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {
				setPixel(r, c, getPixel(r, c) / k);
			}
		}
		return;
	}

	void gammaCorrection() {
		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {
				Vec3 v = getPixel(r, c);
				setPixel(r, c,
					Vec3(std::pow(v.x, 1.0 / 2.2), std::pow(v.y, 1.0 / 2.2), std::pow(v.z, 1.0 / 2.2)));
			}
		}
		return;
	}

	void writePPM(const std::string& filename) const {
		std::ofstream fs(filename);

		fs << "P3\n";
		fs << width << " " << height << "\n";
		fs << 255 << "\n";

		for (int r = 0; r < height; ++r) {
			for (int c = 0; c < width; ++c) {
				Vec3 v = getPixel(r, c);
				const int r = clamp(static_cast<int>(v.x*255.0), 0, 255);
				const int g = clamp(static_cast<int>(v.y*255.0), 0, 255);
				const int b = clamp(static_cast<int>(v.z*255.0), 0, 255);
				fs << r << " " << g << " " << b << "\n";
			}
		}

		fs.close();

		return;
	}

};