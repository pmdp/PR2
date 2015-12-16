#include "Quesito.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
using namespace std;

Quesito::Quesito(int nq, int radio, int altura) :
_nq(nq), _radio(radio), _altura(altura)
{
	int m = 3;//numero de puntos en el perfil original

	PV3D** perfil = new PV3D*[m];//perfil original en el plano xy
	perfil[0] = new	PV3D(_radio, 0, 0, 1);
	perfil[1] = new	PV3D(_radio, _altura, 0, 1);
	perfil[2] = new	PV3D(0, _altura, 0, 1);
	
	int n =_nq; //numero de rotaciones

	//Tamaños de los arrays
	numVertices = n*m +1;// _nq * 2 + 2; // (n * m) + 1;//3 * _nq + 1; // m + (2 * n); // vertices de la cara original + 2 vÃ©rtice por rotaciÃ³n
	numCaras = 2*(n - 1) +2; 
	numNormales = numCaras; //1 normal por cara

	//Creacion de los arrays
	vertice = new PV3D*[numVertices];
	normal  = new PV3D*[numNormales];
	cara    = new Cara*[numCaras];

	//Colocar el perfil original en la tabla de vertices
	for (int j = 0; j < m; j++)
		vertice[j] = perfil[j]->clona();

	//Vertices de la malla
	for (int i = 1; i < n; i++) {//generar el perfil i-esimo
		GLdouble theta = i * 59 / (GLdouble)n;
		GLdouble radians = (theta * M_PI) / 180;
		GLdouble c = cos(radians);
		GLdouble s = sin(radians);
		//R_y es la matriz de rotacion sobre el eje Y
		for (int j = 0; j < m; j++) {
			int indice = i*m + j;
			//Transformar el punto j-isimo del perfil original
			GLdouble x = c*perfil[j]->getX() + s*perfil[j]->getZ();
			GLdouble z = -s*perfil[j]->getX() + c*perfil[j]->getZ();
			PV3D* p = new PV3D(x, perfil[j]->getY(), z, 1);
			vertice[indice] = p;
			//cout << indice << endl;
		}
	}

	//Se mete alfinal de la tabla de vértices el original
 	vertice[numVertices-1] = new PV3D(0, 0, 0, 1);

	//Construccion de las caras
	int indiceCara = 0;
	for (int i = 0; i < n; i++) { //Unir el perfil i-isimo con el (i+1)%n-isimo
		for (int j = 0; j < m - 1; j++) {//esquina inferior-izquierda de una cara
			//indiceCara = i*(m-1)+j;
			int indice = i*m + j;
			VerticeNormal** vn = new VerticeNormal*[4];
			vn[0] = new VerticeNormal(indice, indiceCara);
			vn[1] = new VerticeNormal((indice + m) % numVertices, indiceCara);
			vn[2] = new VerticeNormal((indice + 1 + m) % numVertices, indiceCara);
			vn[3] = new VerticeNormal(indice + 1, indiceCara);
			cara[indiceCara] = new Cara(4, vn);
			PV3D* v = CalculoVectorNormalPorNewell(cara[indiceCara]);//Newell
			normal[indiceCara] = v;
			//cout << "Cara " << indiceCara << " : " << indice << " " << (indice + m) % numVertices << " " << (indice + 1 + m) % numVertices << " " << indice + 1 << endl;
			indiceCara++;
		}
	}
	indiceCara -= 2;
	//Tapa lateral delantera
	VerticeNormal** vn1 = new VerticeNormal*[4];
	vn1[0] = new VerticeNormal(numVertices-1, indiceCara);
	vn1[1] = new VerticeNormal(0, indiceCara);
	vn1[2] = new VerticeNormal(1, indiceCara);
	vn1[3] = new VerticeNormal(2, indiceCara);
	cara[indiceCara] = new Cara(4, vn1);
	PV3D* vn11 = CalculoVectorNormalPorNewell(cara[indiceCara]);
	normal[indiceCara] = vn11;
	//Tapa lateral trasera
	indiceCara++;
	VerticeNormal** vn2 = new VerticeNormal*[4];
	vn2[0] = new VerticeNormal(numVertices-4, indiceCara);
	vn2[1] = new VerticeNormal(numVertices - 1, indiceCara);
	vn2[2] = new VerticeNormal(numVertices - 2, indiceCara);
	vn2[3] = new VerticeNormal(numVertices - 3, indiceCara);
	cara[indiceCara] = new Cara(4, vn2);
	PV3D* vn22 = CalculoVectorNormalPorNewell(cara[indiceCara]);
	normal[indiceCara] = vn22;
}

Quesito::~Quesito() {
//for (int i = 0; i < numVertices; i++)
//delete vertice[i];
//delete[] vertice;
//
//for (int i = 0; i < numNormales; i++)
//delete normal[i];
//delete[] normal;
//
//for (int i = 0; i < numCaras; i++)
//delete cara[i];
//delete[] cara;
}