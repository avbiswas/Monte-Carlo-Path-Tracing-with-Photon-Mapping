#pragma once
#include <math.h>
#include <random> 
#include "Vec3f.h"
#include "Ray.h"
#include <random>
#include "MerlBRDF.h"

/*
* Utility functions
*/


struct Color {
	float r;
	float g;
	float b;

	Color operator+(Color other) {
		Color c = { r + other.r,g + other.g,b + other.b };
		return c;
	}
	
	float sq_error(Color& other) {
		
		return ((r - other.r)* (r - other.r)) + ((g - other.g)* (g - other.g)) + ((b - other.b)* (b - other.b));
	}
	float intensity() {
		return (r * r + g * g + b * b);
	}
	Color operator*(Color other) {
		Color newColor = { r * other.r , g * other.g, b * other.b };
		
		
		return newColor;
	}

	Color operator*(float other) {
		Color newColor = { r * other , g * other, b * other };
		
		return newColor;
	}

	Color operator/(float other) {
		Color c = { r / other,g / other,b / other };
		return c;
	}

	
	Color power(int other) {
		Color c = { pow(r,other), pow(g,other), pow(b,other) };
	}
};

enum material_type {
	GLOSSY,
	DIFFUSE,
	SPECULAR,
	MIRROR,
	EMISSIVE,
	REFRACTIVE
};
class Material {
public:
	Material() {}
	Material(Color diffuse_albedo, material_type type, float K_diffuse, Color emission, float specular_exp,
		float K_specular, float K_absorb, float K_emission_intensity, float K_mirror_brightness, float K_refract, float ior) {
		this->diffuse_albedo = diffuse_albedo;
		this->type = type;
		this->K_diffuse = K_diffuse;
		this->emission = emission;
		this->specular_exp = specular_exp;
		this->K_specular = K_specular;
		this->K_absorb = K_absorb;
		this->K_emission_intensity = K_emission_intensity;
		this->K_mirror_brightness = K_mirror_brightness;
		this->K_refract = K_refract;
		this->ior = ior;
	}
	Color diffuse_albedo;
	material_type type;
	float K_diffuse = 1;
	Color emission = {0,0,0};
	float specular_exp;
	float K_specular = 0;
	float K_absorb = 0.5;
	float K_emission_intensity = 0;
	float K_mirror_brightness;
	float K_refract;
	float ior;


};

#pragma once
#ifndef OBJECT_H
#define OBJECT_H
class Objects
{
public:
	virtual Material getMaterial() = 0;
	virtual float hit(Ray &r, bool bothSides) = 0;
	virtual Vec3f normal(Vec3f& point) = 0;
	virtual Vec3f samplePointLight(float rand1, float rand2, float rand3) = 0;
	virtual string getType() = 0;
	virtual string getID() = 0;
	virtual float computeArea() = 0;
};

#endif

