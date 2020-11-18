/*
#include <iostream>
#include <memory>

#ifdef ENABLE_OPENMP
#include <omp.h>
#endif

#include "Vec3.h"
#include "Ray.h"
#include "Image.h"
#include "Camera.h"
#include "Hit.h"
#include "Sphere.h"
#include "Aggregate.h"
#include "Material.h"

using namespace std;

#define MAX_DEPTH 500
#define ROULETTE 0.99

Vec3 radiance(const Ray& init_ray, const Aggregate& aggregate) {
	Vec3 col;
	Vec3 throughput(1.0);
	Ray ray = init_ray;

	for (int depth = 0; depth < MAX_DEPTH; ++depth) {
		Hit res;

		if (aggregate.intersect(ray, res)) {
			Vec3 n = res.hitNormal;
			Vec3 s, t;
			orthonormalBasis(n, s, t);

			Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);

#if 1
			const Sphere* sphere = (Sphere*)res.hitSphere;
			auto hitMaterial = sphere->p_material;
			auto hitLight = sphere->p_light;
#else
			auto hitMaterial = res.hitSphere->p_material;
			auto hitLight = res.hitSphere->p_light;
#endif

			col += throughput * hitLight->Le();

			Vec3 brdf;
			Vec3 wi_local;
			double pdf;
			brdf = hitMaterial->sample(wo_local, wi_local, pdf);

			double cos = cosTheta(wi_local);
			Vec3 wi = localToWorld(wi_local, s, n, t);

			throughput *= brdf * cos / pdf;

			ray = Ray(res.hitPos + 0.001*res.hitNormal, wi);
		}
		else {
#if 1
			col += throughput * Vec3(0.0);
#else
			col += throughput * Vec3(1.0);
#endif
			break;
		}

		if (rnd() > ROULETTE) {
			break;
		}
		else {
			throughput /= ROULETTE;
		}
	}

	return col;
}

auto main(int argc, char* argv[]) -> int
try
{
	const int height = 512;
	const int width = 512;

	const int N = 500;

	Image img(height, width);
	PinholeCamera cam(Vec3(0, 0, 4), Vec3(0, 0, -1), 1);

#if 0
	// floor
	// auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));
	auto mat1 = std::make_shared<Mirror>();
	// object
	auto mat2 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8));


	// floor
	auto light1 = std::make_shared<Light>(Vec3(0.0));
	// object 
	auto light2 = std::make_shared<Light>(Vec3(0.0));
	// auto light2 = std::make_shared<Light>(Vec3(0.2, 0.2, 0.8));

	Aggregate aggregate;

	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, -10001.0, -0.0), 10000.0, mat1, light1));
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -3.0), 1.0, mat2, light2));
#else
	auto mat1 = std::make_shared<Diffuse>(Vec3(0.8));
	auto mat2 = std::make_shared<Diffuse>(Vec3(0.8, 0.2, 0.2));
	auto mat3 = std::make_shared<Diffuse>(Vec3(0.2, 0.8, 0.2));

	auto light1 = std::make_shared<Light>(Vec3(0.0));
	auto light2 = std::make_shared<Light>(Vec3(5.0));

	Aggregate aggregate;
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, -10001.0, 0.0), 10000.0, mat1, light1));
	aggregate.add(std::make_shared<Sphere>(Vec3(10003.0, 0.0, 0.0), 10000.0, mat2, light1));
	aggregate.add(std::make_shared<Sphere>(Vec3(-10003.0, 0.0, 0.0), 10000.0, mat3, light1));
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, 10003.0, 0.0), 10000.0, mat1, light1));
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -10003.0), 10000.0, mat1, light1));
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, 0.0, 0.0), 1.0, mat1, light1));
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, 3.0, 0.0), 1.0, mat1, light2));

#endif

	Vec3 sun_direction = normalize(Vec3(1.0, 1.0, 1.0));

#pragma omp parallel for schedule(dynamic, 1)
	for (int r = 0; r < img.height; ++r) {
		for (int c = 0; c < img.width; ++c) {
			Vec3 col(0.0);

			for (int i = 0; i < N; ++i) {
				double u = (2.0 * (c + rnd()) - img.width) / img.width;
				double v = (2.0 * (r + rnd()) - img.height) / img.height;

				Ray ray = cam.getRay(-u, -v);
				col += radiance(ray, aggregate);
			}

			img.setPixel(r, c, col / static_cast<double>(N));

#ifdef ENABLE_OPENMP
			if (omp_get_thread_num() == 0)
#endif
				std::cout << double(c + r * img.height) / (img.width*img.height) * 100.0
				<< "\r" << std::flush;
		}
	}

	img.gammaCorrection();

	img.writePPM("cornell_box.ppm");

	return 0;
}
catch (const char* e)
{
	std::cerr << e << std::endl;
	return 1;
}
*/