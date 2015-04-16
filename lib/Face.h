#ifndef FACE_H
#define FACE_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "GlutIncluder.h"
#include "Geometry.h"
#include "Include.h"
#include "Drawer.h"
#include <Vector>

using namespace std;

class Face {
public:

	vector<Point> points; // Coordonées des points composants la face
	vector<int> indices; // Indexe des numéro des points

	Face() {
	}

	Point barycenter() {
		Point enFace = points[0];
		Vector base(&points[1], &points[2]);
		base.diviseNorme(.5);
		base.add(&points[1]);
		Vector v(&base, &enFace);
		v.diviseNorme(.3);
		v.add(&base);
		return v;
	}

	void addPoint(Point p) {
		points.push_back(p);
	}

	void draw() {

		//glBegin(GL_POLYGON);
		glBegin(GL_LINE_STRIP);
		for (auto p : points) {
			drawPoint(&p);
		}
		if (points.size() > 0) {
			drawPoint(&points[0]);
		}
		glEnd();
	}

	Vector getNormale() {
		Vector v1(&(points[0]), &(points[1]));
		Vector v2(&(points[0]), &(points[2]));
		Vector* produit = v2.getProduitVectoriel(&v1);
		produit->normalize();
		return *produit;
	}

};

#endif
