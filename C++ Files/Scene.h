#pragma once
#include "ray_utils.h"
#include <vector>
#include "Sphere.h"
#include "Quad.h"
#include "Triangle.h"
#include "Presets.h"
#ifndef SCENE_H
#define SCENE_H
class Scene {
public:
	Scene(){}
	void getScene(int choice, int WIDTH, int HEIGHT, int far_Z, vector<Objects*>& objects, vector<Objects*>& lights, vector<Objects*>& caustics) {
		int back_mirror_margin = 15;
		float light_height = HEIGHT - 0.1;

		switch (choice) {
		case 1:
			objects.push_back(new Quad(Vec3f(WIDTH - 5, back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, far_Z - 10), Vec3f(WIDTH - 5, back_mirror_margin, far_Z - 10), PERFECT_MIRROR, "RIGHT MIRROR"));

			//RIGHT_WALL
			objects.push_back(new Quad(Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, HEIGHT, 0), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, 0, far_Z), BLUE_WALL, "RIGHT MIRROR"));
			//CEILING
			objects.push_back(new Quad(Vec3f(0, HEIGHT, 0), Vec3f(0, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, 0), WHITE_WALL, "CEILING"));
			light_height = HEIGHT - 0.1;
			objects.push_back(new Quad(Vec3f(back_mirror_margin, light_height, back_mirror_margin), Vec3f(back_mirror_margin, light_height, far_Z - back_mirror_margin), Vec3f(WIDTH - back_mirror_margin, light_height, far_Z - back_mirror_margin), Vec3f(WIDTH - back_mirror_margin, light_height, back_mirror_margin), PERFECT_MIRROR, "CEILING"));
			//FLOOR
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, 0, far_Z), Vec3f(0, 0, far_Z), GRAY_WALL, "FLOOR"));


			//BACK WALL
			objects.push_back(new Quad(Vec3f(0, 0, far_Z), Vec3f(WIDTH, 0, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(0, HEIGHT, far_Z), RED_WALL, "BACK WALL"));


			//LEFT WALL
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(0, 0, far_Z), Vec3f(0, HEIGHT, far_Z), Vec3f(0, HEIGHT, 0), YELLOW_WALL, "LEFT WALL"));


			objects.push_back(new Quad(Vec3f(WIDTH / 6, HEIGHT * 3 / 8, far_Z / 4), Vec3f(WIDTH / 6, HEIGHT * 3 / 8, far_Z * 3 / 4), Vec3f(WIDTH / 3, 0, far_Z * 3 / 4), Vec3f(WIDTH / 3, 0, far_Z / 4), GREEN_WALL, "LEFT SLAB"));
			objects.push_back(new Sphere(Vec3f(WIDTH / 3 + HEIGHT / 6 + 1, HEIGHT / 6, far_Z / 2 + 20), HEIGHT / 6, YELLOW_WALL, "YELLOW BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH * 2 / 3, HEIGHT / 6 + 1, far_Z / 2 + 10), HEIGHT / 6, GLASS_BALL, "GLASS BALL"));
			objects.push_back(new Sphere(Vec3f(50, 20, far_Z / 2), 30, WHITE_LIGHT_SPHERE, "GLASS BALL"));
			break;


		case 2:
			objects.push_back(new Quad(Vec3f(WIDTH - 5, back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, far_Z - 10), Vec3f(WIDTH - 5, back_mirror_margin, far_Z - 10), PERFECT_MIRROR, "RIGHT MIRROR"));

			//RIGHT_WALL
			objects.push_back(new Quad(Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, HEIGHT, 0), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, 0, far_Z), BLUE_WALL, "RIGHT MIRROR"));
			//CEILING
			objects.push_back(new Quad(Vec3f(0, HEIGHT, 0), Vec3f(0, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, 0), WHITE_WALL, "CEILING"));
			
			//LIGHT
			objects.push_back(new Quad(Vec3f(WIDTH/6, HEIGHT - 0.1, 0), Vec3f(WIDTH / 4, HEIGHT - 0.1, far_Z), Vec3f(WIDTH*3/4, HEIGHT - 0.1, far_Z), Vec3f(WIDTH*5/6, HEIGHT - 0.1, 0), WHITE_LIGHT_WALL, "LIGHT"));


			//FLOOR
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, 0, far_Z), Vec3f(0, 0, far_Z), LIGHT_YELLOW_WALL, "FLOOR"));


			//BACK WALL
			objects.push_back(new Quad(Vec3f(0, 0, far_Z), Vec3f(WIDTH, 0, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(0, HEIGHT, far_Z), RED_WALL, "BACK WALL"));

			//objects.push_back(new Quad(Vec3f(0, 0, far_Z/2), Vec3f(WIDTH, 0, far_Z/2), Vec3f(WIDTH, HEIGHT/2, far_Z/2), Vec3f(0, HEIGHT/2, far_Z/2), GLASS_BALL, "BACK WALL"));
			//objects.push_back(new Quad(Vec3f(WIDTH / 4, 0, far_Z / 3), Vec3f(WIDTH*3/4, 0, far_Z / 3), Vec3f(WIDTH*3/4, HEIGHT / 2, far_Z / 3), Vec3f(WIDTH/4, HEIGHT / 2, far_Z / 3), GLASS_BALL, "BACK WALL"));


			//LEFT WALL
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(0, 0, far_Z), Vec3f(0, HEIGHT, far_Z), Vec3f(0, HEIGHT, 0), YELLOW_WALL, "LEFT WALL"));


			//SLOPE
			//objects.push_back(new Quad(Vec3f(0, HEIGHT*3/4, far_Z/4), Vec3f(WIDTH/3, 0, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z*3 /4), Vec3f(0, HEIGHT*3/4, far_Z *3/4),  GREEN_WALL,"LEFT SLAB"));

			objects.push_back(new Sphere(Vec3f(WIDTH * 2 / 3, HEIGHT / 5, far_Z / 2), HEIGHT / 5, GLASS_BALL, "GLASS BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH * 1 / 3, HEIGHT / 4, far_Z / 2 - 10), HEIGHT / 5, YELLOW_BALL, "YELLOW BALL"));
			//objects.push_back(new Sphere(Vec3f(WIDTH * 1 / 3, HEIGHT / 4, far_Z / 2 - 10), HEIGHT / 5, YELLOW_WALL, "YELLOW BALL"));

			break;
		case 3:
			//RIGHT_WALL
			objects.push_back(new Quad(Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, HEIGHT, 0), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, 0, far_Z), BLUE_WALL, "RIGHT MIRROR"));
			//CEILING
			objects.push_back(new Quad(Vec3f(0, HEIGHT, 0), Vec3f(0, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, 0), WHITE_WALL, "CEILING"));

			//LIGHT
			objects.push_back(new Quad(Vec3f(WIDTH / 6, HEIGHT - 0.1, far_Z / 4), Vec3f(WIDTH / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 3 / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 5 / 6, HEIGHT - 0.1, far_Z / 4), WHITE_LIGHT_WALL, "LIGHT"));


			//FLOOR
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, 0, far_Z), Vec3f(0, 0, far_Z), LIGHT_YELLOW_WALL, "FLOOR"));


			//BACK WALL
			objects.push_back(new Quad(Vec3f(0, 0, far_Z), Vec3f(WIDTH, 0, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(0, HEIGHT, far_Z), RED_WALL, "BACK WALL"));


			//LEFT WALL
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(0, 0, far_Z), Vec3f(0, HEIGHT, far_Z), Vec3f(0, HEIGHT, 0), YELLOW_WALL, "LEFT WALL"));


			//SLOPE
			objects.push_back(new Quad(Vec3f(0, HEIGHT * 3 / 4, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z * 3 / 4), Vec3f(0, HEIGHT * 3 / 4, far_Z * 3 / 4), GREEN_WALL, "LEFT SLAB"));

			
			objects.push_back(new Sphere(Vec3f(WIDTH / 3 + HEIGHT / 6 + 1, HEIGHT / 6, far_Z / 2 + 20), HEIGHT / 6, YELLOW_WALL, "YELLOW BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH * 2 / 3 + 20, HEIGHT / 6 + 1, far_Z / 2 + 10), HEIGHT / 6, GLASS_BALL, "GLASS BALL"));
			break;
		case 4:
			objects.push_back(new Sphere(Vec3f(WIDTH * 3 / 4, HEIGHT / 2 + 10, far_Z * 2/3), HEIGHT / 8, WHITE_LIGHT_WALL, "LIGHT_BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH / 2 , HEIGHT / 4, far_Z / 2 ), HEIGHT / 4, CYAN_GLASS, "GLASS_BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH * 1 / 4, HEIGHT / 2 + 10, far_Z * 2/3), HEIGHT / 8, WHITE_LIGHT_WALL, "LIGHT_BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH *3/ 4 + 40, HEIGHT / 5, far_Z * 2 / 3), HEIGHT / 5, YELLOW_GLASS, "GLASS_BALL"));



			//FLOOR
			objects.push_back(new Quad(Vec3f(-WIDTH/1.5, 0, 0), Vec3f(WIDTH*1.5, 0, 0), Vec3f(WIDTH*1.5, 0, 1.5* far_Z), Vec3f(-WIDTH / 1.5, 0, 1.5*far_Z), GRAY_WALL, "FLOOR"));
			
			break;
		case 5:
			//objects.push_back(new Quad(Vec3f(WIDTH - 5, back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, far_Z - 30), Vec3f(WIDTH - 5, back_mirror_margin, far_Z - 30), PERFECT_MIRROR, "RIGHT MIRROR"));

			//RIGHT_WALL
			//objects.push_back(new Quad(Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, HEIGHT, 0), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, 0, far_Z), BLUE_WALL, "RIGHT MIRROR"));
			//CEILING
			//objects.push_back(new Quad(Vec3f(0, HEIGHT, 0), Vec3f(0, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, 0), WHITE_WALL, "CEILING"));

			//LIGHT
			objects.push_back(new Quad(Vec3f(WIDTH / 6, HEIGHT - 0.1, far_Z / 4), Vec3f(WIDTH / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 3 / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 5 / 6, HEIGHT - 0.1, far_Z / 4), WHITE_LIGHT_WALL, "LIGHT"));


			//FLOOR
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, 0, far_Z), Vec3f(0, 0, far_Z), LIGHT_YELLOW_WALL, "FLOOR"));


			//BACK WALL
			//objects.push_back(new Quad(Vec3f(0, 0, far_Z), Vec3f(WIDTH, 0, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(0, HEIGHT, far_Z), RED_WALL, "BACK WALL"));


			//LEFT WALL
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(0, 0, far_Z), Vec3f(0, HEIGHT, far_Z), Vec3f(0, HEIGHT, 0), YELLOW_WALL, "LEFT WALL"));


			//SLOPE
			objects.push_back(new Quad(Vec3f(0, HEIGHT * 3 / 4, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z * 3 / 4), Vec3f(0, HEIGHT * 3 / 4, far_Z * 3 / 4), GREEN_GLASS, "LEFT SLAB"));


			objects.push_back(new Sphere(Vec3f(WIDTH / 3 + HEIGHT / 8 + 1, HEIGHT / 10, far_Z / 2 + 20), HEIGHT / 8, PERFECT_MIRROR, "YELLOW BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH * 2 / 3, HEIGHT / 10 + 1, far_Z / 3 + 10), HEIGHT / 10, GREEN_WALL, "GLASS BALL"));
			break;

		case 6:
			//RIGHT_WALL
			objects.push_back(new Quad(Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, HEIGHT, 0), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, 0, far_Z), GREEN_WALL, "RIGHT MIRROR"));
			//CEILING
			objects.push_back(new Quad(Vec3f(0, HEIGHT, 0), Vec3f(0, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, 0), GRAY_WALL, "CEILING"));
			//LIGHT
			objects.push_back(new Quad(Vec3f(WIDTH / 6, HEIGHT - 0.25, far_Z / 8), Vec3f(WIDTH / 4, HEIGHT - 0.25, far_Z * 7 / 8), Vec3f(WIDTH * 3 / 4, HEIGHT - 0.25, far_Z * 7 / 8), Vec3f(WIDTH * 5 / 6, HEIGHT - 0.25, far_Z / 8), WHITE_LIGHT_WALL, "LIGHT"));
			//FLOOR
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, 0, far_Z), Vec3f(0, 0, far_Z), GRAY_WALL, "FLOOR"));
			//BACK WALL
			objects.push_back(new Quad(Vec3f(0, 0, far_Z), Vec3f(WIDTH, 0, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(0, HEIGHT, far_Z), LIGHT_YELLOW_WALL, "BACK WALL"));
			//LEFT WALL
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(0, 0, far_Z), Vec3f(0, HEIGHT, far_Z), Vec3f(0, HEIGHT, 0), YELLOW_WALL, "LEFT WALL"));


			objects.push_back(new Sphere(Vec3f(HEIGHT/5 + 5, HEIGHT / 5 + 1, far_Z / 2 + 10), HEIGHT / 5, WHITE_WALL, "GLASS BALL"));

			objects.push_back(new Sphere(Vec3f(HEIGHT * 3 / 4 - 5, HEIGHT / 4 + 1, far_Z / 2 + 10), HEIGHT / 4, WHITE_WALL, "GLASS BALL"));
			break;
		case 7:
			//objects.push_back(new Quad(Vec3f(WIDTH - 5, back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, 2), Vec3f(WIDTH - 5, HEIGHT - back_mirror_margin, far_Z - 30), Vec3f(WIDTH - 5, back_mirror_margin, far_Z - 30), PERFECT_MIRROR, "RIGHT MIRROR"));

			//RIGHT_WALL
			//objects.push_back(new Quad(Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, HEIGHT, 0), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, 0, far_Z), BLUE_WALL, "RIGHT MIRROR"));
			//CEILING
			//objects.push_back(new Quad(Vec3f(0, HEIGHT, 0), Vec3f(0, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, 0), WHITE_WALL, "CEILING"));

			//LIGHT
			//objects.push_back(new Quad(Vec3f(WIDTH / 6, HEIGHT - 0.1, far_Z / 4), Vec3f(WIDTH / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 3 / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 5 / 6, HEIGHT - 0.1, far_Z / 4), WHITE_LIGHT_WALL2, "LIGHT"));


			//FLOOR
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, 0, far_Z), Vec3f(0, 0, far_Z), LIGHT_YELLOW_WALL, "FLOOR"));


			//BACK WALL
			//objects.push_back(new Quad(Vec3f(0, 0, far_Z), Vec3f(WIDTH, 0, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(0, HEIGHT, far_Z), RED_WALL, "BACK WALL"));


			//LEFT WALL
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(0, 0, far_Z), Vec3f(0, HEIGHT, far_Z), Vec3f(0, HEIGHT, 0), YELLOW_WALL, "LEFT WALL"));


			//SLOPE
			objects.push_back(new Quad(Vec3f(0, HEIGHT * 3 / 4, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z * 3 / 4), Vec3f(0, HEIGHT * 3 / 4, far_Z * 3 / 4), GREEN_GLASS, "LEFT SLAB"));


			objects.push_back(new Sphere(Vec3f(WIDTH / 3 + HEIGHT / 10 + 1, HEIGHT / 10, far_Z / 2 + 20), HEIGHT / 10, WHITE_LIGHT_WALL, "YELLOW BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH * 2 / 3 + 20, HEIGHT / 6 + 1, far_Z / 2 + 10), HEIGHT / 6, PERFECT_MIRROR, "GLASS BALL"));
			break;
		case 8:
			//RIGHT_WALL
			objects.push_back(new Quad(Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, HEIGHT, 0), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, 0, far_Z), RED_WALL, "RIGHT MIRROR"));
			//CEILING
			objects.push_back(new Quad(Vec3f(0, HEIGHT, 0), Vec3f(0, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(WIDTH, HEIGHT, 0), WHITE_WALL, "CEILING"));

			//LIGHT
			//objects.push_back(new Quad(Vec3f(WIDTH / 6, HEIGHT - 0.1, far_Z / 4), Vec3f(WIDTH / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 3 / 4, HEIGHT - 0.1, far_Z * 3 / 4), Vec3f(WIDTH * 5 / 6, HEIGHT - 0.1, far_Z / 4), WHITE_LIGHT_WALL, "LIGHT"));


			//FLOOR
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(WIDTH, 0, 0), Vec3f(WIDTH, 0, far_Z), Vec3f(0, 0, far_Z), LIGHT_YELLOW_WALL, "FLOOR"));


			//BACK WALL
			objects.push_back(new Quad(Vec3f(0, 0, far_Z), Vec3f(WIDTH, 0, far_Z), Vec3f(WIDTH, HEIGHT, far_Z), Vec3f(0, HEIGHT, far_Z), RED_WALL, "BACK WALL"));
			//objects.push_back(new Quad(Vec3f(WIDTH/4, HEIGHT/2, far_Z - 2), Vec3f(WIDTH*3/4, HEIGHT / 2, far_Z - 2), Vec3f(WIDTH*3/4, HEIGHT *3/4, far_Z - 2), Vec3f(WIDTH / 4, HEIGHT *3/4, far_Z - 2), WHITE_LIGHT_WALL, "BACK WALL"));


			//LEFT WALL
			objects.push_back(new Quad(Vec3f(0, 0, 0), Vec3f(0, 0, far_Z), Vec3f(0, HEIGHT/2, far_Z), Vec3f(0, HEIGHT/2, 0), YELLOW_WALL, "LEFT WALL"));


			//SLOPE
			//objects.push_back(new Quad(Vec3f(0, HEIGHT * 3 / 4, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z / 4), Vec3f(WIDTH / 3, 0, far_Z * 3 / 4), Vec3f(0, HEIGHT * 3 / 4, far_Z * 3 / 4), GREEN_WALL, "LEFT SLAB"));
			objects.push_back(new Sphere(Vec3f(-100, HEIGHT + 40,  far_Z / 2), HEIGHT / 16, WHITE_LIGHT_SPHERE, "YELLOW BALL"));
			objects.push_back(new Sphere(Vec3f(WIDTH / 3 + HEIGHT / 6 + 1, HEIGHT / 6, far_Z / 2 + 20), HEIGHT / 6, YELLOW_WALL, "YELLOW BALL"));
			//objects.push_back(new Sphere(Vec3f(WIDTH * 2 / 3 + 20, HEIGHT / 6 + 1, far_Z / 2 + 10), HEIGHT / 6, GLASS_BALL, "GLASS BALL"));
			break;
		}
		for (auto object : objects) {
			if (object->getMaterial().type == EMISSIVE)
				lights.push_back(object);
			if (object->getMaterial().type == MIRROR || object->getMaterial().type == REFRACTIVE)
			//if (object->getMaterial().type == REFRACTIVE)
				caustics.push_back(object);

		}

	}
};
#endif // !SCENE_H
