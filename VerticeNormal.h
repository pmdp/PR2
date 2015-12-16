#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
#include <GL/glut.h>

class VerticeNormal {
private:
	int _indiceVertice, _indiceNormal;
public:
	VerticeNormal(int indiceVertice, int indiceNormal) : _indiceVertice(indiceVertice), _indiceNormal(indiceNormal)  {}
	int getIndiceNormal()  { return _indiceNormal; }
	int getIndiceVertice() { return _indiceVertice; }
};