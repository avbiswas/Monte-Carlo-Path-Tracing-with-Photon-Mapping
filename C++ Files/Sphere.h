#pragma once
#include "ray_utils.h"
#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Objects{
public:
	 Vec3f center;
	float radius;
	Material material;
	string name;
	Sphere() :center( Vec3f(0, 0, 0)), radius(1) {}
	Sphere( Vec3f c, float r, Material m) : center(c), radius(r), material(m) {}
	Sphere( Vec3f c, float r, Material m, string n) : center(c), radius(r), material(m), name(n) {}

	float hit(Ray& r, bool bothSides);
	Material getMaterial();
	 Vec3f normal( Vec3f& point) {
		return (point - center).normalize();
	}
	string getType() {
		return "Sphere";
	}
	string getID() {
		return name;
	}
	float discriminant( Vec3f d,  Vec3f o, float* A, float* B, float* C);
	 Vec3f samplePointLight(float rand1, float rand2, float rand3) {
		 Vec3f randomUnitVector(rand1, rand2, rand3);
		randomUnitVector = randomUnitVector.normalize();
		return center + randomUnitVector * radius;

	}
	float computeArea() {
		return 4 * M_PI * (radius * radius); // Area of visible hemisphere
	}
};
#endif // !SPHERE_H

