#include<iostream>
#include<cmath>
#include<gl\glut.h>
#include<gl\GLU.h>
#include<gl\GL.h>
using namespace std;

GLfloat tj(GLfloat ti, GLfloat pi,GLfloat pj,GLfloat alpha) {
	GLfloat xi, yi;
	GLfloat xj, yj;
	xi = yi = pi;
	xj = yj = pj;
	return (GLfloat)pow(sqrt(pow(xj-xi,2)+ pow(yj - yi, 2)),alpha)+ti;
}

int* CatmullRomSpline(GLfloat p0, GLfloat p1, GLfloat p2, GLfloat p3,int precision=100) {
	float alpha = 0.5;
	GLfloat t0, t1, t2, t3;
	t0 = 0;
	t1 = tj(t0,p0,p1,alpha);
	t2 = tj(t1, p1, p2, alpha);
	t3 = tj(t2, p2, p3, alpha);
	
}


void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0);
	glOrtho(0, 500, 0, 500, -100, 100.0);
	glViewport(0, 0, 500, 500);
	glTranslatef(250, 250, 0);
	glutSolidSphere(5, 30, 30);
	glutSwapBuffers();
}

int main(int argc,char** argv) {
	
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("it is a demonstration of Catmull-Rom Spline");
	glClearColor(0.0, 0.0, 0.0, 0);
	glutDisplayFunc(render);
	glutMainLoop();
}
