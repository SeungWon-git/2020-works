#pragma warning(disable: 4996)
#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLfloat Shape1[3][3] = {
{ -0.8,0.2,0.0}, { -0.5, 0.5, 0.0 }, { -0.2, 0.8, 0.0}};

GLfloat Shape2[6][3] = {
{ 0.2, 0.2, 0.0} ,{ 0.5, 0.7, 0.0 }, { 0.8, 0.2, 0.0 }, { 0.2, 0.2, 0.0 },{0.5,0.7,0.0},{0.5,0.7,0.0} };

GLfloat Shape3[9][3] = {
{ -0.8, -0.8, 0.0 }, { -0.8, -0.2, 0.0 }, { -0.2, -0.8, 0.0}, { -0.2, -0.8, 0.0} , { -0.8, -0.2, 0.0},{-0.5,-0.2,0.0},{-0.2,-0.8,0.0},{-0.5,-0.2,0.0},{-0.2,-0.2,0.0} };

GLfloat Shape4[9][3] = {
{ 0.3, -0.8, 0.0 }, { 0.2, -0.4, 0.0 }, { 0.7, -0.8, 0.0}, { 0.7, -0.8, 0.0}, { 0.2, -0.4, 0.0},{0.5,-0.1,0.0},{0.7,-0.8,0.0},{0.5,-0.1,0.0},{0.8,-0.4,0.0} };

GLfloat colors1[3][3] = {
{0.0, 0.0, 1.0 }, {0.0,0.0,1.0}, { 0.0, 0.0, 1.0} };

GLfloat colors2[6][3] = {
{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 } ,{ 1.0, 1.0, 0.0 }, { 1.0, 1.0, 0.0 }};

GLfloat colors3[9][3] = {
{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 }, {  0.0, 1.0, 0.0 }, {  0.0, 1.0, 0.0 },{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 },{ 0.0, 1.0, 0.0 }, { 0.0, 1.0, 0.0 } };

GLfloat colors4[9][3] = {
{ 1.0,0.0,0.0}, { 1.0,0.0,0.0}, { 1.0,0.0,0.0} , { 1.0,0.0,0.0} , {  1.0,0.0,0.0}, { 1.0,0.0,0.0}, { 1.0,0.0,0.0} , { 1.0,0.0,0.0} , {  1.0,0.0,0.0} };

GLuint vao1, vbo1[2];
GLuint vao2, vbo2[2];
GLuint vao3, vbo3[2];
GLuint vao4, vbo4[2];

int frame = 0;
int 삼단변신 = 0;

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

	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar * *)& vertexsource, 0);

	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);

	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
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

	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar * *)& fragmentsource, 0);

	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);

	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
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
	glGenVertexArrays(1, &vao1);

	glBindVertexArray(vao1); 

	glGenBuffers(2, vbo1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1[0]);

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), Shape1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1[1]);

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors1, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);


	//#vao2
	glGenVertexArrays(1, &vao2);

	glBindVertexArray(vao2);

	glGenBuffers(2, vbo2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2[0]);

	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), Shape2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2[1]);

	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), colors2, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);

	//#vao3
	glGenVertexArrays(1, &vao3);

	glBindVertexArray(vao3);

	glGenBuffers(2, vbo3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo3[0]);

	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), Shape3, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo3[1]);

	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), colors3, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);

	//#vao4
	glGenVertexArrays(1, &vao4);

	glBindVertexArray(vao4);

	glGenBuffers(2, vbo4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo4[0]);

	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), Shape4, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo4[1]);

	glBufferData(GL_ARRAY_BUFFER, 27 * sizeof(GLfloat), colors4, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
}

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

	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program 사용하기
	glUseProgram(s_program);
}


GLvoid drawScene() 
{
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(vao1);

	if(frame == 0)
		glDrawArrays(GL_LINE_STRIP, 0, 3);
	else 
		glDrawArrays(GL_TRIANGLES, 0, 3);


	glBindVertexArray(vao2);

	glDrawArrays(GL_TRIANGLES, 0, 6);


	glBindVertexArray(vao3);

	glDrawArrays(GL_TRIANGLES, 0, 9);


	glBindVertexArray(vao4);

	if (삼단변신 == 3) {
		glPointSize(10);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, 9);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

int dt = 1, dt_ = 1;
GLfloat a2 = 0.005, a3 = -0.015;				// a2:0.2/40 , a3:-0.6/40
GLfloat b = 0.0075;								// b:0.3/40
GLfloat c1 = 0.0025, c2 = a2;					// c1:0.1/40 
GLfloat c1_ = c1, c2_ = c2, b_ = b, d = 0.01;	// d:0.4/40

void TimerFunction1(int value)
{
	//선 <-> 삼각형
	Shape1[1][1] += a2; Shape1[2][1] += a3;
	//삼각형 <-> 사각형
	Shape2[5][0] -= b; Shape2[4][0] += b; Shape2[1][0] += b;
	//사각형 <-> 오각형
	Shape3[0][0] += c1; Shape3[2][0] -= c1; Shape3[3][0] -= c1; Shape3[6][0] -= c1;
	Shape3[1][1] -= c2; Shape3[4][1] -= c2; Shape3[5][1] += c1; Shape3[7][1] += c1; Shape3[8][1] -= c2;
	//오각형 <-> 점
	if(삼단변신 == -1) {
		c1_ = (-1) * c1_;
		c2_ = (-1) * c2_;
		b_ = (-1) * b_;
		d = (-1) * d;

		삼단변신++;
		dt_ = 1;
	}
	else if (삼단변신 == 0)//오각형 <-> 사각형
	{
		Shape4[0][0] -= c1_; Shape4[2][0] += c1_; Shape4[3][0] += c1_; Shape4[6][0] += c1_;
		Shape4[1][1] += c2_; Shape4[4][1] += c2_; Shape4[5][1] -= c1_; Shape4[7][1] -= c1_; Shape4[8][1] += c2_;	
	}
	else if (삼단변신 == 1)//사각형 <-> 삼각형
	{
		Shape4[1][0] += b_; Shape4[4][0] += b_; Shape4[8][0] -= b_;
	}
	else if (삼단변신 == 2)//삼각형 <-> 점
	{
		Shape4[0][0] += b_; Shape4[2][0] -= b_; Shape4[3][0] -= b_; Shape4[6][0] -= b_;
		Shape4[0][1] += c1_; Shape4[2][1] += c1_; Shape4[3][1] += c1_; Shape4[6][1] += c1_;
		Shape4[1][1] -= d; Shape4[4][1] -= d; Shape4[5][1] -= d; Shape4[7][1] -= d; Shape4[8][1] -= d;
	}
	else if (삼단변신 == 3){
		c1_ = (-1) * c1_;
		c2_ = (-1) * c2_;
		b_ = (-1) * b_;
		d = (-1) * d;

		삼단변신--;
		dt_ = -1;
	}
		


	frame += dt;

	if (frame == 40 || frame == 0) {
		a2 = (-1) * a2;
		a3 = (-1) * a3;
		b = (-1) * b;
		c1 = (-1) * c1;
		c2 = (-1) * c2;

		dt = (-1) * dt;
		삼단변신 += dt_;
	}

	InitBuffer();

	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction1, 1);

}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}


void main(int argc, char** argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example10");

	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1000, TimerFunction1, 1);
	glutMainLoop();
}