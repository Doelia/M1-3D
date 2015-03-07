#include "Coord.h"
#include <iostream>

using namespace std;

Coord::Coord(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Coord::Coord() {
	this->x = 0;
	this->y = 0;
	this->z = 0;
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

double Coord::getX() {
	return x;
}

double Coord::getY() {
	return y;
}

double Coord::getZ() {
	return z;
}


void Coord::add(Coord* v) {
	this->x += v->getX();
	this->y += v->getY();
	this->z += v->getZ();
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

