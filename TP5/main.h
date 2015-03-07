#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"


class Voxel {
	Point* p;
	int size;

public:
	Voxel(Point* p, int size) {
		this->p = p;
		this->size = size;
	}

	Point*** getFaces() {

		int m = 4;
		int r = size;
		int h = size;

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




#endif
