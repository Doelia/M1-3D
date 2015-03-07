#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"


// Généré à partir de O en (0,0,0)
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

// Généré à partir de O en (0,0,0)
Point*** generateCone(int r, Point* sommet, int h, int m) {

	Point*** faces = new Point**[1+m];
	
	faces[0] = new Point*[m];
	for (int i =1; i < 1+m; i++) {
		faces[i] = new Point*[4];
	}
							
	//Point** pts = new Point*[m*2];
	
	for (int i = 0; i < m; ++i) {
		double angle = 2.0f * M_PI * (double) i / (double) m;

		Vector *v = new Vector(
			r * cos(angle),
			(double) h,
			r * sin(angle)
		);
		v->add(sommet);

		faces[0][i] = new Point(*v);
		faces[1][i] = new Point(*sommet);
		
		faces[i+1][0] = faces[0][i];
		faces[i+1][1] = faces[1][i];
		
		int precedent = (i == 0) ? m-1 : i-1;
		faces[precedent+1][3] = faces[1][i];
		faces[precedent+1][2] = faces[0][i];
	}

	return faces;
}



#endif
