#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
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


/*Point* getPointOnCarreau(Point* a, Point* b, Point *c, double u) {
	Vector* ab = new Vector(a, b);
	Vector* ac = new Vector(a, c);
	ab->diviseNorme(u);
	ac->diviseNorme(u);

	ab->add(ac);

	return new Point(
		a->getX() + ab->getX(),
		a->getY() + ab->getY(),
		a->getZ() + ab->getZ()
	);
}
*/

/*
Point* getPointOnCarreau(Point* ij, Point* i_j, Point* ij_, Point * i_j_, double u, double v) {

	cout << *ij << ", " << *i_j << ", " << *ij_ << ", " << *i_j_ << endl;
	
	Vector* p = new Vector(*ij_);
	p->diviseNorme(1-u);
	Vector* q = new Vector(*i_j_);
	p->diviseNorme(u);
	p->add(q);
	Vector* A = new Vector(*p);

	cout << "A (  " << u << ", " << v << ") = " << *A << endl;

	p = new Vector(*ij);
	p->diviseNorme(1-u);
	q = new Vector(*i_j);
	q->diviseNorme(u);
	p->add(q);
	Vector* B = new Vector(*p);

	B->diviseNorme(1-v);
	A->diviseNorme(u);
	B->add(A);

	return new Point(*B);
}
*/


Point* getPointOnCarreau(Point* ij, Point* i_j, Point* ij_, Point * i_j_, double u, double v) {

	cout << *ij << ", " << *i_j << ", " << *ij_ << ", " << *i_j_ << endl;
	//cout << "u=" << u << ", v=" << v << endl;
	
	Vector* A = new Vector(ij_, i_j_);
	A->diviseNorme(u);
	A->add(ij_);
	//cout << "A=" << *A << endl;

	Vector* B = new Vector(ij, i_j);
	B->diviseNorme(u);
	B->add(ij);
	//cout << "B=" << *B << endl;


	Vector* AB = new Vector(A, B);
	AB->diviseNorme(1-v);
	AB->add(A);
	//cout << "P = " << *AB << endl;

	return new Point(*AB);
}

Point* getPointOnCoord(int x, int y, Point** tabCtrlU, Point** tabCtrlV) {
	int xMin = (x == 0)?0:x-1;
	int yMin = (y == 0)?0:y-1;
	Vector* X = new Vector(tabCtrlU[xMin], tabCtrlU[x]);
	cout << "X = " << *X << endl;
	Vector* Y = new Vector(tabCtrlV[yMin], tabCtrlV[y]);
	cout << "Y = " << *Y << endl;
	X->add(Y);
	cout << "tabCtrlU[xMin] = " << *tabCtrlU[xMin] << endl;
	X->add(tabCtrlV[yMin]);
	return new Point(*X);
}

Point* calculPointFromTab(Point*** points, int nbU, int nbV, double u, double v) {
	cout << "calculPointFromTab(" << nbU << ", " << nbV << "), u=" << u << ", v=" << v << endl;
	if (nbU == 1 || nbV == 1) {
		return points[0][0]; // TODO
	} else {
		Point*** nouv = new Point**[nbU-1];
		for (int i = 0; i < nbU-1; ++i) {
			nouv[i] = new Point*[nbV-1];
			for (int j = 0; j < nbV-1; ++j) {
				nouv[i][j] = new Point();
				nouv[i][j] = getPointOnCarreau(points[i][j], points[i+1][j], points[i][j+1], points[j+1][i+1], u , v);
				cout << "nouv[" << i << "," << j << "] = " << *nouv[i][j] << endl;
			}
		}
		return calculPointFromTab(nouv, nbU-1, nbV-1, u, v);
	}
}


std::function<Point*(double, double)>  surfaceByCasteljau(
	Point** tabCtrlU, int nbU,
	Point** tabCtrlV, int nbV
	) {

	auto curbeB = [] (Point** tabCtrlU, int nbU, Point** tabCtrlV, int nbV) -> std::function<Point*(double, double)>
	{ return ([=] (double u, double v) {

		Point*** points = new Point**[nbU];

		for (int i = 0; i < nbU; ++i) {
			points[i] = new Point*[nbV];
			for (int j = 0; j < nbV; ++j) {
				points[i][j] = getPointOnCoord(i, j, tabCtrlU, tabCtrlV);
				cout << "points[" << i << "][" << j << "] = " << *points[i][j] << endl;
			}
		}

		Point* p = calculPointFromTab(points, nbU, nbV, u, v);
		return p;
	}); };

	auto f = curbeB(tabCtrlU, nbU, tabCtrlV, nbV);
	return f;

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
