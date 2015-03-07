#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include <functional>
#include "../lib/Include.h"



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
		delete(v);
		
		return p;
	}); };
	auto f = curbeB(a, b);
	return f;
}

Point*** getSurfaceCylindrique(std::function<Point*(double)> f, Vector* d, int nbU, int nbV) {
	Point*** pts = new Point**[nbU];
	for (int i = 0; i < nbU; ++i) {
		pts[i] = new Point*[nbV];
		double u = 1.0/(nbU-1) * (double) i;
		Point* p = f(u);
		for (int j = 0; j < nbV; j++) {
			double v = 1.0/(nbV-1) * (double) j;
			Vector* n = new Vector(*p);
			Vector* direct = new Vector(*d);
			direct->diviseNorme(v);
			n->add(direct);
			delete(n);
			delete(direct);
			pts[i][j] = new Point(*n);
		}
	}
	return pts;
}

Point** discretiser(std::function<Point*(double)> f, int nbU) {
	Point** pts = new Point*[nbU];
	for (int i = 0; i < nbU; ++i) {
		double u = 1.0/(nbU-1) * (double) i;
		pts[i] = f(u);
	}
	return pts;
}

Point** discretiserDouble(std::function<Point*(double, double)> f, int nbU, int nbV) {
	Point** pts = new Point*[nbU*nbV];
	int cpt = 0;
	for (int i = 0; i < nbU; ++i) {
		double u = 1.0/(nbU-1) * (double) i;
		for (int j = 0; j < nbV; ++j) {
			double v = 1.0/(nbV-1) * (double) j;
			pts[cpt++] = f(u, v);
		}
	}
	return pts;
}

Point* getPointOnCarreau(Point* ij, Point* i_j, Point* ij_, Point * i_j_, double u, double v) {
	Vector* A = new Vector(ij_, i_j_);
	A->diviseNorme(u);
	A->add(ij_);

	Vector* B = new Vector(ij, i_j);
	B->diviseNorme(u);
	B->add(ij);

	Vector* AB = new Vector(A, B);
	AB->diviseNorme(1-v);
	AB->add(A);

	Point* out = new Point(*AB);
	return out;
}

Point* calculPointFromTab(Point*** points, int nbU, int nbV, double u, double v) {
	if (nbU == 1 || nbV == 1) {
		return points[0][0]; // TODO
	} else {
		Point*** nouv = new Point**[nbU-1];
		for (int i = 0; i < nbU-1; ++i) {
			nouv[i] = new Point*[nbV-1];
			for (int j = 0; j < nbV-1; ++j) {
				nouv[i][j] = getPointOnCarreau(points[i][j], points[i+1][j], points[i][j+1], points[i+1][j+1], u , v);
			}
		}
		return calculPointFromTab(nouv, nbU-1, nbV-1, u, v);
	}
}
 
Point*** getMatriceFromBezier(Point** tabCtrlU, int nbU, Point** tabCtrlV, int nbV) {
	Point*** points = new Point**[nbU];
	for (int i = 0; i < nbU; ++i) {
		points[i] = new Point*[nbV];

		Point* Pu = tabCtrlU[i];
		Vector* ref = new Vector(tabCtrlV[0], Pu);

		for (int j = 0; j < nbV; ++j) {
			Vector* Pv = new Vector(*tabCtrlV[j]);
			Pv->add(ref);
			//PUV->add(Pu);
			points[i][j] = new Point(*Pv);
			//cout << "points[" << i << "][" << j << "] = " << *points[i][j] << endl;
		}

		delete(ref);
	}

	return points;

}


std::function<Point*(double, double)> surfaceByCasteljau(
	Point** tabCtrlU, int nbU,
	Point** tabCtrlV, int nbV
	) {

	auto curbeB = [] (Point** tabCtrlU, int nbU, Point** tabCtrlV, int nbV) -> std::function<Point*(double, double)>
	{ return ([=] (double u, double v) {

		Point*** points = getMatriceFromBezier(tabCtrlU, nbU, tabCtrlV, nbV);

		Point* p = calculPointFromTab(points, nbU, nbV, u, v);
		return p;
	}); };

	auto f = curbeB(tabCtrlU, nbU, tabCtrlV, nbV);
	return f;

}

Point** surface(
	std::function<Point*(double)> f1,
	std::function<Point*(double)> f2, int nbU, int nbV) {

	int cpt = 0;
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
