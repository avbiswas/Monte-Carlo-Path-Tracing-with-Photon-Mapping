#include "Plane.h"
float Plane:: hit(Ray& r, bool bothSides) {
	if (r.direction.dot(normal) == 0) {
		return 0;
	}
	float t = (point.dot(normal) - r.point.dot(normal)) / (r.direction.dot(normal));
	if (t < 0) {
		return 0;
	}
	else {
		return t;
	}
}
