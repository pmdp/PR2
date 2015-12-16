#include "PV3D.h"
#include <cmath>

PV3D::PV3D(GLdouble x, GLdouble y, GLdouble z, int pv) {
	_x = x;
	_y = y;
	_z = z;

	_pv = pv;
}

void PV3D::normaliza() {
	GLdouble modulo = sqrt(_x * _x + _y * _y + _z * _z);
	_x = _x / modulo;
	_y = _y / modulo;
	_z = _z / modulo;
}

PV3D* PV3D::clona() {
	PV3D *punto = new PV3D(); 

	punto->_x = _x;
	punto->_y = _y;
	punto->_z = _z;
	punto->_pv = _pv;

	return punto;
}
GLdouble PV3D::productoEscalar(PV3D* v) {
	GLdouble escalar = _x * v->_x + _y * v->_y + _z * v->_z;
	return escalar;
}
PV3D* PV3D::productoVectorial(PV3D* v) {
	GLdouble x, y, z;
	
	x = _y*v->_z - _z*v->_y;
	y = _z*v->_x - _x*v->_z;
	z = _x*v->_y - _y*v->_x;

	return new PV3D(x, y, z, 1);
}
