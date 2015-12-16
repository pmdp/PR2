#pragma once
#include "Malla.h"

class MontanaRusa : public Malla
{
private:
	int _np; //n�mero de lados del pol�gono que aproxima la circunferencia del tubo
	int _nq; //n�mero de rodajas que forman el tubo
	GLdouble M[16]; //Matriz de transformaci�n
	void matrix(GLdouble t);
public:
	MontanaRusa(int np, int nq);
	~MontanaRusa();
	
};

