#ifndef VOXE_H
#define VOXE_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"

class Voxel {
public:
	Point* p;
	float size;

	int flag;

	Voxel(Point* p, float size) {
		this->p = p;
		this->size = size;
	}

	Voxel() {

	}

	void set(const Voxel& v) {
		this->p = v.p;
		this->size = v.size;
	}

	Point*** getFaces() {

		float r = (float) size;

		Vector v(*p);
		Point*** faces = generateCube(r, v);

		return faces;
	}

	void draw(int idColor) {
		switch (idColor) {
			case 1:
				glColor4f(.5f, 1, 1, 0.2f);
				break;
			case 2:
				glColor4f(1, 0.5f, 1, 0.2f);
				break;
			case 3:
				glColor4f(1, 1, .5f, 0.2f);
				break;
			default:
				return;
		}
		drawCube(getFaces());
	}

	Voxel* decoupe() {
		Voxel* vx = new Voxel[8];
		int cpt = 0;
		for (int i = -1; i <= 1; i += 2)
		for (int j = -1; j <= 1; j += 2)
		for (int k = -1; k <= 1; k += 2) {
			float coef = size/4.0;
			Vector v(i*coef,j*coef,k*coef);
			Point* newP = new Point(*this->p);
			newP->add(&v);
			Voxel newV(newP, size/2.0f);
			vx[cpt++].set(newV);
		}
		return vx;
	}
};



#endif
