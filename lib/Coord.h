#ifndef COORD_H
#define COORD_H

#include <iostream>

using namespace std;

class Coord {
protected:
	double x;
	double y;
	double z;
public:
	Coord(double x, double y, double z);
	Coord();
	Coord(const Coord&);
	~Coord();

	virtual double getX() const;
	virtual double getY() const;
	virtual double getZ() const;
	virtual void setX(double x);
	virtual void setY(double y);
	virtual void setZ(double z);
	virtual void add(Coord*);
	virtual void multiply(const Coord*);
	virtual float sum();
	void set(Coord&);
};

ostream& operator<<(ostream &flux, Coord& v);

#endif
