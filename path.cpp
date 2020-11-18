#include <iostream>
#include <memory>


#include <omp.h>


#include "Vec3.h"
#include "Ray.h"
#include "Image.h"
#include "Camera.h"
#include "Hit.h"
#include "Sphere.h"
#include "Aggregate.h"
#include "Material.h"

using namespace std;

#define MAX_DEPTH 500 //最大反射回数
#define ROULETTE 0.9 //ロシアンルーレットの確率



//ray の方向から来る放射輝度の値を計算して返す
//init_ray 最初のレイ
// aggregate：物体集合

Vec3 radiance(const Ray& init_ray, const Aggregate& aggregate) {
	Vec3 col;//最終的に返す色
	Vec3 throughput(1.0); //スループット
	Ray ray = init_ray; //更新していくレイ

	//級数の評価
	for (int depth = 0; depth < MAX_DEPTH; ++depth) {
		Hit res;
		//レイがシーンと衝突した場合
		if (aggregate.intersect(ray, res)) {
			Vec3 n = res.hitNormal; //法線

			//ローカル座標系の構築
			Vec3 s, t;
			orthonormalBasis(n, s, t);

			//出射方向をローカル座標系に変換
			Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);

#if 1		//マテリアルと光源
			const Sphere* sphere = (Sphere*)res.hitSphere;
			auto hitMaterial = sphere->p_material;
			auto hitLight = sphere->p_light;
#else
			auto hitMaterial = res.hitSphere->p_material;
			auto hitLight = res.hitSphere->p_light;
#endif
			//Leの計算
			col += throughput * hitLight->Le();

			//方向のサンプリングとBRDFの評価
			Vec3 brdf;
			Vec3 wi_local;
			double pdf;
			brdf = hitMaterial->sample(wo_local, wi_local, pdf);

			//cos
			double cos = cosTheta(wi_local);
			//サンプリングされた方向をワールド座標系に変換
			Vec3 wi = localToWorld(wi_local, s, n, t);

			//スループットの更新
			throughput *= brdf * cos / pdf;
			//次のレイを生成
			ray = Ray(res.hitPos + 0.001*res.hitNormal, wi);
		}
		//空に飛んで行った場合
		else {
#if 0
			col += throughput * Vec3(0.0);//空の色が黒
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
	PinholeCamera cam(Vec3(0, 0, 1), Vec3(0, 0, -1), 1);

	// floor
	auto mat1 = std::make_shared<Diffuse>(Vec3(0.9)); //白色
	//auto mat1 = std::make_shared<Mirror>();
	// object
	auto mat2 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8)); //青色


	// floor
	auto light1 = std::make_shared<Light>(Vec3(0.0));
	// object 
	auto light2 = std::make_shared<Light>(Vec3(0.0));
	// auto light2 = std::make_shared<Light>(Vec3(0.2, 0.2, 0.8));//球を青く光らせる

	Aggregate aggregate;

	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, -10001.0, -0.0), 10000.0, mat1, light1)); //床
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -3.0), 1.0, mat2, light2));//球

	Vec3 sun_direction = normalize(Vec3(1.0, 1.0, 1.0));

#ifdef ENABLE_OPENMP
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (int r = 0; r < img.height; ++r) {
		for (int c = 0; c < img.width; ++c) {
			Vec3 col(0.0);

			for (int i = 0; i < N; ++i) {
				//(u,v)の計算
				double u = (2.0 * (c + rnd()) - img.width) / img.width;
				double v = (2.0 * (r + rnd()) - img.height) / img.height;
				//レイを生成
				Ray ray = cam.getRay(-u, -v);
				//放射輝度を計算
				col += radiance(ray, aggregate);
			}
			//サンプルを加算、サンプリング数で除算
			img.setPixel(r, c, col / static_cast<double>(N));

//進捗状況の出力
#ifdef ENABLE_OPENMP
			if (omp_get_thread_num() == 0)
#endif
				std::cout << double(c + r * img.height) / (img.width*img.height) * 100.0
				<< "\r" << std::flush;
		}
	}

	img.gammaCorrection();//ガンマ補正

	img.writePPM("path_tracing1.ppm");

	return 0;
}
catch (const char* e)
{
	std::cerr << e << std::endl;
	return 1;
}
