#include "Vector.h"
#include "Point.h"
#include "GlutIncluder.h"
#include "math.h"
#include "Geometry.h"
#include <unistd.h>
#include <pthread.h>

#ifndef INC_H
#define INC_H

void drawPoint(Coord* c) {
	glVertex3f(c->getX(), c->getY(), c->getZ());
}

void drawCurve(Point** tab, long nbPoints) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < nbPoints; ++i) {
		drawPoint(tab[i]);
	}
	glEnd();
}

void drawPoints(Point** tab, long nbPoints) {
	glBegin(GL_POINTS);
	for (int i = 0; i < nbPoints; ++i) {
		drawPoint(tab[i]);
	}
	glEnd();
}

void drawMatrice(Point*** tab, int u, int v) {
	glBegin(GL_POINTS);
	for (int i = 0; i < u; ++i) {
		for (int j = 0; j < v; ++j) {
			drawPoint(tab[i][j]);
		}
	}
	glEnd();
}

void drawFace(Point** tab, int x) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < x; ++i) {
		drawPoint(tab[i]);
	}
	glEnd();
}


Point** copyPoints(Point** tab, int n) {
	Point** out = new Point*[n];
	for (int i = 0; i < n; ++i) {
		out[i] = new Point(*tab[i]);
	}
	return out;
}



#endif
