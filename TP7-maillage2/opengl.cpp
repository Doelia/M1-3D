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

int m = 6;

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
		eyeX = r->center.getX() + r->size/2 * - sinf(phi * (M_PI / 180)) * cosf((theta) * (M_PI / 180));
		eyeY = r->center.getY() + r->size/2 * - sinf((theta) * (M_PI / 180));
		eyeZ = r->center.getZ() - r->size/2 * cosf((phi) * (M_PI / 180)) * cosf((theta) * (M_PI / 180));
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
	glutCreateWindow("TP 3D");

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LEQUAL);
	glClearColor(RED, GREEN, BLUE, ALPHA);   
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutPassiveMotionFunc(&onMouseMove);
	glutDisplayFunc(&window_display);
	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);
	glDisable(GL_BLEND);
	glutMainLoop();  

	return 1;
}


GLvoid window_display() {
	cout << "window_display()" << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	render_scene();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {  
	glViewport(0, 0, width, height);
}

GLvoid window_key(unsigned char key, int x, int y) 
{  
	switch (key) {    
		case KEY_ESC:  
		exit(1); break; 
		case 43: m++; break; // +
		case 45: m--; break; // --
		case 97:  break;// a
		case 122:break;// z
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
	window_display();
}

Maillage maillage;


void exercice1() {

	cout << "m =" << m << endl;

	maillage.clear();

	Point*** cylindre = generateCylindre(10,30,m);
	Point*** shpere = generateSphere(10,new Point(0,0,0),m,m);

	maillage.loadCylindre(cylindre, m);
	//maillage.loadSphere(shpere, m);
	//maillage = parseFile("../ressources/triceratops.off");

	r = new Repere(maillage);
	cout << "center = " << r->center << endl;
	cout << "size = " << r->size << endl;

	float sizeRepere = r->size;


	glMatrixMode(GL_PROJECTION);    
	glLoadIdentity();
	glOrtho(-sizeRepere, sizeRepere, -sizeRepere, sizeRepere, -sizeRepere, sizeRepere);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
	eyeX,eyeY,eyeZ,
	r->center.getX(),r->center.getY(),r->center.getZ(),
	0,1,0);

	if (false) {
		maillage.draw();
	} else {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float positions[] =  {10,10,0,.8};
		glLightfv(GL_LIGHT0, GL_POSITION, positions);

		glEnableClientState (GL_VERTEX_ARRAY);
		glEnableClientState (GL_NORMAL_ARRAY);
		glVertexPointer(maillage.nbrPtsPerFace, GL_FLOAT, 0, maillage.getTabPoints());
		float* normales = maillage.getTabNormalesSommets();
		glNormalPointer (GL_FLOAT, 0, normales);
		cout << maillage.getNbrIndices() << " indices" << endl;
		//maillage.getTabIndices();
		glDrawElements (GL_TRIANGLES, maillage.getNbrIndices(), GL_UNSIGNED_INT, maillage.getTabIndices());
	}
	

	glColor3f(1,1,1);
	//maillage.drawNormales();

	glFlush();
	glutSwapBuffers();


}


void render_scene() {
	cout << "==================  RENDER  =======================" << endl;
	exercice1();
}

