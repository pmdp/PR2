#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
#include <GL/glut.h>
#include "PV3D.h"
#define _USE_MATH_DEFINES
#include <cmath>
class Camara
{
private:
	//Valores de la cámara
	PV3D *_eye, *_look, *_up, *_u, *_v, *_n; 
	//Valores del volumen de vista
	GLdouble _right, _left, _top, _botton, _near, _far, _fovy, _aspect;
	GLdouble _r = 10; //Radio de la circunferencia al girar la cámara sobre un eje
	GLdouble _tX = 0, _tY = 0, _tZ = 0; //Incrementos de la posición en la circunferencia

	
public:
	Camara(PV3D* eye, PV3D* look, PV3D* up, GLdouble r, GLdouble l, GLdouble t, GLdouble b);
	~Camara();
	void setLookAt();
	void setUVN();
	void actualizaCamara();
	void recorridoEje(GLdouble x, GLdouble y, GLdouble z);
	void giraX();
	void giraY();
	void giraZ();
	void setModelViewMatrix();
	void roll(GLdouble angulo);
	void yaw(GLdouble angulo);
	void pitch(GLdouble angulo);
	void lateral();
	void frontal();
	void cenital();
	void esquina();
	void perspectiva();
	void ortogonal();
};

