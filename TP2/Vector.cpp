#include "Vector.h"
#include <math.h>
#include <iostream>

using namespace std;

Vector::Vector(Coord* a, Coord* b) {
	x = b->getX() - a->getX();
	y = b->getY() - a->getY();
	z = b->getZ() - a->getZ();
}

void Vector::normalize() {
	double norme = this->getNorme();
	x /= norme;
	y /= norme;
	z /= norme;
}

void Vector::diviseNorme(double diviser) {
	x /= diviser;
	y /= diviser;
	z /= diviser;
}

double Vector::getNorme() {
	return sqrt(x*x + y*y + z*z);
}

double Vector::getScalar(Vector* v) {
	return x*v->x + y*v->y + z*v->z;
}

Vector* Vector::getProduitVectoriel(Vector* v) {
	Vector* u = this;
	return new Vector(
		u->y*v->z - u->z*v->y,
		u->z*v->x - u->x*v->z,
		u->x*v->y - u->y*v->x
	);
}

double Vector::getAngle(Vector* v) {
	return acos(
		this->getScalar(v)/
			(this->getNorme() *v->getNorme())
			);
}
