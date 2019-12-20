#pragma once
#ifndef TRIANGLE_H
#include "Ray.h"
#include "Plane.h"
class Triangle
{
public:
	Vec3f A, B, C;
	Triangle(Vec3f a, Vec3f b, Vec3f c):A(a), B(b), C(c){}
	bool hit() {
		cout << getPlane() << endl;
		return true;
	}
private:
	Plane getPlane() {
		Vec3f normal;
		return Plane(A, (A - B).cross(B - C, &normal));
		
	}
};
#endif // !TRIANGLE_H

