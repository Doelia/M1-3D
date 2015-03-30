#ifndef MAILLAGE_H
#define MAILLAGE_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "Include.h"
#include "Face.h"
#include "Drawer.h"
#include <Vector>

using namespace std;

class Maillage {
public:
	int nbrFaces = 0;
	vector<Face> faces;
	vector<Point> points; // Talbeau de points uniques
	int nbrPtsPerFace = 0;

	void addFace(Face f) {
		faces.push_back(f);
	}

	void draw() {
		float i = 0;
		for (auto p : faces) {
			glColor3f(0,0,(i++) / nbrFaces);
			p.draw();
		}
	}

	void drawNormales() {
		for (Face f : faces) {
			Vector n = f.getNormale();
			Point center = f.barycenter();
			drawVector(center, n);
		}
	}

	void drawNormalesOnSommet() {
		cout << "Affichage des normales des sommets..." << endl;
		for (int i = 0; i < points.size(); ++i) {
			Vector n = getNormaleOfSommet(i);
			Point center = points[i];
			drawVector(center, n);
		}
		cout << "OK" << endl;
	}

	vector<Face> getFacesOfSommet(int indice) {
		vector<Face> tab;
		for (Face f : faces) {
			for (int i : f.indices) {
				if (i == indice) {
					tab.push_back(f);
				}
			}
		}
		return tab;
	}

	Vector getNormaleOfSommet(int indice) {
		vector<Face> faces = getFacesOfSommet(indice);
		float sumX = 0, sumY = 0, sumZ = 0, cpt = 0;
		for (Face f : faces) {
			Vector v = f.getNormale();
			sumX += v.getX();
			sumY += v.getY();
			sumZ += v.getZ();
			cpt++;
		}
		Vector n(sumX/cpt, sumY/cpt, sumZ/cpt);
		n.normalize();
		return n;
	}

	float* getTabNormalesSommets() {
		cout << "Chargement des normales des sommets..." << endl;
		float* tab = new float[points.size()*3];
		int cpt = 0;
		for (int i = 0; i < points.size(); ++i) {
			Vector normale = getNormaleOfSommet(i);
			tab[cpt++] = normale.getX();
			tab[cpt++] = normale.getY();
			tab[cpt++] = normale.getZ();
		}
		cout << "OK" << endl;
		return tab;
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

	void clear() {
		faces.clear();
		points.clear();
	}

	void loadCylindre(Point*** faces, int m) {

		this->clear();

		int nbrPtsOnFace;
		this->nbrFaces = m*2;

		for (int i = 0; i < m; ++i) {
			Point** ptsFaces = faces[i+2];
			nbrPtsOnFace = 4;
			Face f1, f2;
			f1.addPoint(*ptsFaces[0]);
			f1.addPoint(*ptsFaces[2]);
			f1.addPoint(*ptsFaces[1]);
			f2.addPoint(*ptsFaces[0]);
			f2.addPoint(*ptsFaces[3]);
			f2.addPoint(*ptsFaces[2]);
			addFace(f1);
			addFace(f2);
		}
	}

	void loadSphere(Point*** faces, int m) {
		this->clear();
		this->nbrFaces = m*m*2;
		cout << "nbrFaces = " << nbrFaces << endl;
		for (int i = 0; i < m-1; ++i) {
			for (int j = 0; j < m; ++j) {
				int nFace = i*m + j;
				Point** ptsFaces = faces[nFace];
				Face f1, f2;
				f1.addPoint(*ptsFaces[0]);
				f1.addPoint(*ptsFaces[2]);
				f1.addPoint(*ptsFaces[1]);
				f2.addPoint(*ptsFaces[0]);
				f2.addPoint(*ptsFaces[3]);
				f2.addPoint(*ptsFaces[2]);
				addFace(f1);
				addFace(f2);
			}
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
			//printf("line = %s\n", buffer);
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
