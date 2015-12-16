#pragma once
#include "Malla.h"

class MontanaRusa : public Malla
{
private:
	int _np; //número de lados del polígono que aproxima la circunferencia del tubo
	int _nq; //número de rodajas que forman el tubo
	GLdouble M[16]; //Matriz de transformación
	void matrix(GLdouble t);
public:
	MontanaRusa(int np, int nq);
	~MontanaRusa();
	
};

