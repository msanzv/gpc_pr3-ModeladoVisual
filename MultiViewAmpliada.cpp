/**********************************************
    PRACTICA DE USO DE LA CAMARA SINTETICA
	CON 4 CAMARAS Y MULTIVISTA
 **********************************************/
// Prof: R. Vivó, J.Lluch
// Alumno:

#include <GL/glut.h>
#include <stdio.h>
#include "Primitivas.h"
#include "Camara.h"
#include "SuperficieBezier.h"
#include "Teapot.h"

//La ventana del mundo real debe ser lo suficientemente grande como para
//ver todo el objeto. Selecciono una ventana de 8 unidades de ALTO con la misma
//razon de aspecto que el viewport (ver reshape)
//Debido a que se presenta en una ventana 1/4 más pequeña habrá que aplicar
//un escalado de 1/2 en cada dirección

#define ALTO 8.0
#define DESP 1.0f/2		//Desplazamiento sobre la ventana canónica de 2x2

#define NVERT 10
#define PCONTROL 16

void myinit(void);
void buttonMouse(int key, int state, int x, int y);
void moveMouse(int x, int y);
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

//Tetera
Punto sb[NVERT*NVERT];
Vector n[NVERT*NVERT];
Vector tgU[NVERT*NVERT];
Vector tgV[NVERT*NVERT];
bool inalambrico = true;

//Movimiento de la camara
Punto AT = Punto(4.0, 5.0, 5.0);
Punto FR = Punto(0.0, 0.0, 2.0);
Punto PE = Punto(2.0, 0.0, 0.0);
Punto CE = Punto(0.0, 0.0,-1.0);

//Movimiento del ratón
float last_x, last_y, last_z;
float rotationX = 0.0, rotationY = 0.0, rotationZ = 0.0;

void myinit(void){

	glClearColor (1.0, 1.0, 1.0, 1.0);
	
	//Camara perspectiva
	canon.at(AT);		
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
	NE.scale(0.5, 0.5);
	SO.translation(Vector(-DESP, -DESP, 0.0));
	SO.scale(0.5, 0.5);
	SE.translation(Vector( DESP, -DESP, 0.0));
	SE.scale(0.5, 0.5);
	
	//Inicializacion de la iluminacion
	GLfloat light_position[] = {4.0, 9.0,-4.0, 0.0};
	GLfloat mat_ambient   [] = {0.5, 0.5, 0.5, 1.0};
	GLfloat mat_diffuse   [] = {0.0, 0.0, 0.0, 1.0};
	GLfloat mat_specular  [] = {0.9, 0.9, 0.9, 1.0};
	GLfloat mat_shininess [] = {5.0};

	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse); 
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular); 
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); 
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 's': inalambrico = !inalambrico; break;
		case 'q': exit(1); break;
		default: break;
	}

	glutPostRedisplay();
}

void buttonMouse(int key, int state, int x, int y){

	// Callback cuando pulsamos los botonos del ratón
	if(key == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		last_x = (float) x;
		last_y = (float) y;
		//last_z = z;
	}

	glutPostRedisplay();
}

void moveMouse(int x, int y){

	rotationX += (float)(last_y - y);
	rotationY += (float)(x - last_x);
	//rotationZ += (float) (last_z - z);
	last_x = (float) x;
	last_y = (float) y;
	//last_z = z;

	glutPostRedisplay(); 
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

	float x_color[] ={1.0,0.0,0.0,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,x_color);
	Flecha((x-o)*3,Color::ROJO).drawAt(o);
	float y_color[] ={0.0,1.0,0.0,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,y_color);
	Flecha((y-o)*3,Color::VERDE).drawAt(o);
	float z_color[] ={0.0,0.0,1.0,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,z_color);
	Flecha((z-o)*3,Color::AZUL).drawAt(o);
}

void displayCube(Camara &cam, Transformacion marco){
//Dibuja la foto de un Cubo con la cam y la pone en t

	Transformacion t = marco * cam.getview();

	glColor3f(0,0,1);

	Cubo c;
	c.drawWith(t);
}

void displayTeapot(Camara &cam, Transformacion marco){
//Dibuja la foto de un Cubo con la cam y la pone en t

	// Transformacion de la camara
	Transformacion rotcam;
	Transformacion rotationx, rotationy;
	Punto newAT = AT;
	rotationx.rotX(rotationX);
	rotationy.rotY(rotationY);

	newAT = newAT.transform(rotationx);
	newAT = newAT.transform(rotationy);
	
	Punto vert = Punto(0.0, 1.0, 0.0);
	vert.transform(rotcam);

	canon.at(newAT);
	canon.lookAt(OR);
	canon.setVertical(vert.asVector());

	Punto newFR = FR;
	newFR = newFR.transform(rotationy);
	frontal.at(newFR);
	frontal.lookAt(OR);

	Punto newPE = PE;
	newPE = newPE.transform(rotationy);
	perfil.at(newPE);
	perfil.lookAt(OR);

	Punto newCE = CE;
	newCE = newCE.transform(rotationy);
	cenital.setVertical(newCE.asVector());
	
	rotationx.reset();
	rotationy.reset();


	Transformacion t = marco * cam.getview();
	
	//Dibujo de la tetera
	Transformacion rot; 
	rot.rotX(-90);
	Transformacion mov;
	mov.translation(Real4(0.0, -1.0, 0.0));

	
	for(int k=0;k<32;k++){
		S[k].getPoints(NVERT, sb);
		S[k].getTangents(NVERT, tgU, tgV);
		S[k].getNormals(NVERT, n);
	
		for(int i=0;i<NVERT*NVERT;i++){	
			sb[i] = sb[i].transform(rot).transform(mov);
			sb[i] =	cam.shot(sb[i]).transform(marco);	
		
			float sup_color[] = {0.5, 0.5, 0.5, 1.0}; //Gris
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sup_color);
		}

		if(inalambrico){
			for(int i=0;i<NVERT-1;i++){
				for(int j=0;j<NVERT-1;j++){			
					glBegin(GL_QUADS);
					glNormal3f(n[NVERT*i+j].x(),n[NVERT*i+j].y(),n[NVERT*i+j].z());
					glVertex3f(sb[NVERT*i+j].x(),sb[NVERT*i+j].y(),sb[NVERT*i+j].z());

					glNormal3f(n[NVERT*(i+1)+j].x(),n[NVERT*(i+1)+j].y(),n[NVERT*(i+1)+j].z());
					glVertex3f(sb[NVERT*(i+1)+j].x(),sb[NVERT*(i+1)+j].y(),sb[NVERT*(i+1)+j].z());

					glNormal3f(n[NVERT*(i+1)+(j+1)].x(),n[NVERT*(i+1)+(j+1)].y(),n[NVERT*(i+1)+(j+1)].z());
					glVertex3f(sb[NVERT*(i+1)+(j+1)].x(),sb[NVERT*(i+1)+(j+1)].y(),sb[NVERT*(i+1)+(j+1)].z());

					glNormal3f(n[NVERT*i+(j+1)].x(),n[NVERT*i+(j+1)].y(),n[NVERT*i+(j+1)].z());
					glVertex3f(sb[NVERT*i+(j+1)].x(),sb[NVERT*i+(j+1)].y(),sb[NVERT*i+(j+1)].z());

					glEnd();
				};	
			};
		}
		else{
			for(int i=0;i<NVERT;i++){
				glBegin(GL_LINE_STRIP);
					for(int j=0;j<NVERT;j++){
						glVertex3f( (GLfloat)((sb[i*NVERT+j]).x()),
									(GLfloat)((sb[i*NVERT+j]).y()),
									(GLfloat)((sb[i*NVERT+j]).z()));
					}
				glEnd();
			}
 			for(int i=0;i<NVERT;i++){
				glBegin(GL_LINE_STRIP);
					for(int j=0;j<NVERT;j++){
						glVertex3f( (GLfloat)((sb[i+j*NVERT]).x()),
									(GLfloat)((sb[i+j*NVERT]).y()),
									(GLfloat)((sb[i+j*NVERT]).z()));
					}
				glEnd();
			}	
		}
	}
}

void display(){
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Dibujo de los ejes
	displayAxis(canon, NO);
	displayAxis(frontal, SE);
	displayAxis(cenital, NE);
	displayAxis(perfil, SO);
	
	//Dibujo del cubo
	if(!inalambrico){
		displayCube(canon, NO);
		displayCube(frontal, SE);
		displayCube(cenital, NE);
		displayCube(perfil, SO);
	}
	
	//Dibujo de la tetera
	displayTeapot(canon, NO);
	displayTeapot(frontal, SE);
	displayTeapot(cenital, NE);
	displayTeapot(perfil, SO);


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

	glutPostRedisplay();
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
   glutMotionFunc(moveMouse);
   glutMouseFunc(buttonMouse);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return(0);
}


