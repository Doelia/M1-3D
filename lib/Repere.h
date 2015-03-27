#ifndef REPERE_H
#define REPERE_H

#include <stdio.h>      
#include <stdlib.h>     
#include "Maillage.h"
#include "Include.h"

using namespace std;

class Repere {

	Maillage m;

public:

	Point center;
	float size;

	Repere(Maillage m) {
		this->m = m;
		this->center = getCenter();
		Vector v(center);
		Vector minus(-1, -1, -1);
		v.multiply(&minus);
		this->size = getBestSizeRepere(v);
		this->size = this->size * 2.0f;
	}

private:

	Point getCenter() {
		Point pMin = getMoreNegative();
		Point pMax = getMorePositive();
		Point p(
			(pMin.getX() + pMax.getX()) / 2,
			(pMin.getY() + pMax.getY()) / 2,
			(pMin.getZ() + pMax.getZ()) / 2);
		return p;
	}

	Point getMoreNegative() {
		Point* out = NULL;
		for (Face f : m.faces) {
			for (Point p : f.points) {
				if (out == NULL) {
					out = new Point(p);
				}
				if (p.getX() < out->getX())
					out->setX(p.getX());
				if (p.getY() < out->getY())
					out->setY(p.getY());
				if (p.getZ() < out->getZ())
					out->setZ(p.getZ());
			}
		}
		return *out;
	}

	Point getMorePositive() {
		Point* out = NULL;
		for (Face f : m.faces) {
			for (Point p : f.points) {
				if (out == NULL) {
					out = new Point(p);
				}
				if (p.getX() > out->getX())
					out->setX(p.getX());
				if (p.getY() > out->getY())
					out->setY(p.getY());
				if (p.getZ() > out->getZ())
					out->setZ(p.getZ());
			}
		}
		return *out;
	}

	float getBestSizeRepere(Vector center) {
		float size = 0;
		for (Face f : m.faces) {
			for (Point p : f.points) {
				Point x = (p);
				x.add(&center);

				if (fabs(x.getX()) > size)
					size = fabs(x.getX());
				if (fabs(x.getY()) > size)
					size = fabs(x.getY());
				if (fabs(x.getZ()) > size)
					size = fabs(x.getZ());
			
			}
		}
		return size;
	}

};


#endif
