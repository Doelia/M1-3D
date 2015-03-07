#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"

Point*** generateCylindre(int r, int h, int m) {

	Point*** faces = new Point**[2+m];
	
	faces[0] = new Point*[m];
	faces[1] = new Point*[m];
	for (int i =2; i < 2+m; i++) {
		faces[i] = new Point*[4];
	}
	
	//Point** pts = new Point*[m*2];
	
	for (int i = 0; i < m; ++i) {
		double angle = 2.0f * M_PI * (double) i / (double) m;
		
		faces[0][i] = new Point(
			r * cos(angle),
			(double) h / 2.0f,
			r * sin(angle)
		);
		
		faces[1][i] = new Point(
			r * cos(angle),
			- (double) h / 2.0f,
			r * sin(angle)
		);
		
		faces[i+2][0] = faces[0][i];
		faces[i+2][1] = faces[1][i];
		
		int precedent = (i == 0) ? m-1 : i-1;
		faces[precedent+2][3] = faces[0][i];
		faces[precedent+2][2] = faces[1][i];
	}

	return faces;
}


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
