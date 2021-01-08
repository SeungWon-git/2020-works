#pragma warning(disable: 4996)
#include <iostream>
#include <random>
#include <math.h>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int b, int s, int x, int y);
void TimerFunction1(int value);
void TimerFunction2(int value);

std::random_device rd;

std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid{ 0.0,0.2 };

typedef struct circle {
	GLfloat cx, cy;
	bool flag;
}circle;

circle c_a[20];
double rad[20];
int index = 0;
bool start1 = 0;
bool start2 = 0;
bool p_flag = 0;

void display_circle()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f, 0.0f, 1.0f);

	double angle, x, y;

	glPointSize(3);
	glLineWidth(3);

	for (int j = 0; j < 20; ++j) {
		if (p_flag == 1)
			glBegin(GL_POINTS);
		else
			glBegin(GL_LINE_STRIP);

		if (c_a[j].flag == 1) {
			for (int i = 0; i <= 360; i++) {
				angle = i * 3.141592 / 180;
				x = rad[j] * cos(angle);
				y = rad[j] * sin(angle);
				glVertex2f(x + c_a[j].cx, y + c_a[j].cy);
			}
		}

		glEnd();
	}
	
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL");

	glutDisplayFunc(display_circle);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, 800, 800);
}

void Mouse(int b, int s, int x, int y)
{
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN)
	{
		c_a[index].cx = (GLclampf)((x - 400) / 400.0);		
		c_a[index].cy = (GLclampf)((400 - y) / 400.);
		c_a[index].flag = 1;
		rad[index] = uid(dre);

		if (c_a[index + 10].flag == 1)
			c_a[index + 10].flag = 0;
		

		index++;
		if (index >= 10) {
			index = 0;
			c_a[index + 10].flag = 0;
		}
		if (start1 == 0)	//Å¬¸¯½Ã »¡¶óÁü ¸·À½
			glutTimerFunc(10, TimerFunction1, 1);
	}

	glutPostRedisplay();
}

void TimerFunction1(int value) {
	start1 = 1;

	int sum_flag = 0;
	for (int i = 0; i < 10; ++i)
		sum_flag += c_a[i].flag;

	if (sum_flag != 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (c_a[i].flag == 1) {
				rad[i] += 0.005;
			
				if (c_a[i+10].flag == 0 && rad[i] >= 0.35) {
					c_a[i + 10] = c_a[i]; 
					rad[i + 10] = 0.;
	
					if (start2 == 0) 
						glutTimerFunc(10, TimerFunction2, 1);	
				}

				if (rad[i] >= 2.5)
					c_a[i].flag = 0;
			}
		}

		glutPostRedisplay();
		glutTimerFunc(10, TimerFunction1, 1);
	}
	else 
		start1 = 0;

}

void TimerFunction2(int value) {
	start2 = 1;

	int sum_flag = 0;
	for (int i = 10; i < 20; ++i)
		sum_flag += c_a[i].flag;
	
	if (sum_flag != 0)
	{
		for (int i = 10; i < 20; ++i)
		{
			if (c_a[i].flag == 1) {
				rad[i] += 0.003;

				if (rad[i] >= 2.0)
					c_a[i].flag = 0;
			}
		}

		glutPostRedisplay();
		glutTimerFunc(10, TimerFunction2, 1);
	}
	else 
		start2 = 0;

}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		p_flag = 1;
		break;
	case '2':
		p_flag = 0;
		break;
	}
	glutPostRedisplay();
}