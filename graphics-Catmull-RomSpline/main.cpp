#include<iostream>
#include<cmath>
#include<gl\glut.h>
#include<gl\GLU.h>
#include<vector>
#include<iterator>
#include<gl\GL.h>
#define BUFFER_SIZE 100
using namespace std;

enum DRAW_MODE
{
	LINE,POINT
};

DRAW_MODE dm= LINE;
float alpha = 0.0;
float deltaAlpha = 0.01f;
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
	
	Point operator +(Point& x) {
		Point p =Point(this->x + x.x, this->y + x.y);
		return p;
	}
	Point operator *(GLfloat x) {
		Point p = Point(this->x * x, this->y * x);
		return p;
	}
}Point;

//Point* buffer= new Point[BUFFER_SIZE];
vector<Point>points = vector<Point>();
vector<Point> buffer = vector<Point>();

GLfloat GetT(GLfloat ti, Point pi,Point pj,GLfloat alpha) {
	return ti+pow(pow(pow((pj.x-pi.x),2)+ pow((pj.y - pi.y), 2),0.5f),alpha);
}

void CatmullRomSpline(Point p0, Point p1, Point p2, Point p3, int precision=100,float alpha = 0.5f) {
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

		buffer.push_back(C);
	}

}


void render() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, 1000, 1000);
	glOrtho(0, 1000, 0, 1000, -100, 100.0);
	buffer.clear();
	
	
	
	glPointSize(10);
	


	for (vector<Point>::iterator iter = points.begin(); next(iter, 3) != points.end(); iter++) {
		CatmullRomSpline(*iter, *next(iter), *next(iter,2), *next(iter, 3), BUFFER_SIZE, alpha);
	}

	glColor3f(1, 0, 0);
	glBegin(GL_POINTS); 
	{
		for (vector<Point>::iterator iter = points.begin(); iter != points.end(); iter++) {
			glVertex3f(iter->x, iter->y,0);
		}
	}
	glEnd();
		

	glColor3f(1, 1, 1);
	glPointSize(5);
	
	switch(dm){
	case LINE:
		glLineWidth(2);
		glBegin(GL_LINE_STRIP); {
			for (vector<Point>::iterator it = buffer.begin(); it != buffer.end(); it++) {
				glVertex3f((*it).x, (*it).y, 0);
			}
		}
		glEnd();
		break;
	case POINT:
		for (vector<Point>::iterator it = buffer.begin(); it!= buffer.end(); it++) {
			//printf("%f %f \n",buffer[i].x,buffer[i].y);
			glPushMatrix(); {
				glTranslatef((*it).x, (*it).y,0);
		
				glBegin(GL_POINTS); {
					glVertex3f(0,0,0);
				}
				glEnd();
			}
			glPopMatrix();
		}
		break;
	}
	
	glutSwapBuffers();
	//boundary checker
	if (alpha < 0 || alpha>1) {
		sign *= -1;
	}
	alpha += sign*deltaAlpha;

	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
	if (button == 0 && state == 1) {
		Point p = Point(x,1000-y);
		points.push_back(p);
	}
	
}

int main(int argc,char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_ALPHA|GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);

	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_PHONG_WIN);
	glHint(GL_PHONG_HINT_WIN, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	

	glutCreateWindow("it is a demonstration of Catmull-Rom Spline");
	glClearColor(0.0, 0.0, 0.0, 0);

	Point p0 = Point(10, 10);
	Point p1 = Point(100, 250);
	Point p2 = Point(250, 100);
	Point p3 = Point(490, 490);

	points.clear();

	points.push_back(p0);
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);

	glutDisplayFunc(render);
	glutMouseFunc(MouseFunc);
	glutMainLoop();
}
