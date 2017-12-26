#include<iostream>
#include<cmath>
#include<gl\glut.h>
#include<gl\GLU.h>
#include<gl\GL.h>
#define BUFFER_SIZE 10
using namespace std;

float alpha = 0.0;
float deltaAlpha = 0.001f;
float sign = 1;
//struct for point in two dimensional space
typedef struct Point {
	GLfloat x;
	GLfloat y;
	Point() {}
	Point(GLfloat x, GLfloat y) {
		this->x = x;
		this->y = y;
	}
	
	Point& operator +(Point& x) {
		Point* p = new Point(this->x + x.x, this->y + x.y);
		return *p;
	}
	Point& operator *(GLfloat x) {
		Point* p =new Point(this->x * x, this->y * x);
		return *p;
	}
}Point;

Point* buffer= new Point[BUFFER_SIZE];

GLfloat GetT(GLfloat ti, Point pi,Point pj,GLfloat alpha) {
	return ti+pow(pow(pow((pj.x-pi.x),2)+ pow((pj.y - pi.y), 2),0.5f),alpha);
}

void CatmullRomSpline(Point p0, Point p1, Point p2, Point p3, Point* buffer,int precision=100,float alpha = 0.5f) {
	int index = 0;
	GLfloat t0, t1, t2, t3;
	t0 = 0.0f;
	t1 = GetT(t0,p0,p1,alpha);
	t2 = GetT(t1, p1, p2, alpha);
	t3 = GetT(t2, p2, p3, alpha);
	for (GLfloat t = t1; t < t2; t += ((t2 - t1) / precision)) 
	{
		Point A1 = (p0*((t1 - t) / (t1 - t0))) + (p1*((t - t0) / (t1 - t0)));
		Point A2 = (p1*((t2 - t) / (t2 - t1))) + (p2*((t - t1) / (t2 - t1)));
		Point A3 = (p2*((t3 - t) / (t3 - t2))) + (p3*((t - t2) / (t3 - t2)));

		Point B1 = (A1*((t2 - t) / (t2 - t0))) + (A2*((t - t0) / (t2 - t0)));
		Point B2 = (A2*((t3 - t) / (t3 - t1))) + (A3*((t - t1) / (t3 - t1)));

		Point C = (B1*((t2 - t) / (t2 - t1))) + (B2*((t - t1) / (t2 - t1)));

		buffer[index++] = C;
	}

}


void render() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, 500, 500);
	glOrtho(0, 500, 0, 500, -100, 100.0);
	
	Point p0 = Point(10, 10);
	Point p1 = Point(100, 250);
	Point p2 = Point(250, 100);
	Point p3 = Point(490, 490);

	//Point temp = p0/2;
	//printf("%f %f \n", temp.x, temp.y);

	buffer = new Point[BUFFER_SIZE];
	CatmullRomSpline(p0, p1, p2, p3, buffer, BUFFER_SIZE,alpha);
	glPointSize(10);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS); {
		glVertex3f(p0.x, p0.y,0);
		glVertex3f(p1.x, p1.y,0);
		glVertex3f(p2.x, p2.y,0);
		glVertex3f(p3.x, p3.y,0);
	}
	glEnd();

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
		

	glColor3f(1, 1, 1);

	glPointSize(5);
	

	for (int i = 0; i < BUFFER_SIZE; i++) {
		//printf("%f %f \n",buffer[i].x,buffer[i].y);
		glPushMatrix(); {
			glTranslatef(buffer[i].x, buffer[i].y,0);
			//glutSolidSphere(5, 5, 5);
			glBegin(GL_POINTS); {
				glVertex3f(0,0,0);
			}
			glEnd();
		}
		glPopMatrix();
	}
	
	//delete buffer;
	glutSwapBuffers();
	//boundary checker
	if (alpha < 0 || alpha>1) {
		sign *= -1;
	}
	alpha += sign*deltaAlpha;

	glutPostRedisplay();
}

int main(int argc,char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);

	glutCreateWindow("it is a demonstration of Catmull-Rom Spline");
	glClearColor(0.0, 0.0, 0.0, 0);
	glutDisplayFunc(render);
	glutMainLoop();
}
