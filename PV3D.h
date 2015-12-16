#pragma once
#include <GL/glut.h>

class PV3D {
private:
	GLdouble _x, _y, _z;
	int _pv; // Si pv = 0 --> punto, si pv = 1 --> vector

public:
	PV3D() : _pv(1), _x(0), _y(0), _z(0) {}
	PV3D(GLdouble x, GLdouble y, GLdouble z, int pv);

	void normaliza();
	PV3D* clona();
	GLdouble productoEscalar(PV3D* v);
	PV3D* productoVectorial(PV3D* v);

	GLdouble getX() { return _x; }
	GLdouble getY() { return _y; }
	GLdouble getZ() { return _z; }

	void setX(GLdouble x) { _x = x; }
	void setY(GLdouble y) { _y = y; }
	void setZ(GLdouble z) { _z = z; }
};