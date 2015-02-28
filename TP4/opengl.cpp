///////////////////////////////////////////////////////////////////////////////
// Imagina
// ----------------------------------------------------------------------------
// IN - Synthèse d'images - Modélisation géométrique
// Auteur : Gilles Gesquière
// ----------------------------------------------------------------------------
// Base du TP 1
// programme permettant de créer des formes de bases.
// La forme représentée ici est un polygone blanc dessiné sur un fond rouge
///////////////////////////////////////////////////////////////////////////////  

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include "../lib/Include.h"
#include "main.h"

/* Dans les salles de TP, vous avez généralement accès aux glut dans C:\Dev. Si ce n'est pas le cas, téléchargez les .h .lib ...
Vous pouvez ensuite y faire référence en spécifiant le chemin dans visual. Vous utiliserez alors #include <glut.h>. 
Si vous mettez glut dans le répertoire courant, on aura alors #include "glut.h" 
*/


// Définition de la taille de la fenêtre
#define WIDTH  480
#define HEIGHT 480

// Définition de la couleur de la fenêtre
#define RED   0.15
#define GREEN 0.15
#define BLUE  0.15
#define ALPHA 1


// Touche echap (Esc) permet de sortir du programme
#define KEY_ESC 27


Vector* directionPlan = new Vector(0,0,1);
Point* centerPlan = new Point(0,0,0);

// Entêtes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 


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
		//cout << "directionPlan = " << *directionPlan << endl;
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

int main(int argc, char **argv) 
{  
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


	glEnable(GL_DEPTH_TEST);

  // la boucle prinicipale de gestion des événements utilisateur
	glutMainLoop();  

	return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque
GLvoid initGL() 
{
	glClearColor(RED, GREEN, BLUE, ALPHA);        
}

// Initialisation de la scene. Peut servir à stocker des variables de votre programme
// à initialiser
void init_scene()
{
	glPointSize(3);
}



// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

  // C'est l'endroit où l'on peut dessiner. On peut aussi faire appel
  // à une fonction (render_scene() ici) qui contient les informations 
  // que l'on veut dessiner
	render_scene();

  // trace la scène grapnique qui vient juste d'être définie
	glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{  
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  // ici, vous verrez pendant le cours sur les projections qu'en modifiant les valeurs, il est
  // possible de changer la taille de l'objet dans la fenêtre. Augmentez ces valeurs si l'objet est 
  // de trop grosse taille par rapport à la fenêtre.
	int size = 11;
	glOrtho(-size, size, -size, size, -size*2, size*2);
  //glOrtho(0, size, 0, size, 0, size);

  // toutes les transformations suivantes s´appliquent au modèle de vue 
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) 
{  
	switch (key) {    
		case KEY_ESC:  
		exit(1);                    
		break; 

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

//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////
void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	cout << "==================  RENDER  =======================" << endl;

	Point*** pts = generateCylindre(10,20,10);

	projectAll(pts[0], 10);
	projectAll(pts[1], 10);

	glColor4f(1.0, 0, 1.0, 0.5f);
	drawCylindre(pts, 10);

	glFlush();

}

