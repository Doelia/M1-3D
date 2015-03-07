#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Point.h"
#include "math.h"
#include "Include.h"

// Généré à partir de O en (0,0,0)
Point*** generateCylindre(int r, int h, int m) {

	Point*** faces = new Point**[2+m];
	
	faces[0] = new Point*[m];
	faces[1] = new Point*[m];
	for (int i =2; i < 2+m; i++) {
		faces[i] = new Point*[4];
	}
							
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

Point*** generateCone(int r, Point* sommet, int h, int m) {

	Point*** faces = new Point**[1+m];
	
	faces[0] = new Point*[m];
	for (int i =1; i < 1+m; i++) {
		faces[i] = new Point*[4];
	}
							
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


float getRayon(float r, float u) {
	if (u < .5) {
		u *= 2;
		u = 1-u;
	}
	else {
		u *= 2;
		u -= 1;
		u *= -1;
	}

	float angle = acos(u);
	return sin(angle) * r;
}

Point*** generateSphere(int r, Point* center, int meridiens, int paralleles) {
	Point*** faces = new Point**[paralleles*meridiens];
	for (int i = 0; i < meridiens*paralleles; i++) {
		faces[i] = new Point*[4];
	}

	Point poleNord(*center);
	poleNord.setY(poleNord.getY()-r/2);

	for (int i = 0; i < paralleles; ++i) {
		double u = 1.0/(paralleles-1) * (double) i;
		float rayon  = getRayon(r, u);
		Vector d(0, poleNord.getY() + r*u*2, 0);

		for (int j = 0; j < meridiens; ++j) {
			double angle = 2.0f * M_PI * (double) j / (double) meridiens;
			Point* p = new Point (rayon * cos(angle), 0, rayon * sin(angle));
			p->add(&d);

			int iPrec = i - 1;
			int jPrec = (j == 0) ? meridiens-1 : j-1;

			int faceCourante = i*paralleles + j;
			int facePrecedente = i*paralleles  + jPrec;
			int faceDessus = iPrec*paralleles + j;
			int faceDessusPrecedente = iPrec*paralleles + jPrec;

			faces[faceCourante][0] = p;
			faces[facePrecedente][1] = p;

			if (i > 0) {
				faces[faceDessus][3] = p;
				faces[faceDessusPrecedente][2] = p;
			}
		}
	}

	return faces;
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
	drawCylindre(tab, 4);
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

