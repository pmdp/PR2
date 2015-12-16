#include "Camara.h"
#include <iostream>
using namespace std;

Camara::Camara(PV3D* eye, PV3D* look, PV3D* up, GLdouble r, GLdouble l, GLdouble t, GLdouble b)
: _eye(eye), _look(look), _up(up), _right(r), _left(l), _top(t), _botton(b), _near(1), _far(1000)
{	
	_fovy = 10;
	_aspect = 1;
	setLookAt();

	ortogonal();
	//perspectiva();

	setUVN();
	
}
void Camara::setUVN(){
	//Vector n de la cámara
	_n = new PV3D(_eye->getX() - _look->getX(),
		_eye->getY() - _look->getY(),
		_eye->getZ() - _look->getZ(), 1);
	_n->normaliza();

	//Vector u de la cámara
	_u = _up->productoVectorial(_n);
	_u->normaliza();

	//Vector v de la cámara
	_v = _n->productoVectorial(_u);
}

void Camara::actualizaCamara(){
	//Vuelve a calcular los valores de la cámara en funcion de u,v y n
	GLdouble eyex = (_u->getX()*_eye->getX()) + (_v->getX()*_eye->getY()) + (_n->getX()*_eye->getZ());
	GLdouble eyey = (_u->getY()*_eye->getX()) + (_v->getY()*_eye->getY()) + (_n->getY()*_eye->getZ());
	GLdouble eyez = (_u->getZ()*_eye->getX()) + (_v->getZ()*_eye->getY()) + (_n->getZ()*_eye->getZ());
	_eye->setX(eyex);
	_eye->setY(eyey);
	_eye->setZ(eyez);
	GLdouble lookx = (_u->getX()*_look->getX()) + (_v->getX()*_look->getY()) + (_n->getX()*_look->getZ()) + _eye->getX();
	GLdouble looky = (_u->getY()*_look->getX()) + (_v->getY()*_look->getY()) + (_n->getY()*_look->getZ()) + _eye->getY();
	GLdouble lookz = (_u->getZ()*_look->getX()) + (_v->getZ()*_look->getY()) + (_n->getZ()*_look->getZ()) + _eye->getZ();
	_look->setX(lookx);
	_look->setY(looky);
	_look->setZ(lookz);
	GLdouble upx = (_u->getX()*_up->getX()) + (_v->getX()*_up->getY()) + (_n->getX()*_up->getZ()) + _eye->getX();
	GLdouble upy = (_u->getY()*_up->getX()) + (_v->getY()*_up->getY()) + (_n->getY()*_up->getZ()) + _eye->getY();
	GLdouble upz = (_u->getZ()*_up->getX()) + (_v->getZ()*_up->getY()) + (_n->getZ()*_up->getZ()) + _eye->getZ();
	_up->setX(upx);
	_up->setY(upy);
	_up->setZ(upz);
}
void Camara::perspectiva(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(_left, _right, _botton, _top, _near, _far);
	gluPerspective(_fovy, _aspect, _near, _far);
}
void Camara::ortogonal(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(_left, _right, _botton, _top, _near, _far);
}

Camara::~Camara()
{
	
}
void Camara::setLookAt(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(_eye->getX(), _eye->getY(), _eye->getZ(),
		_look->getX(), _look->getY(), _look->getZ(),
		_up->getX(), _up->getY(), _up->getZ());
}
void Camara::recorridoEje(GLdouble x, GLdouble y, GLdouble z){
	_eye->setX(_eye->getX() + x);
	_eye->setY(_eye->getY() + y);
	_eye->setZ(_eye->getZ() + z);
	setLookAt();
}
void Camara::giraX(){
	_tX += 10;
	_eye->setY(_r*cos(_tX*(M_PI/180)));
	_eye->setZ(_r*sin(_tX*(M_PI/180)));
	setLookAt();
}
void Camara::giraY(){
	_tY += 10;
	_eye->setX(_r*cos(_tY*(M_PI / 180)));
	_eye->setZ(_r*sin(_tY*(M_PI / 180)));
	setLookAt();
}
void Camara::giraZ(){
	_tZ += 10;
	_eye->setX(_r*cos(_tZ*(M_PI / 180)));
	_eye->setY(_r*sin(_tZ*(M_PI / 180)));
	setLookAt();
}
void Camara::setModelViewMatrix() {
	
	PV3D* e_v = new PV3D(-(_eye->getX()), -(_eye->getY()), -(_eye->getZ()), 0);
	GLdouble V[16];
	// Calcula la matriz de vista
	V[0] = _u->getX(); V[4] = _u->getY(); V[8] = _u->getZ();  V[12] = e_v->productoEscalar(_u);
	V[1] = _v->getX(); V[5] = _v->getY(); V[9] = _v->getZ();  V[13] = e_v->productoEscalar(_v);
	V[2] = _n->getX(); V[6] = _n->getY(); V[10] = _n->getZ(); V[14] = e_v->productoEscalar(_n);
	V[3] = 0;		   V[7] = 0; 		  V[11] = 0; 		  V[15] = 1.0;

	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glLoadMatrixd(V);
}
// Cambiar matriz de vista, gluLookAt hace lo mismo pero no actualiza los valores
// Cada vez que se haga glulookat hay q actualizar u, v y n
// n = look - eye -> normalizados
// roll no actualizan eye, mira y up
void Camara::roll(GLdouble angulo){ //Gira respecto a n
	PV3D *u = _u->clona();
	GLdouble cs = cos(angulo / 180 * M_PI);
	GLdouble sn = sin(angulo / 180 * M_PI);

	// El nuevo eje _u representado con coordenadas globales
	_u->setX(cs * u->getX() + sn * _v->getX());
	_u->setY(cs * u->getY() + sn * _v->getY());
	_u->setZ(cs * u->getZ() + sn * _v->getZ());

	// El nuevo eje _v representado con coordenadas globales
	_v->setX(-sn * u->getX() + cs * _v->getX());
	_v->setY(-sn * u->getY() + cs * _v->getY());
	_v->setZ(-sn * u->getZ() + cs * _v->getZ());

	setModelViewMatrix();
	
}

void Camara::yaw(GLdouble angulo){ //Gira respecto a v
	PV3D *u = _u->clona();
	GLdouble cs = cos(angulo / 180 * M_PI);
	GLdouble sn = sin(angulo / 180 * M_PI);

	// El nuevo eje _u representado con coordenadas globales
	_u->setX(cs * u->getX() - sn * _n->getX());
	_u->setY(cs * u->getY() - sn * _n->getY());
	_u->setZ(cs * u->getZ() - sn * _n->getZ());

	// El nuevo eje _n representado con coordenadas globales
	_n->setX(sn * u->getX() + cs * _n->getX());
	_n->setY(sn * u->getY() + cs * _n->getY());
	_n->setZ(sn * u->getZ() + cs * _n->getZ());

	setModelViewMatrix();
}
void Camara::pitch(GLdouble angulo){ //Gira respecto a u
	PV3D *v = _v->clona();
	GLdouble cs = cos(angulo / 180 * M_PI);
	GLdouble sn = sin(angulo / 180 * M_PI);

	// El nuevo eje _v representado con coordenadas globales
	_v->setX(cs * _v->getX() + sn * _n->getX());
	_v->setY(cs * _v->getY() + sn * _n->getY());
	_v->setZ(cs * _v->getZ() + sn * _n->getZ());

	// El nuevo eje _n representado con coordenadas globales
	_n->setX(-sn * v->getX() + cs * _n->getX());
	_n->setY(-sn * v->getY() + cs * _n->getY());
	_n->setZ(-sn * v->getZ() + cs * _n->getZ());

	setModelViewMatrix();

}
void Camara::lateral(){
	_eye = new PV3D(100, 0, 0, 0);
	_look = new PV3D(0, 0, 0, 0);
	_up = new PV3D(0, 1, 0, 1);

	setLookAt();
	setUVN();
}
void Camara::frontal(){
	_eye = new PV3D(100, 100, 100, 0);
	_look = new PV3D(0, 0, 0, 0);
	_up = new PV3D(0, 1, 0, 1);

	setLookAt();
	setUVN();
}
void Camara::cenital(){
	_eye = new PV3D(0, 100, 0, 0);
	_look = new PV3D(0, 0, 0, 0);
	_up = new PV3D(1, 0, 0, 0);

	setLookAt();
	setUVN();
}
void Camara::esquina(){
	_eye = new PV3D(0, 0, 100, 0);
	_look = new PV3D(0, 0, 0, 0);
	_up = new PV3D(0, 1, 0, 1);

	setLookAt();
	setUVN();
}
