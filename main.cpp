#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
//#include <GL/glut.h>

#include "Ficha.h"
#include "MontanaRusa.h"
#include "Camara.h"
#include <iostream>
using namespace std;

// Freeglut parameters
// Flag telling us to keep processing events
// bool continue_in_main_loop= true; //(**)

// Viewport size
int WIDTH= 500, HEIGHT= 500;

//Clases y variables
Ficha *ficha;
int numQuesitos = 6;
bool normales=false, lines=false, quesMont=true, baldosas=false, rotar=false;
MontanaRusa *montanaRusa;
GLdouble alphaX=0, alphaY=0, alphaZ=0;
Camara* cam;

// Viewing frustum parameters
GLdouble xRight=10, xLeft=-xRight, yTop=10, yBot=-yTop, N=1, F=1000;

// Camera parameters
GLdouble eyeX=100.0, eyeY=100.0, eyeZ=100.0;
GLdouble lookX=0.0, lookY=0.0, lookZ=0.0;
GLdouble upX=0, upY=1, upZ=0;

//Prototipos
void zoom();
void display();
void drawScene();
void drawAxes();
void rotacion(GLdouble alpha, GLdouble x, GLdouble y, GLdouble z);

void drawAxes() {
	// Drawing axes
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(20, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 20, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 20);
	glEnd();
}

void buildSceneObjects()  {
	//Objetos de la escena
	ficha = new Ficha(numQuesitos);
	montanaRusa = new MontanaRusa(20, 200);
	
	//Cámara
	PV3D* eye = new PV3D(eyeX, eyeY, eyeZ, 1);
	PV3D* look = new PV3D(lookX, lookY, lookZ, 0);
	PV3D* up = new PV3D(upX, upY, upZ, 0);
	cam = new Camara(eye, look, up, xRight, xLeft, yTop, yBot);
}
void traslacionX(GLdouble incr) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	xLeft  += incr;
	xRight += incr;

	glOrtho(xLeft, xRight, yBot, yTop, N, F);
}

void embaldosar(int nCol){	
	GLdouble SVAratio = (xRight - xLeft) / (yTop - yBot);
	GLdouble w = (GLdouble)WIDTH / (GLdouble)nCol;
	GLdouble h = w / SVAratio;
	int i = 0;
	for (GLint c = 0; c<nCol; c++){
		GLdouble currentH = 0;
		while ((currentH + h) <= HEIGHT){
			if (i == 0)
				cam->lateral();
			else if (i == 1)
				cam->frontal();
			else if (i == 2)
				cam->cenital();
			else
				cam->esquina();
			i++;
			glViewport((GLint)(c*w), (GLint)currentH, (GLint)w, (GLint)h);						
			drawScene();
			currentH += h;
			
		}
	}
}
void desembaldosar() {
	glViewport(0, 0, WIDTH, HEIGHT);
	cam->frontal();
}

void zoom (GLdouble f) {
	GLdouble anchoNew = (xRight - xLeft) / f;
	GLdouble altoNew = (yTop - yBot) / f;
	GLdouble centroX = (xLeft + xRight) / 2.0;
	GLdouble centroY = (yTop + yBot) / 2.0;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(centroX-anchoNew/2.0, centroX+anchoNew/2.0, centroY-altoNew/2.0, centroY+altoNew/2.0, N, F);
	display();
	xLeft = centroX - anchoNew / 2.0;
	xRight = centroX + anchoNew / 2.0;
	yBot = centroY - altoNew / 2.0;
	yTop = centroY + altoNew / 2.0;
}
void initGL() {	 		 
	glClearColor(0.6f,0.7f,0.8f,1.0);
    glEnable(GL_LIGHTING);    

	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);


	// Light0
	glEnable(GL_LIGHT0);
	GLfloat d[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
	GLfloat a[] = { 0.3f, 0.3f, 0.3f, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, a);
	GLfloat p[] = { 25.0, 25.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, p);


	buildSceneObjects();

	//// Camera set up
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

	// Frustum set up
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();     
	//glOrtho(xLeft, xRight, yBot, yTop, N, F);

	// Viewport set up
	//glViewport(0, 0, WIDTH, HEIGHT);  

	
	//Setup cámera matrix view and projection
	/*cam->setLookAt();
	cam->ortogonal();
	cam->setUVN();*/
 }
void drawScene() {
	glMatrixMode(GL_MODELVIEW);

	drawAxes();

	glPushMatrix();

	glRotated(alphaX, 1, 0, 0);
	glRotated(alphaY, 0, 1, 0);
	glRotated(alphaZ, 0, 0, 1);

	if (quesMont)
		montanaRusa->dibuja(normales, lines);
	else
		ficha->dibuja(normales, lines);
	glPopMatrix();
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (baldosas)
		embaldosar(2);
	else 
		drawScene();
	
	glFlush();
	glutSwapBuffers();
	
}

void resize(int newWidth, int newHeight) {
	WIDTH= newWidth;
	HEIGHT= newHeight;
	GLdouble RatioViewPort= (float)WIDTH/(float)HEIGHT;
	glViewport (0, 0, WIDTH, HEIGHT) ;
   
	GLdouble SVAWidth= xRight-xLeft;
	GLdouble SVAHeight= yTop-yBot;
	GLdouble SVARatio= SVAWidth/SVAHeight;
	if (SVARatio >= RatioViewPort) {		 
		GLdouble newHeight= SVAWidth/RatioViewPort;
		GLdouble yMiddle= ( yBot+yTop )/2.0;
		yTop= yMiddle + newHeight/2.0;
		yBot= yMiddle - newHeight/2.0;
    }
	else {      
		GLdouble newWidth= SVAHeight*RatioViewPort;
		GLdouble xMiddle= ( xLeft+xRight )/2.0;
		xRight= xMiddle + newWidth/2.0;
		xLeft=  xMiddle - newWidth/2.0;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();   
	glOrtho(xLeft, xRight, yBot, yTop, N, F);
}
void special_key(int key, int, int y) {
	bool need_redisplay = true;
	switch (key) {
	case 101://up arrow
		glRotatef(-20, 1, 0, 0);
		break;
	case 103://down arrow
		glRotatef(20, 1, 0, 0);
		break;
	case 102://right arrow
		glRotatef(20, 0, 1, 0);
		break;
	case 100://left arrow
		glRotatef(-20, 0, 1, 0);
		break;
	default:
		need_redisplay = false;
		break;
	}
	if (need_redisplay)
		glutPostRedisplay();
}
void key(unsigned char key, int x, int y){
	bool need_redisplay = true;
	switch (key) {

		case 27:  /* Escape key */
			//continue_in_main_loop = false; // (**)
			//Freeglut's sentence for stopping glut's main loop (*)
			glutLeaveMainLoop (); 
			break;	
		case 97://a
			alphaX += 20;
			break;
		case 122://z
			alphaX -= 20;
			break;
		case 115://s
			alphaY += 20;
			break;
		case 120://x
			alphaY -= 20;
			break;
		case 100://d
			alphaZ += 20;	
			break;
		case 99://c
			alphaZ -= 20;	
			break;
		case 101://e
			cam->recorridoEje(-7, 0, 0);
			break;
		case 114://r
			cam->recorridoEje(7, 0, 0);
			break;
		case 102://f
			cam->recorridoEje(0, -7, 0);
			break;
		case 103://g
			cam->recorridoEje(0, 7, 0);
			break;
		case 118://v
			cam->recorridoEje(0, 0, -7);
			break;
		case 98://b
			cam->recorridoEje(0, 0, 7);
			break;
		case 119://w
			traslacionX(-2);
			break;
		case 113://q
			traslacionX(2);
			break;
		case 49://1
			cam->giraX();
			break;
		case 50://2
			cam->giraY();
			break;
		case 51://3
			cam->giraZ();
			break;
		case 52://4
			cam->lateral();
			break;
		case 53://5
			cam->frontal();
			break;
		case 54://6
			cam->cenital();
			break;
		case 55://7
			cam->esquina();
			break;
		case 56://8
			cam->perspectiva();
			break;
		case 57://9
			cam->ortogonal();
			break;
		case 241://ñ
			if (normales)
				normales = false;
			else
				normales = true;
			break;
		case 108://l
			if (lines)
				lines = false;
			else
				lines = true;
			break;
		case 32://espacio
			if (quesMont)
				quesMont = false;
			else
				quesMont = true;
			break;
		case 116://t
			zoom(1.25);
			break;
		case 121://y
			zoom(0.75);
			break;
		case 111://o
			baldosas = true;
			break;
		case 112://p
			baldosas = false;
			desembaldosar();
			break;
		case 117://u
			cam->roll(2.0);
			break;
		case 105://i
			cam->roll(-2.0);
			break;
		case 106://j
			cam->yaw(2.0);
			break;
		case 107://k
			cam->yaw(-2.0);
			break;
		case 109://m
			cam->pitch(-2.0);
			break;
		case 110://n
			cam->pitch(2.0);
			break;
		default:
			need_redisplay = false;
			break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}
int main(int argc, char *argv[]){
	cout<< "Starting console..." << endl;

	int my_window; // my window's identifier

	// Initialization
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition (140, 140);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	// Window construction
	my_window = glutCreateWindow("Freeglut 3D-project");
    
	// Callback registration
	glutReshapeFunc(resize);
	glutKeyboardFunc(key);
	glutSpecialFunc(special_key);
	glutDisplayFunc(display);

	// OpenGL basic setting
	initGL();

	// Freeglut's main loop can be stopped executing (**)
	// while (continue_in_main_loop) glutMainLoopEvent();

	// Classic glut's main loop can be stopped after X-closing the window,
	// using the following freeglut's setting (*)
	glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION) ;
    
	// Classic glut's main loop can be stopped in freeglut using (*)
	glutMainLoop(); 
  
	// We would never reach this point using classic glut
	system("PAUSE"); 
   

	return 0;
}


