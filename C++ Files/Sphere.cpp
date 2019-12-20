
#include "Sphere.h"
float Sphere::hit(Ray& r, bool bothSides) {
	float A, B, C, d;
	 Vec3f first_hit;
	float disc = discriminant(r.direction, r.point, &A, &B, &C);
	if ( disc >= 0) {
		disc = abs(sqrt(disc));
		
		d = (-B - disc) / (2 * A);
		if (d < 0) {
			d = (-B + disc) / (2 * A);
			if (d < 0) {
				return 0;
			}
		}
		// Vec3f hit_point = r.point + r.direction * d;
		return d;
	}
	return 0;
}

float Sphere::discriminant( Vec3f d,  Vec3f o, float* A, float* B, float* C) {
	*A = d.dot(d);
	*B = 2 * d.dot(o - this->center);
	*C = (o - this->center).dot(o - this->center) - radius * radius;
	float dis = pow(*B, 2) - 4 * (*A) * (*C);
	return dis;
}
Material Sphere::getMaterial() { return this->material; };
