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
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_QUADS);
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


void drawCylindre(Point*** tab, int nbrMeridiens) {
	for (int i = 2; i < 2+nbrMeridiens; i++) {
		Point** face = tab[i];
		glColor4f(1, 0, (double) i / (double) nbrMeridiens, .5f);
		drawFace(face, 4);
	}
	for (int i = 0; i < 2; i++) {
		Point** face = tab[i];
		glColor4f(.5f, 1, 1, 0.5f);
		drawFace(face, nbrMeridiens);
	}
}

void drawCube(Point*** tab) {
	for (int i = 0; i < 6; i++) {
		Point** face = tab[i];
		drawFace(face, 4);
	}
}

void drawCone(Point*** tab, int nbrMeridiens) {
	for (int i = 1; i < 1+nbrMeridiens; i++) {
		Point** face = tab[i];
		glColor4f(1, 0, (double) i / (double) nbrMeridiens, .5f);
		drawFace(face, 4);
	}
	glColor4f(.5f, 1, 1, 0.5f);
	drawFace(tab[0], nbrMeridiens);
}

void drawSphere(Point*** tab, int m, int l) {
	for (int i = 0; i < l-1; ++i) {
		for (int j = 0; j < m; ++j) {
			int cpt = i*l + j;
			Point** face = tab[cpt];
			glColor4f(1, 0, (double) i / (double) m, .5f);
			drawFace(face, 4);
		}
	}
}




#endif
