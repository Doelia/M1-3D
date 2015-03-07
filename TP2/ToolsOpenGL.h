#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include "Vector.h"
#include "Point.h"
#include "GlutIncluder.h"

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

Point** hermiteCurve(Point* p0, Point* p1, Vector* v0, Vector* v1, long nbU) {
	Point** pts = new Point*[nbU];
	for (int i = 0; i < nbU; ++i) {
		double u = 1.0/(nbU-1) * (double) i;

		double f1 = 2*pow(u,3) - 3*pow(u,2) + 1;
		double f2 = -2*pow(u,3) + 3*pow(u,2);
		double f3 = pow(u,3) - 2*pow(u,2) + u;
		double f4 = pow(u,3) - pow(u,2);

		pts[i] = new Point(
			f1*p0->getX() + f2*p1->getX() + f3*v0->getX() + f4*v1->getX(),
			f1*p0->getY() + f2*p1->getY() + f3*v0->getY() + f4*v1->getY(),
			f1*p0->getZ() + f2*p1->getZ() + f3*v0->getZ() + f4*v1->getZ()
		);
	}
	return pts;
}

double fact(double n) {
	if (n == 0) {
		return 1.0;
	}
	return n*fact(n-1);
}

Point** bezierCurveByBernstein(Point** tab, long nControl, long nbU) {

	double n = nControl-1;

	Point** pts = new Point*[nbU];

	for (int j = 0; j < nbU; ++j) {
		double u = 1.0/(nbU-1) * (double) j;
		Point* p = new Point();
		for (int i = 0; i <= n; ++i) {
			double Bni = (fact(n) / (fact(i) * fact(n-i))) * pow(u, i) * pow(1-u, n-i);
			p->setX(p->getX() + Bni*tab[i]->getX());
			p->setY(p->getY() + Bni*tab[i]->getY());
			p->setZ(p->getZ() + Bni*tab[i]->getZ());
		}
		pts[j] = p;

	}
	return pts;
}

Point* getPt(Point* a, Point* b, double u) {
	Vector* v = new Vector(a, b);
	v->diviseNorme(u);
	Point* out = new Point(	v->getX() + a->getX(),
							v->getY() + a->getY(),
							v->getZ() + a->getZ());

	cout << u << endl;
	if (u == 0.3) {
		cout << "hey" << endl;
		glBegin(GL_LINE_STRIP);
			drawPoint(a);
			drawPoint(b);
		glEnd();
	}
	return out;
}

Point** getPos(Point** tab, int nbrPoints, double u) {
	if (nbrPoints == 1) {
		return tab;
	} else {
		Point** pts = new Point*[nbrPoints-1];
		for (int i = 0; i < nbrPoints-1; ++i) {
			pts[i] = getPt(tab[i], tab[i+1], u);
		}
		return getPos(pts, nbrPoints-1, u);
	}
}


Point** bezierCurveByCasteljau(Point** tab, long nControl, long nbU) {
	Point** pts = new Point*[nbU];

	for (int j = 0; j < nbU; ++j) {
		double u = 1.0/(nbU-1) * (double) j;
		pts[j] = *getPos(tab, nControl, u);
	}

	return pts;
}
 

#endif
