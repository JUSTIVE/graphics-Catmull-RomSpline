#include<iostream>
#include<cmath>
#include<gl\glut.h>
#include<gl\GLU.h>
#include<gl\GL.h>
using namespace std;

//struct for point in two dimensional space
typedef struct Point {
	GLfloat x;
	GLfloat y;
	Point() {}
	Point(GLfloat x, GLfloat y) {
		this->x = x;
		this->y = y;
	}
	Point& operator /(Point& x) {
		Point p = Point(this->x / x.x, this->y/y);
		return p;
	}
	Point& operator *(Point& x) {
		this->x *= x.x;
		this->y *= x.y;
		return *this;
	}
	Point& operator +(Point& x) {
		this->x += x.x;
		this->y += x.y;
		return *this;
	}
	Point& operator *(GLfloat x) {
		Point p = Point(this->x * x, this->y * y);
		return p;
	}
}Point;

float GetT(GLfloat ti, Point pi,Point pj,GLfloat alpha) {
	GLfloat xi, yi;
	GLfloat xj, yj;
	xi = pi.x;
	yi = pi.y;
	xj = pj.x;
	yj = pj.y;
	return ti+pow(sqrt(pow(xj-xi,2)+ pow(yj - yi, 2)),alpha);
}

Point* CatmullRomSpline(Point p0, Point p1, Point p2, Point p3,int precision=100) {
	Point* InterpolatedValue = new Point[precision];
	int index = 0;
	float alpha = 0.5;
	GLfloat t0, t1, t2, t3;
	t0 = 0.0f;
	t1 = GetT(t0,p0,p1,alpha);
	t2 = GetT(t1, p1, p2, alpha);
	t3 = GetT(t2, p2, p3, alpha);
	for (GLfloat t = t1; t < t2; t += (t2 - t1) / precision) {
		Point A1 = p0*((t1 - t) / t1 - t0) + p1*((t - t0) / (t1 - t0));
		Point A2 = p1*((t2 - t) / (t2 - t1)) + p2*((t - t1) / (t2 - t1));
		Point A3 = p2*((t3 - t) / (t3 - t2)) + p2*((t - t2) / (t3 - t2));

		Point B1 = A1*((t2 - t) / (t2 - t0)) + A2*((t - t0) / (t2 - t0));
		Point B2 = A2*((t3 - t) / (t3 - t1)) + A3*((t - t1) / (t3 - t1));

		Point C = B1*((t2 - t) / (t2 - t1)) + B2*((t - t1) / (t2 - t1));

		InterpolatedValue[index++] = C;
	}
	return InterpolatedValue;
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
