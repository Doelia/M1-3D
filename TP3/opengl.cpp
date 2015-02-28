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
#include "ToolsOpenGL.h"



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


// Entêtes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 


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


int main(int argc, char **argv) 
{  
  // initialisation  des paramètres de GLUT en fonction
  // des arguments sur la ligne de commande
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);

  // définition et création de la fenêtre graphique, ainsi que son titre
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Premier exemple : carré");

  // initialisation de OpenGL et de la scène
  initGL();  
  init_scene();

  // choix des procédures de callback pour 
  // le tracé graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fenêtre
  glutReshapeFunc(&window_reshape);
  // la gestion des événements clavier
  glutKeyboardFunc(&window_key);

  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);

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
  int size = 10;
  glOrtho(-size, size, -size, size, -1000, 1000);
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

//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////
void render_scene()
{

  cout << "========================================================" << endl;

  int nbrPoints = 30;

  glClear(GL_COLOR_BUFFER_BIT);


  std::function<Point*(double)> f1 = bezierCurveByBernstein(pts3, nbr);
  std::function<Point*(double)> f3 = bezierCurveByBernstein(pts4, nbr);
  std::function<Point*(double)> f2 = getDroite(new Point(1,2,0), new Point(2,3,0));

  //Point** pts2 = discretiser(f1,10);
  Point** pts2 = surface(f1,f3, 70, 30);

  Point** pt3_c = copyPoints(pts3, nbr);
  Point** pt4_c = copyPoints(pts4, nbr);

  projectAll(pt3_c, nbr);
  projectAll(pt4_c, nbr);
   glColor3f(1.0, 0, 0);
  drawCurve(pt3_c, nbr);
  glColor3f(1.0, 1.0, 0);
  drawCurve(pt4_c, nbr);

  /*
  projectAll(pts2, 70*30);
  glColor3f(0, 1.0, 1.0);
  drawPoints(pts2, 70*30);
  //*/

  //*

  int nbrU = nbr;
  int nbrV = nbr;

  Point** pU = new Point*[3];
  Point** pV = new Point*[3];
  pU[0] = new Point(1,1,0);
  pU[1] = new Point(2,1,0);
  pU[2] = new Point(3,1,0);
  pV[0] = new Point(1,4,0);
  pV[1] = new Point(1,5,0);
  pV[2] = new Point(1,6,0);

  std::function<Point*(double, double)> f4 = surfaceByCasteljau(pts3, nbrU, pts4, nbrV);
  Point*** matrice = getMatriceFromBezier(pts3, nbrU, pts4, nbrV);

  Point** pts5 = discretiserDouble(f4,nbrPoints,nbrPoints);
  projectAll(pts5, nbrPoints*nbrPoints);

  glColor3f(0, 1.0, 1.0);
  drawPoints(pts5, nbrPoints*nbrPoints);
  glColor3f(0, 1.0, 0);
  drawMatrice(matrice, nbrU, nbrV);
  glColor3f(1.0, 0, 1.0);
  drawPoints(pU, 3);
  glColor3f(1.0, 1.0, 0.5);
  drawPoints(pV, 3);
  glColor3f(1.0, 1.0, 0);

  //*/


  glFlush();

}

