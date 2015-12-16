#pragma once
#include "Malla.h"


class Quesito : public Malla
{
private:
	int _nq; //Nº de veces q se divide la parte circular
	int _radio;  //Radio
	int _altura;  //Altura
public:
	Quesito(int nq, int z, int x);
	~Quesito();
};

