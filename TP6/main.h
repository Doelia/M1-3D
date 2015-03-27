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
	vector<int> indices;

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

	Vector getNormale() {
		Vector v1(&(points[0]), &(points[1]));
		Vector v2(&(points[1]), &(points[2]));
		Vector* produit = v1.getProduitVectoriel(&v2);
		produit->normalize();
		return *produit;
	}


};


class Maillage {
public:
	int nbrFaces = 0;
	vector<Face> faces;
	vector<Point> points;
	int nbrPtsPerFace = 0;

	void addFace(Face f) {
		faces.push_back(f);
	}

	void draw() {
		for (auto p : faces) {
			p.draw();
		}
	}

	// AFFICHAGE DYNAMIQUE OPENGL

	GLfloat* getTabPoints() {
		GLfloat* tab = new GLfloat[points.size()*3];
		int i = 0;
		for (Point p : points) {
			tab[i++] = p.getX();
			tab[i++] = p.getY();
			tab[i++] = p.getZ();
		}
		return tab;
	}

	int getNbrIndices() {
		return nbrFaces*nbrPtsPerFace;
	}

	int* getTabIndices() {
		int* tab = new int[getNbrIndices()];
		int i = 0;
		for (Face f : faces) {
			for (int indice : f.indices) {
				tab[i++] = indice;
			}
		}
		return tab;
	}

	float* getTabNormales() {
		cout << "chargement des normales..." << endl;
		float* tab = new float[nbrFaces*3];
		int i = 0;
		for (Face f : faces) {
			Vector v = f.getNormale();
			//cout << "v = " << v << endl;
			tab[i++] = v.getX();
			tab[i++] = v.getY();
			tab[i++] = v.getZ();
		}
		return tab;
	}

};

class Repere {

	Maillage m;

public:

	Point center;
	float size;

	Repere(Maillage m) {
		this->m = m;
		this->center = getCenter();
		Vector v(center);
		Vector minus(-1, -1, -1);
		v.multiply(&minus);
		this->size = getBestSizeRepere(v);
		this->size = this->size * 2.0f;
	}

private:

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
		for (Face f : m.faces) {
			for (Point p : f.points) {
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
		for (Face f : m.faces) {
			for (Point p : f.points) {
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
		for (Face f : m.faces) {
			for (Point p : f.points) {
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
		int nbrSommets;
		fscanf(f_maillage, "OFF");
		fscanf(f_maillage, "%d %d %d", &nbrSommets, &maillage.nbrFaces, &out);
		printf("nbrSommets=%d, nbrFaces=%d\n", nbrSommets, maillage.nbrFaces);

		cout << "Chargement des sommets..." << endl;
		// Chargement des sommets
		Point* points = new Point[nbrSommets];
		for (int i = 0; i < nbrSommets; ++i) {
			float x,y,z;
			fscanf(f_maillage, "%f %f %f", &x, &y, &z);
			int scalar = 1;
			Point p(x*scalar,y*scalar,z*scalar);
			points[i].set(p);
			maillage.points.push_back(p);
		}

		cout << "Chargement des faces..." << endl;

		// Chargement des sommets
		char buffer[250];
		fgets (buffer, sizeof(buffer), f_maillage);
		  while (fgets (buffer, sizeof(buffer), f_maillage)) {
		  	printf("line = %s\n", buffer);
			Face f;
		    int nbrPoints = atoi(strtok(buffer, " "));
		    maillage.nbrPtsPerFace = nbrPoints;
		    for (int j = 0; j < nbrPoints; ++j) {
				int pt = atoi(strtok(NULL, " "));
				f.indices.push_back(pt);
				f.addPoint(points[pt]);
			}
			maillage.addFace(f);
		  }

		  cout << "Faces chargées ! " << endl;

	}

	return maillage;

}

#endif
