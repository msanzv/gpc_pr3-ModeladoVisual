/* IMPLEMENTACIÓN DE LA CLASE CAMARA Y DERIVADAS
Prof: R.Vivó, J.Lluch para GPC.etsinf.upv.es 2011
Alumno:											*/

#include "Camara.h"
#include <math.h>

// class Camara +++++++++++++++++++++++++++++++++++++++++++++++++++++++

Camara::Camara(){
//Constructor comun a los dos tipos de camara

	pov = Punto(0.0,0.0,0.0);
	look = Vector(0.0,0.0,-1.0);
	up = Vector(0.0,1.0,0.0);
	aspectRatio = 4.0f/3.0f;
	near = 1.0f; 
	far = 10.0f;
	ready = 0;
};

void Camara::setView(){
//Calcula la transformación de la vista general

	/* Introduce aquí tu código */
	Vector w = look.negated().unitary();
	Vector u = (up^w).unitary();
	Vector v = w^u;

	view.rotation(u,v,w);
	view.translation(pov.negated());

	ready = 1;
};

void Camara::at(Punto pos){
//Coloca la camara

	pov = pos;
	ready = 0;									
};

void Camara::lookAt(Punto interes){
//Apunta la camara hacia mira

	look = (interes-pov).unitary();
	ready = 0;
};

void Camara::lookTo(Vector direccion){
//Apunta la camara en esa direccion

	look = direccion.unitary();
	ready = 0;
};

void Camara::setVertical(Vector arriba){
//Fija la vertical de la camara

	up = arriba.unitary();
	ready = 0;
};


void Camara::setAspectRatio(float razon){
//Dimensiones de la foto

	aspectRatio = razon;
	ready = 0;
};

void Camara::setFOV(float cca, float ljs){
//Limitacion del campo

	near = cca; 
	far = ljs;
	ready = 0;
};

Punto Camara::shot(Punto p){
//Aplica al punto la transformacion de la vista

	/* Introduce aquí tu código */
	if(ready == 0)
		setView();
	return p.transform(view).homogen();
};

Transformacion Camara::getview(){
//Devuelve la matriz de la vista

	if(!ready) 
		setView();
	return view;
};

// clase Camara Ortográfica +++++++++++++++++++++++++++++++++++++++++++++++++

CamaraOrtografica::CamaraOrtografica(){

	height = 2.0f;
};

void CamaraOrtografica::setView(){
//Se compone del escalado ortoedrico y la transformacion de la vista general
//So*Tc*Rn*Tn
//recuerda que es LIFO

	/* Introduce aquí tu código */
	view.reset();

	float sx, sy, sz;
	sx = 2/(height*aspectRatio);
	sy = 2/height;
	sz = 1/far;

	view.scale(sx,sy,sz);
	view.translation(Real4(0,0,near,0));

	Camara::setView();
};

void CamaraOrtografica::setHeight(float h){

	height = h;
	ready = 0;
};

void CamaraOrtografica::getParam(Punto &pos, Vector &direccion, Vector &arriba,
								 float &alt, float &anch, float &cca, float &ljs) const{

	pos = pov;
	direccion = look;
	arriba = up;
	alt = height;
	anch = height*aspectRatio;
	cca = near;
	ljs = far;
};

// clase Camara Perspectiva +++++++++++++++++++++++++++++++++++++++++++++

CamaraPerspectiva::CamaraPerspectiva(){

	verticalAngle = 60.0f;
};

void CamaraPerspectiva::setView(){
//Se compone de la transformacion perspectiva, el escalado a la piramide canónica
// y la transformacion de la vista general
// Mpp*Sp*Rn*Tn
//Recuerda que es LIFO

	/* Introduce aquí tu código */
	view.reset();

	float k = near/far;
	Matriz Mpp = Matriz(Real4(1, 0, 0, 0), 
					    Real4(0, 1, 0, 0), 
					    Real4(0, 0, 1/(1-k), k/(1-k)), 
					    Real4(0, 0,-1, 0));
	float sx, sy, sz, h, w;
	float hRad = verticalAngle*PI/180;
	h = tan(hRad/2);
	w = h*aspectRatio;
	sx = 1/(w*far);
	sy = 1/(h*far);
	sz = 1/far;

	view.append(Mpp);
	view.scale(sx, sy, sz);

	Camara::setView();
};

void CamaraPerspectiva::setVerticalAperture(float av){

	verticalAngle = av;
	ready = 0;
};

void CamaraPerspectiva::getParam(Punto &pos, Vector &direccion,Vector &vertical,
								 float &av,  float &ah, float &cca, float &ljs) const{

	pos = pov;
	direccion = look;
	vertical = up;
	av = verticalAngle;
	ah = verticalAngle*aspectRatio;
	cca = near;
	ljs = far;
};
