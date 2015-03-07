#include "Vector.h"
#include "Point.h"
#include <iostream>

using namespace std;

int main() {
	
	/*
	Point *a = new Point(2, 1, 0);
	Point *b = new Point(4, 1, 0);
	Point *c = new Point(3.5, 3.5, 0);
	Point* cPrim = c->projectOnLine(a, b);
	cout << "c' = " << *cPrim << endl; // 3.5, 1, 0
	//*/
		
	//*
	Point *a = new Point(0, 0, 0);
	Vector* n = new Vector(0, 0, 1);
	Point* m = new Point(1, 1, 1);
	Point* mPrim = m->projectOnPlan(a, n);
	cout << "m' = " << *mPrim << endl; // 1 1 0
	//*/

	
	/*
	Vector* AB = new Vector(4, 4, 0);
	Vector* AC = new Vector(4, 8, 0);
	cout << "produit' = " << AB->getScalar(AC) << endl;
	cout << "produit' = " << AC->getScalar(AB) << endl;
	//*/



}

