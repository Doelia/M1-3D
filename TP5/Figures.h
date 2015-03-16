#ifndef FIGURE
#define FIGURE

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"
#include "Voxel.h"

class Figure {
public:

	virtual float equation(const Point& p) = 0;

	virtual bool intersect(const Point&p, float tolerance) {
		float r = equation(p);
		return r < tolerance && r > -tolerance;
	}

	virtual bool isOn(const Voxel& p) {
		return this->intersect(*(p.p), p.size*5);
	}

	virtual bool isOut(const Voxel& p) {
		return equation(*p.p) > 0;
	}

	virtual bool isIn(const Voxel& p) {
		return equation(*p.p) < 0;
	}
};

class Sphere : public Figure {
	public:
	Point center;
	int rayon;

	Sphere(const Point p, int rayon) {
		this->center = center;
		this->rayon = rayon;
	}

	virtual float equation(const Point& p) {
		float sum = pow(p.getX() - center.getX(), 2) + pow(p.getY() - center.getY(), 2) + pow(p.getZ() - center.getZ(), 2);
		float rCarre = pow(rayon, 2);
		return sum - rCarre;
	}
};

class Cilynder : public Figure {
	public:
	Point center;
	float rayon;
	Vector axe;

	Cilynder(const Point center, Vector axe, float rayon) {
		this->center = center;
		this->rayon = rayon;
		this->axe = axe;
	}

	virtual float equation(const Point& p) {
		float carreCenter = pow(p.getX(), 2) + pow(p.getY(), 2) + pow(p.getZ(), 2);
		float carreAxe = pow(axe.getX(), 2) + pow(axe.getY(), 2) + pow(axe.getZ(), 2);

		Vector v(axe);
		v.multiply(&p);
		float multilyCenterAxe = pow(v.sum(), 2);

		float division = multilyCenterAxe / carreAxe;
		float minus = carreCenter - division;
		float rCarre = pow(rayon, 2);

		return minus - rCarre;
	}
};



#endif
