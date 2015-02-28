#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>


// Généré à partir de O en (0,0,0)
Point** generateCylindre(int r, int h, int m) {
	double angle = 0;

	//Point*** pts = new Point**[2];
	Point** pts = new Point*[m*2];
	//pts[0] = new Point*[m];
	//pts[1] = new Point*[m];

	for (int i = 0; i < m; ++i) {
		double angle = 2.0f * M_PI * (double) i / (double) m;
		pts[i] = new Point(
			r * cos(angle),
			r * sin(angle),
			(double) h / 2.0f
		);
		pts[i+m] = new Point(
			r * cos(angle),
			r * sin(angle),
			- (double) h / 2.0f
		);
	}

	return pts;
}

#endif
