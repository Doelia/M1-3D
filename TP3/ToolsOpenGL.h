#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include "Vector.h"
#include "Point.h"
#include "GlutIncluder.h"
#include <functional>

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


double fact(double n) {
	if (n == 0) {
		return 1.0;
	}
	return n*fact(n-1);
}

std::function<Point*(double)> bezierCurveByBernstein(Point** tab, long nControl) {

	auto curbeB = [] (Point** tab, long nControl) -> std::function<Point*(double)>
	{ return ([=] (double u) {

		double n = nControl-1;

		Point* p = new Point();
		for (int i = 0; i <= n; ++i) {
			double Bni = (fact(n) / (fact(i) * fact(n-i))) * pow(u, i) * pow(1-u, n-i);
			p->setX(p->getX() + Bni*tab[i]->getX());
			p->setY(p->getY() + Bni*tab[i]->getY());
			p->setZ(p->getZ() + Bni*tab[i]->getZ());
		}
		return p;


	}); };
	auto f = curbeB(tab, nControl);
	return f;
}

std::function<Point*(double)> getDroite(Point* a, Point *b) {

	auto curbeB = [] (Point* a, Point* b) -> std::function<Point*(double)>
	{ return ([=] (double u) {

		Vector* v = new Vector(a, b);
		v->diviseNorme(u);
		Point* p = new Point(
			a->getX() + v->getX(),
			a->getY() + v->getY(),
			a->getZ() + v->getZ()
		);
		
		return p;
	}); };
	auto f = curbeB(a, b);
	return f;
}

Point** discretiser(std::function<Point*(double)> f, int nbU) {
	Point** pts = new Point*[nbU];
	for (int i = 0; i < nbU; ++i) {
		double u = 1.0/(nbU-1) * (double) i;
		pts[i] = f(u);
	}
	return pts;
}


Point** surface(
	std::function<Point*(double)> f1,
	std::function<Point*(double)> f2, int nbU, int nbV) {

	int cpt =0;
	Point** pts = new Point*[nbU*nbV];

	for (int i = 0; i < nbU; ++i) {
		double u = 1.0/(nbU-1) * (double) i;
		Point* Pu = f1(u);
		Point* Qu = f2(u);
		for (int j = 0; j < nbV; ++j) {
			double v = 1.0/(nbV-1) * (double) j;
			pts[cpt] = new Point(
				(1-v) * Pu->getX() + v * Qu->getX(),
				(1-v) * Pu->getY() + v * Qu->getY(),
				(1-v) * Pu->getZ() + v * Qu->getZ()
			);
			cpt++;
		}
	}
	return pts;
}

#endif
