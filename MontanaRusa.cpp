#include "MontanaRusa.h"
#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
using namespace std;

MontanaRusa::MontanaRusa(int np, int nq)
{	
	_np = np;//número de lados del polígono que aproxima la circunferencia del tubo
	_nq = nq;//número de rodajas que forman el tubo
	GLdouble r = 1; //Radio de la circunferencia que aproxima el polígono

	PV3D** perfil = new PV3D*[_np]; //perfil original en el plano xy
	
	numVertices = _np * _nq;
	numCaras    = _np * _nq;
	numNormales = _np * _nq;

	vertice = new PV3D*[numVertices];
	normal  = new PV3D*[numNormales];
	cara    = new Cara*[numCaras];

	//Construimos el perfil original
	GLdouble inc = (2 * M_PI / _np);
	for (int i = 0; i < _np; i++) 
		perfil[i] = new PV3D(r*cos(2 * M_PI - i*inc), r*sin(2 * M_PI - i*inc), 0, 1);

	// Vertices de la malla
	GLdouble incremento = 4 * M_PI / _nq;
	GLdouble t = 0;
	for (int i = 0; i < _nq; i++) {
		t += incremento;
		matrix(t);
		for (int j = 0; j < _np; j++) {
			int indice = i * _np + j;

			PV3D *p = new PV3D(M[0] * perfil[j]->getX() +
							   M[4] * perfil[j]->getY() +
							   M[8] * perfil[j]->getZ() + M[12],
							   
							   M[1] * perfil[j]->getX() +
							   M[5] * perfil[j]->getY() +
							   M[9] * perfil[j]->getZ() + M[13],
							   
							   M[2] * perfil[j]->getX() +
							   M[6] * perfil[j]->getY() +
							   M[10] * perfil[j]->getZ() + M[14],

							   1);
			vertice[indice] = p;
//cout << p->getX() << " " << p->getY() << " " << p->getZ() << endl;
		}
//cout << endl;
	}

	//Construccion de las caras
	int indiceCara = 0;
	int indice = 0;
	for (int i = 0; i < _nq - 1; i++) {
		for (int j = 0; j < _np ; j++) {
			indice = i * _np + j;

			VerticeNormal** vn = new VerticeNormal*[4];

			if ((indice + 1) % _np == 0) {
// DEBUG
//cout << indice << " ";
//cout << indice + _np << " ";
//cout << indice + 1 << " ";
//cout << (indice + 1) - _np << endl;
//------				
				vn[0] = new VerticeNormal(indice, indiceCara);
				vn[1] = new VerticeNormal(indice + _np, indiceCara);
				vn[2] = new VerticeNormal(indice + 1, indiceCara);
				vn[3] = new VerticeNormal((indice + 1) - _np, indiceCara);
			} else {
// DEBUG
//cout << indice << " ";
//cout << indice + _np << " ";
//cout << indice + _np + 1 << " ";
//cout << indice + 1 << endl;
//------
				vn[0] = new VerticeNormal(indice, indiceCara);
				vn[1] = new VerticeNormal(indice + _np, indiceCara);
				vn[2] = new VerticeNormal(indice + _np + 1, indiceCara);
				vn[3] = new VerticeNormal(indice + 1, indiceCara);
			}
			cara[indiceCara] = new Cara(4, vn);
			
			PV3D* v = CalculoVectorNormalPorNewell(cara[indiceCara]);
			normal[indiceCara] = v;

			indiceCara++;
		}
//cout << endl;
	}

	// Union de las ultimas caras con las primeras
	for (int i = 0; i < _np; i++) {
		VerticeNormal** vn = new VerticeNormal*[4];
		vn[0] = new VerticeNormal(indice + i + 1, indiceCara);
		vn[1] = new VerticeNormal(i, indiceCara);

// DEBUG
//cout << indice + i + 1 << " ";
//cout << i << " ";
		if (i == (_np - 1)) {
// DEBUG
//cout << 0 << " ";
//cout << _np * (_nq - 1) << endl;
//------

			vn[2] = new VerticeNormal(0, indiceCara);
			vn[3] = new VerticeNormal(_np * (_nq - 1), indiceCara);
		} else {
//cout << i + 1 << " ";
//cout << indice + i + 2 << endl;

			vn[2] = new VerticeNormal(i + 1, indiceCara);
			vn[3] = new VerticeNormal(indice + 2 + i, indiceCara);
		}

		cara[indiceCara] = new Cara(4, vn);
		PV3D* vn11 = CalculoVectorNormalPorNewell(cara[indiceCara]);
		normal[indiceCara] = vn11;

		indiceCara++;
	}

}

void MontanaRusa::matrix(GLdouble t) {

	PV3D* C = new PV3D(3 * cos(t), 3 * sin(2 * t), 3 * sin(t), 1); //C(t)
	PV3D* Cprima = new PV3D(-3 * sin(t), 6 * cos(2 * t), 3 * cos(t), 1); //C'(t)
	PV3D* C2prima = new PV3D(-3 * cos(t), -12 * sin(2 * t), -3 * sin(t), 1); //C''(t)

	PV3D* B = Cprima->productoVectorial(C2prima); //B(t) sin normalizar
	B->normaliza();//B(t)

	Cprima->normaliza();//T(t) sin normalizar
	PV3D* T = Cprima;//T(t)

	PV3D* N = B->productoVectorial(T);//N(t)

	//Creamos la matriz de transformación
	
	M[0] = N->getX();
	M[1] = N->getY();
	M[2] = N->getZ();
	M[3] = 0.0;
	M[4] = B->getX();
	M[5] = B->getY();
	M[6] = B->getZ();
	M[7] = 0.0;
	M[8] = T->getX();
	M[9] = T->getY();
	M[10] = T->getZ();
	M[11] = 0.0;
	M[12] = C->getX();
	M[13] = C->getY();
	M[14] = C->getZ();
	M[15] = 1.0;
}

MontanaRusa::~MontanaRusa()
{
}
