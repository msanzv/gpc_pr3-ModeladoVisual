/* DEFINICIÓN DE CAMARAS SINTÉTICAS
R.Vivó, J.Lluch para GPC.etsinf.upv.es 2011 */

#ifndef __CAMARA
#define __CAMARA

#include "Algebra.h"

// Clase Camara +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class Camara{

protected:
	Punto pov;						// Posicion de la camara (def: 0,0,0)
	Vector look;					// Orientacion de la camara (def: 0,0,-1)
	Vector up;						// Arriba en la camara (def: 0,1,0)
	float aspectRatio;				// Razon de aspecto (def: 4/3)
	float near;						// Distancia al plano frontal (def: 1)
	float far;						// Distancia al plano trasero (def: 10)

	Transformacion view;			// Matriz de transformación al sistema de la vista
	int ready;						// Indica si la camara esta lista para disparar

	virtual void setView();			// Actualiza la vista	

public:

	Camara();								// Constructor por defecto
	void at(Punto pos);						// Posiciona la camara 
	void lookAt(Punto poi);					// Orienta la camara mirando hacia el punto de interés poi
	void lookTo(Vector to);					// Orienta la camara mirando en esa direccion
	void setVertical(Vector v);				// Indica el vector up
	void setAspectRatio(float ratio);		// Cambia las proporciones de la foto
	void setFOV(float neardistance, float fardistance);// Indica los limites del campo visual
	Punto shot(Punto p);					// Transforma un punto al sistema de referencia propio
	Transformacion getview();				// Devuelve la matriz de la vista

};

// Clase Camara Ortografica +++++++++++++++++++++++++++++++++++++++++++++++++++++++

class CamaraOrtografica: public Camara
{
protected:

	float height;							// Altura de la foto (def: 2)

	void setView();							// Calculo de la transformacion de la vista

public:

	CamaraOrtografica();					// Constructor por defecto
	void setHeight(float h);				// Cambia la altura del cuadro que saldra en la foto (zoom)
	void getParam(Punto &posicion, Vector &hacia, Vector &vertical,
				  float &alt, float &anch, float &cca, float &ljs)const;

};

// Clase Camara Perspectiva +++++++++++++++++++++++++++++++++++++++++++++++++++++++

class CamaraPerspectiva: public Camara
{
protected:

	float verticalAngle;					// Apertura vertical del objetivo

	void setView();							// Calcula de la transformacion de la vista 
											// (incluida transformacion perspectiva)
public:

	CamaraPerspectiva();					// Constructor por defecto
	void setVerticalAperture(float av);		// Cambia la apertura vertical del objetivo
	void getParam(Punto &posicion, Vector &hacia, Vector &vertical, 
				  float &angV, float &angH, float &cerca, float &lejos)const;

};


#endif
