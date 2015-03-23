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

float size = 10; // Taille du repére

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 

GLint winWidth=WIDTH, winHeight=HEIGHT;
GLfloat eyeX=0.0, eyeY=0.0, eyeZ=2.0;
GLfloat theta=270.0, phi=180.0;
GLfloat upX=0.0, upY=1.0, upZ=0.0;
GLfloat r=1;

void eyePosition( void ) {
	eyeX = r * sin(theta*0.0174532) * sin(phi*0.0174532);
	eyeY = r * cos(theta*0.0174532);
	eyeZ = r * sin(theta*0.0174532) * cos(phi*0.0174532);
	GLfloat dt=1.0;
	GLfloat eyeXtemp = r * sin(theta*0.0174532-dt) * sin(phi*0.0174532);
	GLfloat eyeYtemp = r * cos(theta*0.0174532-dt);
	GLfloat eyeZtemp = r * sin(theta*0.0174532-dt) * cos(phi*0.0174532); 

	upX=eyeXtemp-eyeX;
	upY=eyeYtemp-eyeY;
	upZ=eyeZtemp-eyeZ;

	glutPostRedisplay();
}

void onMouseMove(int x, int y) { 
	// Mouse point to angle conversion
	theta = (360.0/(double)winHeight)*(double)y*1.0; //3.0 rotations possible
	phi = (360.0/(double)winWidth)*(double)x*1.0; 
	// Restrict the angles within 0~360 deg (optional)
	if(theta > 360) theta = fmod((double)theta,360.0);
	if(phi > 360) phi = fmod((double)phi,360.0);
	eyePosition();
}

int main(int argc, char **argv)  {  
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Premier exemple : carré");
	initGL();  
	init_scene();

	glutPassiveMotionFunc(&onMouseMove);

	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);
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
}

GLvoid window_display() {
	cout << "window_display()" << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);    
	glLoadIdentity();
	gluLookAt(eyeX,eyeY,eyeZ, 0,0,0, upX,upY,upZ);
	render_scene();
	glutSwapBuffers();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {  
	cout << "window_reshape()" << endl;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);    
}

GLvoid window_key(unsigned char key, int x, int y) 
{  
	switch (key) {    
	case KEY_ESC:  
		exit(1);                    
		break; 
	case 43: break; // +
	case 45: break; // --
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

void exercice1() {
	vector<Face> faces = parseFile("res/bunny.off");
	Point center = Face::getCenter(faces);
	cout << "center = " << center << endl;
	Vector v(0,0,0);

	float size = Face::getBestSizeRepere(faces, v);
	size *= 5.0f;
	cout << "size = " << size << endl;

	Point pMin =  Face::getMoreNegative(faces);
	Point pMax =  Face::getMorePositive(faces);

	cout << "pMin=" << pMin << endl;
	cout << "pMax=" << pMax << endl;

	//glOrtho(pMin.getX(), pMax.getX(), pMin.getY(), pMax.getY(), pMin.getZ(), pMax.getZ());
	glOrtho(-size, size, -size, size, -size, size);
	//gluLookAt(0,0,0, 0.5f,0,0, 0,1,0);

	Face::drawSet(faces);


}


void render_scene() {
	cout << "==================  RENDER  =======================" << endl;
	exercice1();
}

