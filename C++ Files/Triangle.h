#pragma once
#include "ray_utils.h"
#include "Plane.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H
class Triangle : public Objects
{
public:
	 Vec3f A, B, C;
	Material material;
	 Vec3f normal_;
	Plane* plane;
	Triangle( Vec3f a,  Vec3f b,  Vec3f c, Material m) :A(a), B(b), C(c), material(m) {
		
		normal_ = (b - a).cross(a - c).normalize();
		plane =  new Plane(a, (a - b).cross(a - c).normalize());;
		
	}
	string getType() {
		return "Triangle";
	}

	Material getMaterial() { return material; }
	 Vec3f normal( Vec3f& point) { return normal_; }
	float hit(Ray &r, bool bothSides) {
		 Vec3f nullHit(NULL, NULL, NULL);

		
		float t = plane->hit(r, false);
		if (t >= 0) {
			int sign = (r.point - A).dot(normal_) > 0 ? 1 : -1;
			 Vec3f hitPoint = r.point + r.direction * t;
			//cout << "Hits Plane at : " << hitPoint << endl;
			 Vec3f edge1 = A - B;
			 Vec3f edge2 = B - C;
			 Vec3f edge3 = C - A;
			 Vec3f v1 = hitPoint - A;
			 Vec3f v2 = hitPoint - B;
			 Vec3f v3 = hitPoint - C;
			if (normal_.dot(edge1.cross(v1))*sign >= 0
				&& normal_.dot(edge2.cross(v2))*sign >= 0
				&& normal_.dot(edge3.cross(v3))*sign >= 0) {
				return t;
				//return hitPoint;
			}
			if (bothSides) {
				sign = sign * -1;
				if (normal_.dot(edge1.cross(v1)) * sign >= 0
					&& normal_.dot(edge2.cross(v2)) * sign >= 0
					&& normal_.dot(edge3.cross(v3)) * sign >= 0) {

					return t;
				}

			}
		}
		return 0;

	}
	 Vec3f samplePointLight(float rand1, float rand2, float rand3) { return A + B*rand2 + C*rand2; }
	float computeArea() { return (B - A).cross(A - C).length()/2; }
};
#endif // !TRIANGLE_H

