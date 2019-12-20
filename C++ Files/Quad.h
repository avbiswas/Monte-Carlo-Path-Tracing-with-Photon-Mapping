#pragma once
#ifndef QUAD_H

#define QUAD_H

#include "ray_utils.h"
#include "Plane.h"
# include <random>
class Quad : public Objects {
public:
	Material material;
	string name;
	Vec3f A, B, C, D;
	Vec3f normal_;
	Plane* plane;
	Quad(Vec3f a, Vec3f b, Vec3f c, Vec3f d, Material mat) : A(a), B(b), C(c), D(d), material(mat) {
		normal_ = (b - a).cross(a - c).normalize();
		plane = new Plane(a, (a - b).cross(a - c).normalize());;
	}
	Quad(Vec3f a, Vec3f b, Vec3f c, Vec3f d, Material mat, string n) : A(a), B(b), C(c), D(d), material(mat), name(n) {
		normal_ = (b - a).cross(a - c).normalize();
		plane = new Plane(a, (a - b).cross(a - c).normalize());;
	}

	Material getMaterial() {
		return material;
	}
	string getType() {
		return "Quad";
	}
	string getID() {
		return name;
	}
	float hit(Ray& r, bool bothSides) {
		Vec3f nullHit(NULL, NULL, NULL);


		float t = plane->hit(r, false);
		if (t >= 0) {
			int sign = (r.point - A).dot(normal_) > 0 ? 1 : -1;
			Vec3f hitPoint = r.point + r.direction * t;
			//cout << "Hits Plane at : " << hitPoint << endl;
			Vec3f edge1 = B - A;
			Vec3f edge2 = C - B;
			Vec3f edge3 = D - C;
			Vec3f edge4 = A - D;
			Vec3f v1 = hitPoint - B;
			Vec3f v2 = hitPoint - C;
			Vec3f v3 = hitPoint - D;
			Vec3f v4 = hitPoint - A;


			if (normal_.dot(edge1.cross(v1)) >= 0
				&& normal_.dot(edge2.cross(v2)) >= 0
				&& normal_.dot(edge3.cross(v3)) >= 0
				&& normal_.dot(edge4.cross(v4)) >= 0) {

				return t;

			}

			if (normal_.dot(edge1.cross(v1)) <= 0
				&& normal_.dot(edge2.cross(v2)) <= 0
				&& normal_.dot(edge3.cross(v3)) <= 0
				&& normal_.dot(edge4.cross(v4)) <= 0) {

				return t;

			}

		}
		return 0;

	}
	Vec3f normal(Vec3f& point) {
		return normal_;
	}
	Vec3f samplePointLight(float rand1, float rand2, float rand3) {
		// Bilinear Interpolation
		Vec3f p1 = A * rand1 + B * (1 - rand1);
		Vec3f p2 = C * rand2 + D * (1 - rand2);
		return p1 * rand3 + p2 * (1 - rand3);
	}
	float computeArea() {
		return (B - A).cross(A - C).length();
	}
};

#endif