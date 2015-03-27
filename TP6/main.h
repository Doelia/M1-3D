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


};

class Maillage {
public:
	vector<Face> faces;

	

	void addFace(Face f) {
		faces.push_back(f);
	}

	Point getCenter() {
		Point pMin = getMoreNegative();
		Point pMax = getMorePositive();
		Point p(
			(pMin.getX() + pMax.getX()) / 2,
			(pMin.getY() + pMax.getY()) / 2,
			(pMin.getZ() + pMax.getZ()) / 2);
		return p;
	}

	Point getMoreNegative() {
		Point* out = NULL;
		for (auto f : faces) {
			for (auto p : f.points) {
				if (out == NULL) {
					out = new Point(p);
				}
				if (p.getX() < out->getX())
					out->setX(p.getX());
				if (p.getY() < out->getY())
					out->setY(p.getY());
				if (p.getZ() < out->getZ())
					out->setZ(p.getZ());
			}
		}
		return *out;
	}

	Point getMorePositive() {
		Point* out = NULL;
		for (auto f : faces) {
			for (auto p : f.points) {
				if (out == NULL) {
					out = new Point(p);
				}
				if (p.getX() > out->getX())
					out->setX(p.getX());
				if (p.getY() > out->getY())
					out->setY(p.getY());
				if (p.getZ() > out->getZ())
					out->setZ(p.getZ());
			}
		}
		return *out;
	}

	float getBestSizeRepere(Vector center) {
		float size = 0;
		for (auto f : faces) {
			for (auto p : f.points) {
				Point x = (p);
				x.add(&center);

				if (fabs(x.getX()) > size)
					size = fabs(x.getX());
				if (fabs(x.getY()) > size)
					size = fabs(x.getY());
				if (fabs(x.getZ()) > size)
					size = fabs(x.getZ());
			
			}
		}
		return size;
	}

	void draw() {
		for (auto p : faces) {
			p.draw();
		}
	}

};

Maillage parseFile(const char* path) {

	FILE *f_maillage;
	Maillage maillage;

	if ((f_maillage = fopen(path, "rb")) == NULL) {
		printf("\nPas d'acces en lecture sur l'image %s \n", path);
		exit(EXIT_FAILURE);
	}
	else {
		int out;
		int nbrSommets, nbrFaces;
		fscanf(f_maillage, "OFF");
		fscanf(f_maillage, "%d %d %d", &nbrSommets, &nbrFaces, &out);
		printf("nbrSommets=%d, nbrFaces=%d\n", nbrSommets, nbrFaces);

		cout << "Chargement des sommets..." << endl;
		// Chargement des sommets
		Point* points = new Point[nbrSommets];
		for (int i = 0; i < nbrSommets; ++i) {
			float x,y,z;
			fscanf(f_maillage, "%f %f %f", &x, &y, &z);
			int scalar = 1;
			Point p(x*scalar,y*scalar,z*scalar);
			points[i].set(p);
		}

		cout << "Chargement des faces..." << endl;

		// Chargement des sommets
		char buffer[250];
		  while (fgets (buffer, sizeof(buffer), f_maillage)) {
		  	//printf("line = %s\n", buffer);
			Face f;
		    int nbrPoints = atoi(strtok(buffer, " "));
		    for (int j = 0; j < nbrPoints; ++j) {
				int pt = atoi(strtok(NULL, " ")); // TODO
				f.addPoint(points[pt]);
			}
			maillage.addFace(f);
		  }

		  cout << "Faces chargées ! " << endl;

	}

	return maillage;

}

#endif
