#include "Point.h"

Point* Point::projectOnLine(Point* b, Point *c) {
	Vector* u = new Vector(b,c);
	u->normalize();
	cout << "u = " << *u << endl;
	return this->projectOnLine(u, b);
}

Point* Point::projectOnLine(Vector* u, Point* b) {
	Point* a = this;
	Vector* ba = new Vector(b,a);
	cout << "ba = " << *ba << endl;

	double normeBAPrim = ba->getScalar(u) / u->getNorme();
	cout << "norme = " << normeBAPrim << endl;
	return new Point(
		b->getX() + u->getX()*normeBAPrim,
		b->getY() + u->getY()*normeBAPrim,
		b->getZ() + u->getZ()*normeBAPrim
	);
}

Point* Point::projectOnPlan(Point* a, Vector* n) {
	Point* m = this;
	Vector* ma = new Vector(m, a);
	double normeMMPrim = ma->getScalar(n) / n->getNorme();
	return new Point(
		m->getX() - n->getX() * normeMMPrim,
		m->getY() - n->getY() * normeMMPrim,
		m->getZ() - n->getZ() * normeMMPrim
	);
}

