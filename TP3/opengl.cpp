#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../lib/Include.h"
#include "ToolsOpenGL.h"
#include <functional>


#define WIDTH  480
#define HEIGHT 480

#define RED   0.15
#define GREEN 0.15
#define BLUE  0.15
#define ALPHA 1

#define KEY_ESC 27

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);


// Globales main
Point** pts3;
Point** pts4;
Point*** inChange = &pts3;
Point* modify;
int nbr = 4;

// CAMERA
Vector* directionPlan = new Vector(0,0,1);
Point* centerPlan = new Point(0,0,0);
float deltaAngleX = 0.0f;
float deltaAngleY = 0.0f;
float deltaMove = 0;
int xOrigin = -1;
int yOrigin = -1;
float angleX = 0.0f;
float angleY = 0.0f;

void mouseMove(int x, int y) {
	if (xOrigin >= 0) {
		deltaAngleX = (x - xOrigin) * 0.001f;
		deltaAngleY = (y - yOrigin) * 0.001f;
		float lx = sin(angleX + deltaAngleX);
		float ly = sin(angleY + deltaAngleY);
		float lz = -cos(angleX + deltaAngleX);
		directionPlan = new Vector(lx, ly, lz);
		render_scene();
	}
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			angleX += deltaAngleX;
			angleY += deltaAngleY;
			xOrigin = -1;
			yOrigin = -1;
		}
		else  {
			xOrigin = x;
			yOrigin = y;
		}
	}
}

void init_scene() {
	glPointSize(3);

	pts3 = new Point*[nbr];
	pts3[0] = new Point(0,0,0);
	pts3[1] = new Point(1,2,0);
	pts3[2] = new Point(3,3,0);
	pts3[3] = new Point(2,2,0);

	pts4 = new Point*[nbr];
	pts4[0] = new Point(1,0,0);
	pts4[1] = new Point(2,2,0);
	pts4[2] = new Point(4,3,0);
	pts4[3] = new Point(3,2,0);

	modify = pts3[0];
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Premier exemple : carré");
	glClearColor(RED, GREEN, BLUE, ALPHA);
	init_scene();
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);
	glutKeyboardFunc(&window_key);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutMainLoop();
	return 1;
}



GLvoid window_display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	render_scene();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int size = 10;
	glOrtho(-size, size, -size, size, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
		case KEY_ESC:
		exit(1); break;
		case 97: // a
		modify = (*inChange)[0]; break;
		case 122: // z
		modify = (*inChange)[1]; break;
		case 101: // e
		modify = (*inChange)[2]; break;
		case 114: // r
		modify = (*inChange)[3]; break;
		case 111: // o (haut)
		modify->setY(modify->getY()+.1); break;
		case 108: // l (bas)
		modify->setY(modify->getY()-.1); break;
		case 107: // k (gauche)
		modify->setX(modify->getX()-.1); break;
		case 109: // k (droite)
		modify->setX(modify->getX()+.1); break;
		case 113: // q (changer couleur)
		inChange = &pts3; break;
		case 115: // s (changer couleur)
		inChange = &pts4; break;
		default:
		printf ("La touche %d n´est pas active.\n", key);
		break;
	}

	render_scene();
}

void projectAll(Point** pts, int nb) {
	for (int i = 0; i < nb; ++i) {
		pts[i] = pts[i]->projectOnPlan(centerPlan, directionPlan);
	}
}

void exercice_1() {
	int nbrPoints = 11;

	std::function<Point*(double)> f1 = bezierCurveByBernstein(pts3, nbr);
	Point*** surfaceCylindrique = getSurfaceCylindrique(f1, new Vector(5,0,0), nbrPoints, nbrPoints);

	// Affichage
	drawMatrice(surfaceCylindrique, nbrPoints, nbrPoints);
	drawCurve(pts3, nbr);
}

void show_ptsControles() {
	// AFFICHAGE POINTS BEZIER JAUNE/ROUGE
	Point** pt3_c = copyPoints(pts3, nbr); // Points rouge
	Point** pt4_c = copyPoints(pts4, nbr); // Points jaunes
	projectAll(pt3_c, nbr);
	projectAll(pt4_c, nbr);
	glColor3f(1.0, 0, 0);
	drawCurve(pt3_c, nbr);
	glColor3f(1.0, 1.0, 0);
	drawCurve(pt4_c, nbr);

}

void exercice_2() {
	int nbrPoints = 11;
	std::function<Point*(double)> f1 = bezierCurveByBernstein(pts3, nbr);
	std::function<Point*(double)> f3 = bezierCurveByBernstein(pts4, nbr);
	Point** pts2 = surface(f1,f3, 70, 30);
	show_ptsControles();

	// Affichage
	//projectAll(pts2, 70*30);
	glColor3f(0, 1.0, .5f);
	drawPoints(pts2, 70*30);
}

void exercice_3() {
	int nbrPoints = 40;

	int nbrU = nbr;
	int nbrV = nbr;
	glPointSize(5);
	std::function<Point*(double, double)> f4 = surfaceByCasteljau(pts3, nbrU, pts4, nbrV);
	glPointSize(2);
	Point*** matrice = getMatriceFromBezier(pts3, nbrU, pts4, nbrV);
	Point** pts5 = discretiserDouble(f4,nbrPoints,nbrPoints);
	projectAll(pts5, nbrPoints*nbrPoints);
	// AFFICHAGE EXERCICE 3
	glPointSize(2);
	glColor3f(0, 1.0, 1.0);
	drawPoints(pts5, nbrPoints*nbrPoints);
	glColor3f(0, 1.0, 0);
	drawMatrice(matrice, nbrU, nbrV);
	glColor3f(1.0, 1.0, 0);
	show_ptsControles();
}


void render_scene() {

	cout << "========================================================" << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	exercice_3();
	glFlush();

}

