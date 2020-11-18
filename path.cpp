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

#define MAX_DEPTH 500 //�ő唽�ˉ�
#define ROULETTE 0.9 //���V�A�����[���b�g�̊m��



//ray �̕������痈����ˋP�x�̒l���v�Z���ĕԂ�
//init_ray �ŏ��̃��C
// aggregate�F���̏W��

Vec3 radiance(const Ray& init_ray, const Aggregate& aggregate) {
	Vec3 col;//�ŏI�I�ɕԂ��F
	Vec3 throughput(1.0); //�X���[�v�b�g
	Ray ray = init_ray; //�X�V���Ă������C

	//�����̕]��
	for (int depth = 0; depth < MAX_DEPTH; ++depth) {
		Hit res;
		//���C���V�[���ƏՓ˂����ꍇ
		if (aggregate.intersect(ray, res)) {
			Vec3 n = res.hitNormal; //�@��

			//���[�J�����W�n�̍\�z
			Vec3 s, t;
			orthonormalBasis(n, s, t);

			//�o�˕��������[�J�����W�n�ɕϊ�
			Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);

#if 1		//�}�e���A���ƌ���
			const Sphere* sphere = (Sphere*)res.hitSphere;
			auto hitMaterial = sphere->p_material;
			auto hitLight = sphere->p_light;
#else
			auto hitMaterial = res.hitSphere->p_material;
			auto hitLight = res.hitSphere->p_light;
#endif
			//Le�̌v�Z
			col += throughput * hitLight->Le();

			//�����̃T���v�����O��BRDF�̕]��
			Vec3 brdf;
			Vec3 wi_local;
			double pdf;
			brdf = hitMaterial->sample(wo_local, wi_local, pdf);

			//cos
			double cos = cosTheta(wi_local);
			//�T���v�����O���ꂽ���������[���h���W�n�ɕϊ�
			Vec3 wi = localToWorld(wi_local, s, n, t);

			//�X���[�v�b�g�̍X�V
			throughput *= brdf * cos / pdf;
			//���̃��C�𐶐�
			ray = Ray(res.hitPos + 0.001*res.hitNormal, wi);
		}
		//��ɔ��ōs�����ꍇ
		else {
#if 0
			col += throughput * Vec3(0.0);//��̐F����
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
	auto mat1 = std::make_shared<Diffuse>(Vec3(0.9)); //���F
	//auto mat1 = std::make_shared<Mirror>();
	// object
	auto mat2 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8)); //�F


	// floor
	auto light1 = std::make_shared<Light>(Vec3(0.0));
	// object 
	auto light2 = std::make_shared<Light>(Vec3(0.0));
	// auto light2 = std::make_shared<Light>(Vec3(0.2, 0.2, 0.8));//��������点��

	Aggregate aggregate;

	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, -10001.0, -0.0), 10000.0, mat1, light1)); //��
	aggregate.add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -3.0), 1.0, mat2, light2));//��

	Vec3 sun_direction = normalize(Vec3(1.0, 1.0, 1.0));

#ifdef ENABLE_OPENMP
#pragma omp parallel for schedule(dynamic, 1)
#endif
	for (int r = 0; r < img.height; ++r) {
		for (int c = 0; c < img.width; ++c) {
			Vec3 col(0.0);

			for (int i = 0; i < N; ++i) {
				//(u,v)�̌v�Z
				double u = (2.0 * (c + rnd()) - img.width) / img.width;
				double v = (2.0 * (r + rnd()) - img.height) / img.height;
				//���C�𐶐�
				Ray ray = cam.getRay(-u, -v);
				//���ˋP�x���v�Z
				col += radiance(ray, aggregate);
			}
			//�T���v�������Z�A�T���v�����O���ŏ��Z
			img.setPixel(r, c, col / static_cast<double>(N));

//�i���󋵂̏o��
#ifdef ENABLE_OPENMP
			if (omp_get_thread_num() == 0)
#endif
				std::cout << double(c + r * img.height) / (img.width*img.height) * 100.0
				<< "\r" << std::flush;
		}
	}

	img.gammaCorrection();//�K���}�␳

	img.writePPM("path_tracing1.ppm");

	return 0;
}
catch (const char* e)
{
	std::cerr << e << std::endl;
	return 1;
}
