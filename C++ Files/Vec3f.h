#pragma once

#ifndef  Vec3f_H
#define  Vec3f_H

/* Utility Vector class */

#include <iostream>
#include <math.h>
using namespace std;
class  Vec3f {
public:
	float x, y, z;
	inline Vec3f() : x(0), y(0), z(0) {}
	inline Vec3f(float px, float py, float pz) : x(px), y(py), z(pz) {}
	inline Vec3f(const  Vec3f& c) : x(c.x), y(c.y), z(c.z) {}
	inline Vec3f normalize() { float n = sqrt(x * x + y * y + z * z);  Vec3f v(x / n, y / n, z / n); return v; }
	inline float dot(const  Vec3f& c) { return x * c.x + y * c.y + z * c.z; }
	inline float dist2(const  Vec3f& p) { return pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z, 2); }
	inline float dist(const  Vec3f& p) { return sqrt(this->dist2(p)); }
	inline float length() { return sqrt(x * x + y * y + z * z); }
	inline  Vec3f cross(const  Vec3f& p,  Vec3f &n) {
		 Vec3f cross(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
		n = cross.normalize();
		return cross;
	}
	inline  Vec3f cross(const  Vec3f& p) {
		 Vec3f cross(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
		return cross;
	}
	inline  Vec3f operator * (const float f) { return  Vec3f(x * f, y * f, z * f); }
	inline  Vec3f operator / (const float f) { return  Vec3f(x / f, y / f, z / f); }
	inline  Vec3f operator * (const  Vec3f& c) { return  Vec3f(x * c.x, y * c.y, z * c.z); }
	inline  Vec3f operator + (const  Vec3f& c) { return  Vec3f(x + c.x, y + c.y, z + c.z); }
	inline  Vec3f operator - (  Vec3f& const c) { return  Vec3f(x - c.x, y - c.y, z - c.z);  }
	inline  Vec3f operator - () { return  Vec3f(-x, -y, -z); }
	inline bool operator == ( Vec3f& c) { return ((x == c.x) && (y == c.y) && (z == c.z)); }
	inline bool operator != ( Vec3f& c) { return !(*this == c); }
	inline bool exists() { if (x == NULL) return false; else return true; }

	friend std::ostream& operator << (std::ostream& os, const  Vec3f& v) {
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}
};

#endif