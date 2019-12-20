#pragma once
#ifndef RAY_H
#define RAY_H
#include "Vec3f.h"
#include <math.h>
#include <iostream>
using namespace std;

class Ray {
public:
	Vec3f point;
	Vec3f direction;
	Ray() {}
	inline Ray( Ray& const r) :  point(r.point), direction(r.direction) {}
	inline Ray(Vec3f& arg1, Vec3f& arg2, bool make) :point(arg1) { if (!make) direction = arg2.normalize(); else direction = (arg2 - arg1).normalize(); }
	friend ostream& operator<<(ostream& os, const Ray& r) {
		return os << "Point" << r.point << ", Direction:"<< r.direction << endl;
	}
	
	
};

#endif
