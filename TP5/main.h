#ifndef TOOLGL_H
#define TOOLGL_H

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

	void draw() {
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


class Sphere {
	public:
	Point center;
	int rayon;

	Sphere(const Point p, int rayon) {
		this->center = center;
		this->rayon = rayon;
	}

	bool appartient(const Point& p) {
		float sum = pow(p.getX() - center.getX(), 2) + pow(p.getY() - center.getY(), 2) + pow(p.getZ() - center.getZ(), 2);
		float rCarre = pow(rayon, 2);
		return (sum - rCarre) < 0;
	}

	bool intersect(const Point&p, float tolerance) {
		float sum = pow(p.getX() - center.getX(), 2) + pow(p.getY() - center.getY(), 2) + pow(p.getZ() - center.getZ(), 2);
		float rCarre = pow(rayon, 2);
		return (sum - rCarre) < tolerance && (sum - rCarre) > -tolerance;
	}

	bool appartient(Voxel& p) {
		return this->intersect(*(p.p), p.size*5);
	}
};

void goAlgo(Sphere& s, Voxel v, float resolution) {
	if (s.appartient(v)) {
		if (resolution == 1) {
			v.draw();
		} else {
			Voxel* tab = v.decoupe();
			for (int i = 0; i < 8; ++i) {
				goAlgo(s, tab[i], resolution-1);
			}
		}
	} else {
		
	}
}

void drawShpereAdaptatif(Point center, float r, float resolution) {
	Voxel v(&center, r*2);
	Sphere s(center, r);
	goAlgo(s, v, resolution);

}

void drawShpere(Point center, float r, float resolution) {
	int sizeRepere = r;

	int minX = -sizeRepere - center.getX();
	int minY = -sizeRepere - center.getY();
	int minZ = -sizeRepere - center.getZ();
	int maxX = sizeRepere + center.getX();
	int maxY = sizeRepere + center.getY();
	int maxZ = sizeRepere + center.getZ();

	float sizeVoxel = r*2.0f / resolution;
	//cout << "sizeVoxel = " << sizeVoxel << endl;

	Sphere s(center, r);

	for (float i = minX; i <= maxX; i += sizeVoxel) {
		for (float j = minY; j <= maxY; j += sizeVoxel) {
			for (float k = minZ; k <= maxZ; k += sizeVoxel) {
				//cout << i << ", " << j << ", " << k << endl;
				Point p(i,j,k);
				if (s.appartient(p)) {
					Voxel v(&p,sizeVoxel);
					v.draw();
				}
			}
		}
	}
}




#endif
