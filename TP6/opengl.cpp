bool maillageManuel = false;
bool maillageAuto = true;
bool lumiere = true;

#include <stdio.h>      
#include <stdlib.h>    
#include <math.h>
#include "../lib/Include.h"

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

GLint winWidth=WIDTH, winHeight=HEIGHT;
GLfloat eyeX=0.0, eyeY=0.0, eyeZ=2.0;

// Position sourie
GLfloat theta=270.0, phi=180.0;

Repere* r;

void eyePosition( void ) {
	if (r != NULL) {
		eyeX = r->center.getX() + r->size/2 * sin(theta*0.0174532) * sin(phi*0.0174532);
		eyeY = r->center.getY() + r->size/2 * cos(theta*0.0174532);
		eyeZ = r->center.getZ() + r->size/2 * sin(theta*0.0174532) * cos(phi*0.0174532);
		glutPostRedisplay();
	}
}

void onMouseMove(int x, int y) { 
	theta = (360.0/(double)winHeight)*(double)y*1.0; //3.0 rotations possible
	phi = (360.0/(double)winWidth)*(double)x*1.0; 
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
	glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();
	render_scene();
	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {  
	cout << "window_reshape()" << endl;
	glViewport(0, 0, width, height);
}

GLvoid window_key(unsigned char key, int x, int y) 
{  
	switch (key) {    
		case KEY_ESC:  
		exit(1); break; 
		case 43: break; // +
		case 45: break; // --
		case 97: lumiere = !lumiere; break;// a
		case 122: maillageManuel = !maillageManuel; break;// z
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

Maillage maillage;
float* normales;

void exercice1() {

	if (r == NULL) {
		maillage = parseFile("../ressources/triceratops.off");
		r = new Repere(maillage);
		cout << "center = " << r->center << endl;
		cout << "size = " << r->size << endl;
		normales = maillage.getTabNormalesSommets();
	}

	float sizeRepere = r->size;
	glOrtho(-sizeRepere, sizeRepere, -sizeRepere, sizeRepere, -sizeRepere, sizeRepere);
	gluLookAt(
		eyeX,eyeY,eyeZ,
		r->center.getX(),r->center.getY(),r->center.getZ(),
		0,1,0);
	
	glColor3f(0.5,0,0.5);
	if (lumiere) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnableClientState (GL_VERTEX_ARRAY);
		glEnableClientState (GL_NORMAL_ARRAY);
	}

	if (maillageAuto) {
		glVertexPointer(maillage.nbrPtsPerFace, GL_FLOAT, 0, maillage.getTabPoints());
		glNormalPointer (GL_FLOAT, 0, normales);

		glDrawElements (GL_TRIANGLES, maillage.getNbrIndices(), GL_UNSIGNED_INT, maillage.getTabIndices());
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	

	glColor3f(0,0.5,0.5);
	if (maillageManuel) {
		//maillage.draw();
	}

	glColor4f(1,1,1, 0.2f);
	if (maillageManuel) {
		maillage.drawNormalesOnSommet();
		maillage.drawNormales();
	}


}


void render_scene() {
	cout << "==================  RENDER  =======================" << endl;
	exercice1();
}

