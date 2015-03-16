#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"
#include <Vector>

using namespace std;

class Face {
public:

	vector<Point> points;

	Face() {

	}

	void addPoint(Point p) {
		points.push_back(p);
	}

	void draw() {
		for (auto p : points) {
			// TODO
		}
	}

	static void drawSet(vector<Face> faces) {
		for (auto p : faces) {
			p.draw();
		}
	}

};

vector<Face> parseFile(const char* path) {

	FILE *f_maillage;
	vector<Face> faces;

	if ((f_maillage = fopen(path, "rb")) == NULL) {
		printf("\nPas d'acces en lecture sur l'image %s \n", path);
		exit(EXIT_FAILURE);
	}
	else {
		int nbrSommets, nbrFaces;
		fscanf(f_maillage, "%d %d", &nbrSommets, &nbrFaces);

		// Chargement des sommets
		Point* points = new Point[nbrSommets];
		for (int i = 0; i < nbrSommets; ++i) {
			float x,y,z;
			fscanf(f_maillage, "%f %f %f", &x, &y, &z);
			Point p(x,y,z);
			points[i].set(p);
		}

		// Chargement des sommets
		for (int i = 0; i < nbrFaces; ++i) {
			char buffer[250];
			int nbrPoints;
			fscanf(f_maillage, "%d %s", &nbrPoints, buffer);
			Face f;
			for (int j = 0; j < nbrPoints; ++j) {
				int pt = 0; // TODO
				f.addPoint(points[pt]);
			}
			faces.push_back(f);
		}

		delete(points);

	}

	return faces;

}

#endif
