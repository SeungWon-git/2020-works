#pragma warning(disable: 4996)
#include <stdio.h>
#include <random>
#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat cubeShape[36][3] = {
	{-0.1 ,-0.1 ,-0.1},{0.1 ,0.1 ,-0.1},{0.1 ,-0.1 ,-0.1},			
	{-0.1 ,-0.1 ,-0.1},{-0.1 ,0.1 ,-0.1},{0.1 ,0.1 ,-0.1},			
	{-0.1 ,-0.1 ,-0.1},{-0.1 ,0.1 ,0.1},{-0.1 ,0.1 ,-0.1},			
	{-0.1 ,-0.1 ,-0.1},{-0.1 ,-0.1 ,0.1},{-0.1 ,0.1 ,0.1},			
	{-0.1 ,0.1 ,-0.1},{0.1 ,0.1 ,0.1},{0.1 ,0.1 ,-0.1},				
	{-0.1 ,0.1 ,-0.1},{-0.1 ,0.1 ,0.1},{0.1 ,0.1 ,0.1},				
	{0.1 ,-0.1 ,-0.1},{0.1 ,0.1 ,-0.1},{0.1 ,0.1 ,0.1},				
	{0.1 ,-0.1 ,-0.1},{0.1 ,0.1 ,0.1},{0.1 ,-0.1 ,0.1},				
	{-0.1 ,-0.1 ,-0.1},{0.1 ,-0.1 ,-0.1},{0.1 ,-0.1 ,0.1},			
	{-0.1 ,-0.1 ,-0.1},{0.1 ,-0.1 ,0.1},{-0.1 ,-0.1 ,0.1},			
	{-0.1 ,-0.1 ,0.1},{0.1 ,-0.1 ,0.1},{0.1 ,0.1 ,0.1},				
	{-0.1 ,-0.1 ,0.1},{0.1 ,0.1 ,0.1},{-0.1 ,0.1 ,0.1}				
};

GLfloat pyramidShape[18][3] = {
	{-0.1,-0.1, -0.1},{0.0, 0.125, 0.0},{0.1, -0.1, -0.1},
	{-0.1,-0.1, -0.1},{-0.1, -0.1, 0.1},{0.0, 0.125, 0.0},
	{0.1,-0.1, -0.1},{0.0, 0.125, 0.0},{0.1, -0.1, 0.1},
	{-0.1, -0.1, -0.1},{0.1, -0.1,-0.1 },{0.1, -0.1, 0.1},
	{-0.1, -0.1, -0.1},{0.1, -0.1, 0.1},{-0.1, -0.1, 0.1},
	{-0.1, -0.1, 0.1},{0.1,-0.1, 0.1},{0.0, 0.125, 0.0}
};

GLfloat linex[2][3] = { {-0.9,0.0,0.0},{0.9,0.0,0.0} };
GLfloat liney[2][3] = { {0.0,-0.9,0.0},{0.0,0.9,0.0} };
GLfloat linez[2][3] = { {0.0,0.0,-0.9},{0.0,0.0,0.9} };

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid_color{ 0.0,1.0 };

GLuint c_vao, c_vbo[2],p_vao,p_vbo[2];
GLuint lx_vao, ly_vao, lz_vao, lx_vbo[2], ly_vbo[2], lz_vbo[2];

bool obj = 0;  //0-우 육면,좌 구  1-우 피라미드,좌 원뿔
GLclampf x_ro = 0.0f, y_ro = 0.0f, y_re = 0.0f;	//방향전환
GLfloat X_ro = 0.0f, Y_ro = 0.0f, Y_re = 0.0f;		//각도
bool stop_animation = 1;




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

void ReadObj(FILE* objFile)	//리턴값을 주게 바꿔야함
{
	using namespace glm;

	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];
	int vertexNum = 0;
	int faceNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // 배열 초기화
	}

	//--- 2. 메모리 할당
	vec4* vertex;
	vec4* face;
	int vertIndex = 0;
	int faceIndex = 0;
	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
	face = (vec4*)malloc(sizeof(vec4) * faceNum);

	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	char bind[100];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&vertex[vertIndex].x, &vertex[vertIndex].y,
				&vertex[vertIndex].z);
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&face[faceIndex].x, &face[faceIndex].y, &face[faceIndex].z);
			faceIndex++;
		}
	}
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexshader, 1, (const GLchar * *)& vertexsource, 0);

	glCompileShader(vertexshader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentshader, 1, (const GLchar * *)& fragmentsource, 0);

	glCompileShader(fragmentshader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &c_vao);
	glBindVertexArray(c_vao);
	glGenBuffers(2, c_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, c_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), cubeShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);
	glGenBuffers(2, p_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof(GLfloat), pyramidShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &lx_vao);
	glBindVertexArray(lx_vao);
	glGenBuffers(2, lx_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, lx_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), linex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &ly_vao);
	glBindVertexArray(ly_vao);
	glGenBuffers(2, ly_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ly_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), liney, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &lz_vao);
	glBindVertexArray(lz_vao);
	glGenBuffers(2, lz_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, lz_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), linez, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void InitShader()
{
	make_vertexShader();
	make_fragmentShader();

	s_program = glCreateProgram();

	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	glUseProgram(s_program);
}


GLvoid drawScene()
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);


	glUseProgram(s_program);

	glm::mat4 model = glm::mat4(1.0f);
	unsigned int modelLocation;

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(Y_re), glm::vec3(0.0, 1.0, 0.0));
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	//x,y,z축
	glBindVertexArray(lx_vao);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(ly_vao);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(lz_vao);
	glDrawArrays(GL_LINES, 0, 2);

	//도형
	if (obj == 0) {
		//좌측 - 구
		model = glm::translate(model, glm::vec3(-0.45, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(X_ro), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(Y_ro), glm::vec3(0.0, 1.0, 0.0));
		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		GLUquadric* qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluSphere(qobj, 0.1, 30, 30);

		//구 중심 y축 회전 방지
		model = glm::rotate(model, glm::radians(-Y_ro), glm::vec3(0.0, 1.0, 0.0));
		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		//우측 - 육면체
		model = glm::translate(model, glm::vec3(0.9, 0.0, 0.0));		
		model = glm::rotate(model, glm::radians(Y_ro), glm::vec3(0.0, 1.0, 0.0));
		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(c_vao);
		glDrawArrays(GL_LINE_STRIP, 0, 36);
		
	}
	else if (obj == 1) {
		//우측 - 피라미드 
		model = glm::translate(model, glm::vec3(0.45, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(X_ro), glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model, glm::radians(Y_ro), glm::vec3(0.0, 1.0, 0.0));
		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(p_vao);
		glDrawArrays(GL_LINE_STRIP, 0, 18);

		//피라미드 중심 y축 회전 방지(원래 원뿔이 중심이어야하는데 어차피 자전이니까 이렇게 해도 같다.)
		model = glm::rotate(model, glm::radians(-Y_ro), glm::vec3(0.0, 1.0, 0.0));
		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		//좌측 - 원뿔
		model = glm::translate(model, glm::vec3(-0.9, 0.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0, -0.125, 0.0));
		model = glm::rotate(model, glm::radians(Y_ro), glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));


		GLUquadric* qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE);
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricOrientation(qobj, GLU_OUTSIDE);
		gluCylinder(qobj, 0.1, 0.0, 0.25, 20, 8);
	}

	glutSwapBuffers();
}

void TimerRotate_X(int value)
{
	if (stop_animation == 0) {

		X_ro += x_ro;

		if (X_ro == 360.0f)
			X_ro = 0.0f;
		if (X_ro == 0.0f)
			X_ro = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerRotate_X, 1);
	}
}

void TimerRotate_Y(int value)
{
	if (stop_animation == 0) {

		Y_ro += y_ro;

		if (Y_ro == 360.0f)
			Y_ro = 0.0f;
		if (Y_ro == 0.0f)
			Y_ro = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerRotate_Y, 1);
	}
}

void TimerRevolution_Y(int value)
{
	if (stop_animation == 0) {

		Y_re += y_re;

		if (Y_re == 360.0f)
			Y_re = 0.0f;
		if (Y_re == 0.0f)
			Y_re = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerRevolution_Y, 1);
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'X':
		stop_animation = 0;
		x_ro = 1.0f;
		glutTimerFunc(0, TimerRotate_X, 1);
		break;
	case 'x':
		stop_animation = 0;
		x_ro = -1.0f;
		glutTimerFunc(0, TimerRotate_X, 1);
		break;
	case 'Y':
		stop_animation = 0;
		y_ro = 1.0f;
		glutTimerFunc(0, TimerRotate_Y, 1);
		break;
	case 'y':
		stop_animation = 0;
		y_ro = -1.0f;
		glutTimerFunc(0, TimerRotate_Y, 1);
		break;
	case 'B':
	case 'b':
		stop_animation = 0;
		if (y_re >= 1.0f)
			y_re = -1.0f;
		else y_re = 1.0f;
		glutTimerFunc(0, TimerRevolution_Y, 1);
		break;
	case 'S':
		stop_animation = 1;
		X_ro = 0.0, Y_ro = 0.0, Y_re = 0.0;
		break;

	case 'C':
		if (obj == 0)
			obj = 1;
		else obj = 0;
		break;

	case 'q':
		exit(0);
		break;
	case 27:
		exit(0);
		break;

		InitBuffer();
	}
	glutPostRedisplay();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Example14");

	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}