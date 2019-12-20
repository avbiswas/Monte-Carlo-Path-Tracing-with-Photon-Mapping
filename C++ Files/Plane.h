#pragma once
#ifndef PLANE_H
#define PLANE_H
#include "ray_utils.h"

class Plane {
public:
	 Vec3f point;
	 Vec3f normal;
	Plane(){}
	Plane( Vec3f pt,  Vec3f N) : point(pt), normal(N) {}
	float hit(Ray& r, bool bothSides);
	friend std::ostream& operator << (std::ostream& os, const Plane& p) {
		return os << "Plane point : " << p.point << "-> Plane Normal : " << p.normal << endl;
		
	}
};
#endif 