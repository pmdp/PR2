#include "Malla.h"
#include <iostream>
using namespace std;

void Malla::dibuja(bool normales, bool lines) {
	int paintMode;
	if (lines)
		paintMode = GL_LINE_LOOP;
	else
		paintMode = GL_POLYGON;
	
	GLdouble pMX = 0, pMY = 0, pMZ = 0;
	PV3D* vDirector;

	for (int i = 0; i < numCaras; i++) {	
		glLineWidth(1.0);
		glBegin(paintMode);
		for (int j = 0; j < cara[i]->getNumVertices(); j++)	{
			int iN = cara[i]->getIndiceNormal(j);
			int	iV = cara[i]->getIndiceVertice(j);

			glNormal3f(normal[iN]->getX(), normal[iN]->getY(), normal[iN]->getZ());
			glVertex3f(vertice[iV]->getX(), vertice[iV]->getY(), vertice[iV]->getZ());
			
			if (normales) {
				//Va sumando las componentes de cada vértice de la cara
				pMX += vertice[iV]->getX();
				pMY += vertice[iV]->getY();
				pMZ += vertice[iV]->getZ();
				//Guarda el vector normal a la cara
				vDirector = normal[iN];// new PV3D(normal[iN]->getX(), normal[iN]->getY(), normal[iN]->getZ(), 1);
			}

		}
		glEnd();
		if (normales) {
			//Divide cada suma de componentes entre el número de vértices de la cara
			int numV = cara[i]->getNumVertices();
			pMX = pMX / numV;
			pMY = pMY / numV;
			pMZ = pMZ / numV;
			
			//Se guarda el color anterior en el array currentColor, para luego volver a ponerlo
			GLfloat currentColor[4];
			glGetFloatv(GL_CURRENT_COLOR, currentColor);


			//Se comienza a pintar las normales de la cara i-ésima
			glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			glVertex3f(pMX,pMY,pMZ);
			glVertex3f(pMX + vDirector->getX(), pMY + vDirector->getY(), pMZ + vDirector->getZ());
			glEnd();

			//Se vuelve a poner el color anterior a pintar las normales
			glColor3f(currentColor[0],currentColor[1],currentColor[2]);

			pMX = 0;
			pMY = 0;
			pMZ = 0;
		}
	}
}

PV3D* Malla::CalculoVectorNormalPorNewell(Cara *c) {
	
	PV3D *vertActual	= new PV3D(0.0f, 0.0f, 0.0f, 1);
	PV3D *vertSiguiente = new PV3D(0.0f, 0.0f, 0.0f, 1);

	GLdouble x = 0, y = 0, z = 0;

	for (int i = 0; i < c->getNumVertices(); i++)
	{
		vertActual = vertice[c->getIndiceVertice(i)];
		//int asd = c->getIndiceVertice(i + 1);
		//int b = c->getNumVertices();
		//int d = a % b;
		int index = (c->getIndiceVertice((i + 1) % c->getNumVertices()));

		vertSiguiente = vertice[index];
		x += (vertActual->getY() - vertSiguiente->getY()) * (vertActual->getZ() + vertSiguiente->getZ());
		y += (vertActual->getZ() - vertSiguiente->getZ()) * (vertActual->getX() + vertSiguiente->getX());
		z += (vertActual->getX() - vertSiguiente->getX()) * (vertActual->getY() + vertSiguiente->getY());		
	}
	PV3D* n = new PV3D(x, y, z, 1);

	//delete vertActual;
	//delete vertSiguiente;

	n->normaliza();
	return n;
}