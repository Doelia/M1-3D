#include "Coord.h"
#include <iostream>

using namespace std;

Coord::Coord(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Coord::Coord() {
	
}

void Coord::setX(double x) {
	this->x = x;
}

void Coord::setY(double y) {
	this->y = y;
}

void Coord::setZ(double z) {
	this->z = z;
}

const double Coord::getX() {
	return x;
}

const double Coord::getY() {
	return y;
}

const double Coord::getZ() {
	return z;
}

Coord::Coord(Coord& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

ostream& operator<<(ostream &flux, Coord& v) {
    flux << "[" << v.getX() << "," << v.getY() << "," << v.getZ() << "]";
    return flux;
}

