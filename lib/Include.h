#ifndef INC_H
#define INC_H

#include "Vector.h"
#include "Point.h"
#include "GlutIncluder.h"
#include "math.h"

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
		cout << "point " << *tab[i] << endl;
	}
	glEnd();
}

void drawCylindre(Point*** tab, int nbrMeridiens) {
	for (int i = 0; i < nbrMeridiens+2; i++) {
		cout << "Face " << i << endl;
		Point** face = tab[i];
		drawFace(face, (i <= 1) ? nbrMeridiens : 4);
	}
}

Point** copyPoints(Point** tab, int n) {
	Point** out = new Point*[n];
	for (int i = 0; i < n; ++i) {
		out[i] = new Point(*tab[i]);
	}
	return out;
}



#endif
