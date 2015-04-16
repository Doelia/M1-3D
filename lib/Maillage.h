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
	int nbrPtsPerFace = 0; // Forcé à 3 depuis l'utilisation de l'indéxtation

	vector<Face> faces; // TOutes les faces calculées

	vector<Point> points; // Talbeau de points uniques 
	vector<Face>* indexationFaces = NULL; // Sommet => Faces


	void clear() {
		faces.clear();
		points.clear();
		if (indexationFaces != NULL) indexationFaces->clear();
	}


	// FONCTIONS STOCKAGE PAR FACES

	void addFace(Face f) {
		faces.push_back(f);
	}

	int pointExits(Point x) {
		int i = 0;
		for (Point p : points) {
			if (p.equalsTolerance(x, 0.0001f)) {
				return i;
			}
			i++;
		}
		return -1;
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

	void draw() {
		float i = 0;
		for (auto p : faces) {
			glColor3f(0,0,(i++) / nbrFaces);
			p.draw();
		}
	}

	void loadIndexationFaces() {
		indexationFaces = new vector<Face>[points.size()];
		for (Face f : faces) {
			for (int i : f.indices) {
				indexationFaces[i].push_back(f);
			}
		}
	}

	vector<Face> getFacesOfSommet(int indice) {
		if (indexationFaces == NULL)
			loadIndexationFaces();
		return indexationFaces[indice];
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

	float* getTabNormales() {
		cout << "chargement des normales..." << endl;
		float* tab = new float[nbrFaces*3];
		int i = 0;
		for (Face f : faces) {
			Vector v = f.getNormale();
			tab[i++] = v.getX();
			tab[i++] = v.getY();
			tab[i++] = v.getZ();
		}
		return tab;
	}

	// FONCTIONS STOCKAGE PAR INDEXATION / OFF

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

	GLfloat* getTabPoints() {
		GLfloat* tab = new GLfloat[points.size()*3];
		int i = 0;
		for (Point p : points) {
			cout << p << endl;
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
				cout << "indice[" << i << "] = " << indice << endl;
				tab[i++] = indice;
			}
		}
		return tab;
	}

	// Retoune l'indece. En créé un s'il n'existe pas encore
	int addNewPoint(Point p) {
		int indice = this->pointExits(p);
		if (indice >= 0) {
			return indice;
		} else {
			points.push_back(p);
			return points.size() - 1;
		}
	}

	Face createFaceAndIndexeIt(Point p1, Point p2, Point p3) {
		Face f;
		f.addPoint(p1);
		f.addPoint(p2);
		f.addPoint(p3);
		f.indices.push_back(addNewPoint(p1));
		f.indices.push_back(addNewPoint(p2));
		f.indices.push_back(addNewPoint(p3));
		return f;
	}

	// CHARGEMENTS / DECHARGEMENTS

	void loadCylindre(Point*** faces, int m) {

		this->clear();
		nbrPtsPerFace = 3;
		this->nbrFaces = m*2;

		for (int i = 0; i < m; ++i) {
			Point** ptsFaces = faces[i+2];
			addFace(createFaceAndIndexeIt(*ptsFaces[0], *ptsFaces[2], *ptsFaces[1]));
			addFace(createFaceAndIndexeIt(*ptsFaces[0], *ptsFaces[3], *ptsFaces[2]));
		}
	}

	void loadSphere(Point*** faces, int m) {

		this->clear();
		this->nbrFaces = m*m*2;
		this->nbrPtsPerFace = 3;

		cout << "nbrFaces = " << nbrFaces << endl;
		for (int i = 0; i < m-1; ++i) {
			for (int j = 0; j < m; ++j) {
				int nFace = i*m + j;
				Point** ptsFaces = faces[nFace];
				addFace(createFaceAndIndexeIt(*ptsFaces[0], *ptsFaces[2], *ptsFaces[1]));
				addFace(createFaceAndIndexeIt(*ptsFaces[0], *ptsFaces[3], *ptsFaces[2]));
			}
		}
	}

	void writeOffFormat() {

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
