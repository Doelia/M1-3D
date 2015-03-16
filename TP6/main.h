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
		glBegin(GL_LINES);
		for (auto p : points) {
			drawPoint(&p);
		}
		glEnd();
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
			int scalar = 100;
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
			faces.push_back(f);
		  }

		  cout << "Faces chargées ! " << endl;

	}

	return faces;

}

#endif
