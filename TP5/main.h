#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"

class Sphere {
	Point center;
	int rayon;

	public:
	Sphere(const Point p, int rayon) {
		this->center = center;
		this->rayon = rayon;
	}

	bool appartient(const Point& p) {
		float sum = pow(p.getX() - center.getX(), 2) + pow(p.getY() - center.getY(), 2) + pow(p.getZ() - center.getZ(), 2);
		float rCarre = pow(rayon, 2);
		return (sum - rCarre) < 5.0f && (sum - rCarre) > -5.0f;
	}
};

class Voxel {
	Point* p;
	float size;

public:
	Voxel(Point* p, float size) {
		this->p = p;
		this->size = size;
	}

	Point*** getFaces() {

		int m = 4;
		float r = (float) size;
		float h = (float) size;

		int nbrFaces = 6;

		Point*** faces = generateCylindre(r, h, m);

		for (int i = 0; i < nbrFaces; ++i) {
			for (int j = 0; j < 4; ++j) {
				Vector v(*faces[i][j]);
				v.add(p);
				faces[i][j]->set(v);
			}
		}

		return faces;
	}

	void draw() {
		drawCube(getFaces());
	}
};

void drawShpere() {
	int sizeRepere = 10;
	int minX = -sizeRepere;
	int minY = -sizeRepere;
	int minZ = -sizeRepere;
	int maxX = sizeRepere;
	int maxY = sizeRepere;
	int maxZ = sizeRepere;

	Point center(0,0,0);
	Sphere s(center, 10);

	for (int i = minX; i < maxX; ++i) {
		for (int j = minY; j < maxY; ++j) {
			for (int k = minZ; k < maxZ; ++k) {
				Point p(i,j,k);
				if (s.appartient(p)) {
					Voxel v(&p,1);
					v.draw();
				}
			}
		}
	}
}



#endif
