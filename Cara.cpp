#include "Cara.h"

Cara::~Cara()  {
	for (int i = 0; i < _numVertices; i++)
		delete arrayVN[i];
	delete[] arrayVN;
}