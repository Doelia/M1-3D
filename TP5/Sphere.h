#ifndef SPHERE
#define SPHERE

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"
#include "Voxel.h"


class Sphere {
	public:
	Point center;
	int rayon;

	Sphere(const Point p, int rayon) {
		this->center = center;
		this->rayon = rayon;
	}

	float equation(const Point& p) {
		float sum = pow(p.getX() - center.getX(), 2) + pow(p.getY() - center.getY(), 2) + pow(p.getZ() - center.getZ(), 2);
		float rCarre = pow(rayon, 2);
		return sum - rCarre;
	}

	bool intersect(const Point&p, float tolerance) {
		float r = equation(p);
		return r < tolerance && r > -tolerance;
	}

	bool isOn(const Voxel& p) {
		return this->intersect(*(p.p), p.size*5);
	}

	bool isOut(const Voxel& p) {
		return equation(*p.p) > 0;
	}

	bool isIn(const Voxel& p) {
		return equation(*p.p) > 0;
	}
};




#endif
