#include "Ficha.h"
#include <iostream>
using namespace std;

Ficha::Ficha(int numQuesitos) : _numQuesitos(numQuesitos) {
	quesito = new Quesito(15,7,5);	
}

void Ficha::dibuja(bool normales, bool lines) {

	for (int i = 0; i <= _numQuesitos; i++) {
		if      (i == 0) glColor3f(1.f, 0.f, 0.f);
		else if (i == 1) glColor3f(0.f, 1.f, 0.f);
		else if (i == 2) glColor3f(0.f, 0.f, 1.f);
		else if (i == 3) glColor3f(1.f, 1.f, 0.f);
		else if (i == 4) glColor3f(1.f, 0.f, 1.f);
		else if (i == 5) glColor3f(0.f, 1.f, 1.f);
		
		glPushMatrix();
		glRotatef(60 * i, 0.f, 1.f, 0.f);
		quesito->dibuja(normales, lines);
		glPopMatrix();
	}
}

Ficha::~Ficha()
{
}
