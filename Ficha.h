#pragma once
#include "Quesito.h"

class Ficha
{
	Quesito* quesito;
	int _numQuesitos;
public:
	Ficha(int numQuesitos);
	~Ficha();

	void dibuja(bool normales, bool lines);
};

