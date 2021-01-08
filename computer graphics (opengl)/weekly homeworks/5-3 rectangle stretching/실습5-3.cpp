#pragma warning(disable: 4996)
#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 

using namespace std;

GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLfloat triShape1[3][3] = {
{ -0.4, 0.3, 0.0 }, { -0.4, -0.3, 0.0 } ,{ 0.4, -0.3, 0.0 } };

GLfloat triShape2[3][3] = {
{ -0.4, 0.3, 0.0 } , { 0.4, 0.3, 0.0 } ,{ 0.4, -0.3, 0.0 } };


GLfloat colors1[3][3] = {
{1.0, 0.0, 0.0 }, {0.0,1.0,0.0}, { 1.0, 0.0, 1.0} };

GLfloat colors2[3][3] = {
{ 0.0, 1.0, 0.0 }, {0.0,1.0, 1.0 }, {1.0,1.0,0.0 } };


GLuint vao1, vbo1[2];
GLuint vao2, vbo2[2];

bool left_button = 0;
int point;


char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");				// Open file for reading
	if (!fptr)								// Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END);				// Seek to the end of the file
	length = ftell(fptr);					// Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1);		// Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET);				// Go back to the beginning of the file
	fread(buf, length, 1, fptr);			// Read the contents of the file in to the buffer
	fclose(fptr);							// Close the file
	buf[length] = 0;						// Null terminator

	return buf;								// Return the buffer
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar * *)& vertexsource, 0);

	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);

	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar * *)& fragmentsource, 0);

	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);

	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

//--- ���� �����ϰ� ������ �޾ƿ���
void InitBuffer()
{
	glGenVertexArrays(1, &vao1); //--- VAO �� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao1); //--- VAO�� ���ε��ϱ�


	glGenBuffers(2, vbo1); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo1[0]);

	// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape1, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo1[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 *float

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors1, GL_STATIC_DRAW);


	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);


	//#vao2
	glGenVertexArrays(1, &vao2);

	glBindVertexArray(vao2);

	glGenBuffers(2, vbo2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors2, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);

}

//--- ���̴� ���α׷� �����ϱ�
GLuint s_program;

void InitShader()
{
	make_vertexShader();
	make_fragmentShader();

	//-- shader Program
	s_program = glCreateProgram();

	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");

	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program ����ϱ�
	glUseProgram(s_program);
}

GLvoid drawScene()
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glBegin(GL_LINES);
	glVertex2i(0, 1.0);
	glVertex2i(0, -1.0);
	glEnd();

	glBegin(GL_LINES);
	glVertex2i(-1.0, 0);
	glVertex2i(1.0, 0);
	glEnd();


	glUseProgram(s_program);


	glBindVertexArray(vao1);

	glDrawArrays(GL_LINE_STRIP, 0, 3);


	glBindVertexArray(vao2);

	glDrawArrays(GL_LINE_STRIP, 0, 3);

	
	glutSwapBuffers(); 	
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		exit(0);
		break;
	case 27:	//esc
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Mouse(int b, int s, int x, int y)
{
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN)
	{
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - 400) / 400.0;
		Y = static_cast<GLfloat>(300 - y) / 300.0;

		if (0 <= abs(X - triShape1[0][0]) && 0.03 >= abs(X - triShape1[0][0]) && 0 <= abs(Y - triShape1[0][1]) && 0.03 >= abs(Y - triShape1[0][1]))
		{
			point = 1;
			left_button = 1;
		}
		else if (0 <= abs(X - triShape1[1][0]) && 0.03 >= abs(X - triShape1[1][0]) && 0 <= abs(Y - triShape1[1][1]) && 0.03 >= abs(Y - triShape1[1][1]))
		{
			point = 2;
			left_button = 1;
		}
		else if (0 <= abs(X - triShape1[2][0]) && 0.03 >= abs(X - triShape1[2][0]) && 0 <= abs(Y - triShape1[2][1]) && 0.03 >= abs(Y - triShape1[2][1]))
		{
			point = 3;
			left_button = 1;
		}
		else if (0 <= abs(X - triShape2[1][0]) && 0.03 >= abs(X - triShape2[1][0]) && 0 <= abs(Y - triShape2[1][1]) && 0.03 >= abs(Y - triShape2[1][1]))
		{
			point = 4;
			left_button = 1;
		}
	}
	else if (b == GLUT_LEFT_BUTTON && s == GLUT_UP) {
		point = 0;
		left_button = 0;
	}

	glutPostRedisplay();
}

void Motion(int x, int y)
{
	if (left_button == true)
	{
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - 400) / 400.0;
		Y = static_cast<GLfloat>(300 - y) / 300.0;

		if (point == 1)
		{
			triShape1[0][0] = X; triShape1[0][1] = Y;
			triShape2[0][0] = X; triShape2[0][1] = Y;
		}
		else if (point == 2)
		{
			triShape1[1][0] = X; triShape1[1][1] = Y;
		}
		else if (point == 3)
		{
			triShape1[2][0] = X; triShape1[2][1] = Y;
			triShape2[2][0] = X; triShape2[2][1] = Y;
		}
		else if (point == 4)
		{
			triShape2[1][0] = X; triShape2[1][1] = Y;
		}

		InitBuffer();
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example11");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);	
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}