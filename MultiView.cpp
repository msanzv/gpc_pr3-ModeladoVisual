/**********************************************
    PRACTICA DE USO DE LA CAMARA SINTETICA
	CON 4 CAMARAS Y MULTIVISTA
 **********************************************/
// Prof: R. Vivó, J.Lluch
// Alumno:

#include <GL/glut.h>
#include "Primitivas.h"
#include "Camara.h"

//La ventana del mundo real debe ser lo suficientemente grande como para
//ver todo el objeto. Selecciono una ventana de 8 unidades de ALTO con la misma
//razon de aspecto que el viewport (ver reshape)
//Debido a que se presenta en una ventana 1/4 más pequeña habrá que aplicar
//un escalado de 1/2 en cada dirección

#define ALTO 8.0
#define DESP 1.0f/2		//Desplazamiento sobre la ventana canónica de 2x2

void myinit(void);
void myReshape(GLsizei w, GLsizei h);
void display(void);

//Cámaras
CamaraPerspectiva canon;
CamaraOrtografica cenital, perfil, frontal;

//Transformaciones que se aplican a las fotos para situarlas en la ventana
Transformacion NO, NE, SO, SE;

//Ejes
Punto OR = Punto();
Punto EX = Punto(1.0);
Punto EY = Punto(0.0,1.0);
Punto EZ = Punto(0.0,0.0,1.0);

void myinit(void){

	glClearColor (1.0, 1.0, 1.0, 1.0);
	
	//Camara perspectiva
	canon.at(Punto(2.0, 0.5, 3.0));		
	canon.lookAt(OR);
	canon.setVertical(Vector(0.0, 1.0, 0.0));

	//Camara en el techo - Planta
	cenital.at(Punto(0.0, 2.0, 0.0));		
	cenital.lookAt(OR);
	cenital.setVertical(Vector(0.0, 0.0, -1.0));
	cenital.setHeight(ALTO);

	//Cámara de perfil - Perfil
	perfil.at(Punto(2.0, 0.0, 0.0));		
	perfil.lookAt(OR);
	perfil.setVertical(Vector(0.0, 1.0, 0.0));
	perfil.setHeight(ALTO);

	//Cámara de frente - Alzado
	frontal.at(Punto(0.0, 0.0, 2.0));		
	frontal.lookAt(OR);
	frontal.setVertical(Vector(0.0, 1.0, 0.0));
	frontal.setHeight(ALTO);

	//Transformaciones de las fotos (LIFO)
	NO.translation(Vector(-DESP,  DESP, 0.0));
	NO.scale(0.5, 0.5);
	NE.translation(Vector( DESP,  DESP, 0.0));
	//NE.scale(0.5, 0.5);
	SO.translation(Vector(-DESP, -DESP, 0.0));
	//SO.scale(0.5, 0.5);
	SE.translation(Vector( DESP, -DESP, 0.0));
	//SE.scale(0.5, 0.5);
}

void displayAxis(Camara &cam, Transformacion marco){
//Fotografía con la camara c y muestra en el marco

	Punto o = cam.shot(OR).transform(marco);
	Punto x = cam.shot(EX).transform(marco);
	Punto y = cam.shot(EY).transform(marco);
	Punto z = cam.shot(EZ).transform(marco);

	Flecha((x-o), Color::ROJO ).drawAt(o);
	Flecha((y-o), Color::VERDE).drawAt(o);
	Flecha((z-o), Color::AZUL ).drawAt(o);
}
void displayCube(Camara &cam, Transformacion marco){
//Dibuja la foto de un Cubo con la cam y la pone en t

	Transformacion t = marco * cam.getview();

	Cubo c;
	c.drawWith(t);
}

void display(void){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	displayAxis(canon, NO);
	displayAxis(frontal, SE);
	displayAxis(cenital, NE);
	displayAxis(perfil, SO);
	
	glColor3f(0,0,1);
	displayCube(canon,NO);
	displayCube(frontal,SE);
	displayCube(cenital,NE);
	displayCube(perfil,SO);
	glFlush();
}


void myReshape(GLsizei w, GLsizei h){

    h = (h == 0) ? 1 : h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-1.0,1.0,-1.0,1.0,-10000.0,10000.0);
	canon.setAspectRatio(w/(float)h);
	cenital.setAspectRatio(w/(float)h);
	frontal.setAspectRatio(w/(float)h);
	perfil.setAspectRatio(w/(float)h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv){

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Montaje de 4 cámaras");
   myinit();
   glutDisplayFunc(display); 
   glutReshapeFunc(myReshape);
   glutMainLoop();
   return(0);
}


