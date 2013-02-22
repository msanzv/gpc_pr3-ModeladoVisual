/**********************************************
    PRACTICA DE USO DE UNA CAMARA SINTETICA
	R.Vivó, J.Lluch para GPC.etsinf.upv.es 2011 
 **********************************************/

#include <GL/glut.h>

#include "Primitivas.h"
#include "Camara.h"

void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

CamaraOrtografica ixus;


void myinit(void)
{
    glClearColor (1.0, 1.0, 1.0, 1.0);

	ixus.at(Punto(4.0,3.0,2.0)); //Situación de la cámara
	ixus.lookAt(Punto(0.0,0.0,0.0));
	ixus.setHeight(4.0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Dibujo del cubo
	Cubo().drawWith(ixus.getview());

	glFlush();
}


void myReshape(GLsizei w, GLsizei h)
{
    h = (h == 0) ? 1 : h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0,1.0,-1.0,1.0,-10000.0,-10000.0);
	ixus.setAspectRatio(w/(float)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Proyección Paralela de un Cubo");
   myinit();
   glutDisplayFunc(display); 
   glutReshapeFunc(myReshape);
   glutMainLoop();
   return(0);
}
 