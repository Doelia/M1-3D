#ifndef VECTOR_H
#define VECTOR_H

#include "Coord.h"

class Vector : public Coord {
private:

public:
	Vector(double x, double y, double z) : Coord(x, y, z) {};
	Vector() : Coord() {};
	Vector(Coord& v) : Coord(v) {};
	Vector(Coord* a, Coord* b);

	double getNorme();
	double getAngle(Vector*);
	void normalize();
	void diviseNorme(double diviser);
	double getScalar(Vector*);
	Vector* getProduitVectoriel(Vector*);
	void set(Coord&);

};

#endif

