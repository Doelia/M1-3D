#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include "../lib/Include.h"
#include "main.h"

#define WIDTH  480
#define HEIGHT 480

#define RED   0.15
#define GREEN 0.15
#define BLUE  0.15
#define ALPHA 1

#define KEY_ESC 27

Vector* directionPlan = new Vector(0,0,1);
Point* centerPlan = new Point(0,0,0);
int m = 3; // Méridiens
int size = 50; // Taille du repére


void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 

void *thread_1(void *arg) {
    usleep(100*1000);
    thread_1(NULL);
    return NULL;
}

void start_thread() {
	cout << "start_thread" << endl;
	pthread_t thread1;
    pthread_create(&thread1, NULL, thread_1, NULL);
}

float deltaAngleX = 0.0f;
float deltaAngleY = 0.0f;
int xOrigin = -1;
int yOrigin = -1;

void mouseMove(int x, int y) { 	
    if (xOrigin >= 0) {
		deltaAngleX = (x - xOrigin) * 0.001f;
		deltaAngleY = (y - yOrigin) * 0.001f;
		
		float sinX = sin(deltaAngleX);
		float sinY = sin(deltaAngleY);
		float cosX = cos(deltaAngleX);
		float cosY = cos(deltaAngleY);

		directionPlan = new Vector(sinX*cosY, sinX*sinY, cosX);
		directionPlan->normalize();
		render_scene();
	}
}

void mouseButton(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			xOrigin = -1;
			yOrigin = -1;
		}
		else  {
			xOrigin = x;
			yOrigin = y;
		}
	}
}

int main(int argc, char **argv)  {  
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Premier exemple : carré");

	initGL();  
	init_scene();

	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(&window_key);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);


	glEnable(GL_BLEND);

	glutMainLoop();  

	return 1;
}

GLvoid initGL()  {
	glClearColor(RED, GREEN, BLUE, ALPHA);   
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);     
}

void init_scene() {
	glPointSize(3);
	start_thread();
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
	glOrtho(-size, size, -size, size, -size*100, size*100);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) 
{  
	switch (key) {    
	case KEY_ESC:  
		exit(1);                    
		break; 
	case 43: m++; break; // +
	case 45: m--; break; // --
	case 97: // a
	case 122: // z
	case 101: // e
	case 114: // r
	case 111: // o (haut)
	case 108: // l (bas)
	case 107: // k (gauche)
	case 109: // k (droite)
	case 113: // q
	case 115: // s 
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

void exercice1() {
	drawShpere();
}

void render_scene() {
	glClear(GL_COLOR_BUFFER_BIT);
	cout << "==================  RENDER  =======================" << endl;
	exercice1();
	glColor4f(0, 1.0f, 0, 0.5f);
	glRotatef(3, 1.0f, 1.0f, 0.1f);
	glFlush();
}

