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

GLfloat sin_g[200][3] = {};

GLfloat spring_g[200][3] = {};

GLfloat zigzag_g[200][3] = {};

GLfloat triShape[3][3] = {
	{-0.1, -0.085, 0.0},{0.1, -0.085, 0.0},{0.0, 0.085, 0.0}
};

GLfloat recShape[4][3] = {
	{-0.1, -0.1, 0.0},{0.1, -0.1, 0.0},{0.1, 0.1, 0.0},{-0.1, 0.1, 0.0}
};

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

GLfloat linex[2][3] = { {-1.0,0.0,0.0},{1.0,0.0,0.0} };
GLfloat liney[2][3] = { {0.0,-1.0,0.0},{0.0,1.0,0.0} };

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid_color{ 0.0,1.0 };

GLuint sin_vao, sin_vbo[2], spring_vao, spring_vbo[2], zigzag_vao, zigzag_vbo[2];
GLuint tri_vao, tri_vbo[2], rec_vao, rec_vbo[2],c_vao, c_vbo[2];
GLuint lx_vao, ly_vao, lx_vbo[2], ly_vbo[2];

int path = 1; //1-사인,2-스프링,3-지그재그
int obj = 1;  //1-삼각형,2-사각형,3-육면체,4-원뿔,5-구
GLfloat move_x = 0.0f,move_y = 0.0f,move_z = 0.0f;
GLclampf change_size = 1.0f; //크기배율
GLclampf c_s = 0.01f;	//수축/팽창 전환
GLclampf r_y = 0.0f;	//방향전환
GLfloat RY = 0.0f;		//각도
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
	glGenVertexArrays(1, &sin_vao);
	glBindVertexArray(sin_vao);
	glGenBuffers(2, sin_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sin_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 600 * sizeof(GLfloat), sin_g, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &spring_vao);
	glBindVertexArray(spring_vao);
	glGenBuffers(2, spring_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, spring_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 600 * sizeof(GLfloat), spring_g, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &zigzag_vao);
	glBindVertexArray(zigzag_vao);
	glGenBuffers(2, zigzag_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, zigzag_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 600 * sizeof(GLfloat), zigzag_g, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &tri_vao);
	glBindVertexArray(tri_vao);
	glGenBuffers(2, tri_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tri_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &rec_vao);
	glBindVertexArray(rec_vao);
	glGenBuffers(2, rec_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rec_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), recShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &c_vao);
	glBindVertexArray(c_vao);
	glGenBuffers(2, c_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, c_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), cubeShape, GL_STATIC_DRAW);
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
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	//x,y축
	glBindVertexArray(lx_vao);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(ly_vao);
	glDrawArrays(GL_LINES, 0, 2);

	model = glm::rotate(model, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	if (path == 1) {//sin그래프
		glBindVertexArray(sin_vao);
		glDrawArrays(GL_LINE_STRIP, 0, 200);
	}
	else if (path == 2) {//스프링
		glBindVertexArray(spring_vao);
		glDrawArrays(GL_LINE_STRIP, 0, 200);
	}
	else if (path == 3) {//지그재그
		glBindVertexArray(zigzag_vao);
		glDrawArrays(GL_LINE_STRIP, 0, 200);
	}

	model = glm::translate(model, glm::vec3(move_x, move_y, move_z));
	model = glm::scale(model, glm::vec3(change_size, change_size, change_size));
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	//도형
	if (obj == 1) {
		glBindVertexArray(tri_vao);
		glDrawArrays(GL_LINE_LOOP, 0, 3);	
	}
	else if (obj == 2) {
		glBindVertexArray(rec_vao);
		glDrawArrays(GL_LINE_LOOP, 0, 4);
	}
	else if (obj == 3) {
		glBindVertexArray(c_vao);
		glDrawArrays(GL_LINE_STRIP, 0, 36);
	}
	else if (obj == 4) {
		model = glm::translate(model, glm::vec3(0.0, -0.125, 0.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		modelLocation = glGetUniformLocation(s_program, "modelTransform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		GLUquadric* qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); //→ 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); //→ 생략 가능
		gluCylinder(qobj, 0.1, 0.0, 0.25,20,8); // 객체(원뿔) 만들기
	}
	else if (obj == 5) {
		GLUquadric* qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); //→ 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); //→ 생략 가능
		gluSphere(qobj, 0.1, 30, 30); // 객체(원) 만들기
	}


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerRotate_RY(int value)
{
	if (stop_animation == 0) {

		RY += r_y;
		
		if (RY == 360.0f)
			RY = 0.0f;
		if (RY == 0.0f)
			RY = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerRotate_RY, 1);
	}
}

void TimerMovetoright(int value)
{
	if (stop_animation == 0) {

		if (path == 1)
		{
			move_x += 0.01f;
			if (move_x >= 1.0f) move_x = -1.0f;
			move_y = 0.5 * glm::sin(move_x * 10);
			move_z = 0.0f;
		}
		else if (path == 2) {
			move_x += 0.01f;
			if (move_x >= 1.0) move_x = -1.0f;
			move_y = 0.5 * glm::sin(move_x * 10);
			move_z = 0.5 * glm::cos(move_x * 10);
		}
		else if (path == 3) {
			move_x += 0.01f;
			if (move_x >= 1.0f) move_x = -1.0f;
			if (move_x <= -0.6f) 
				move_y = 1.25f * (move_x + 1.0f);
			else if (move_x <= -0.4f) 
				move_y = -5.0f * (move_x + 0.5);
			else if (move_x <= 0.4f) 
				move_y = 1.25f * move_x;
			else if (move_x <= 0.6f)
				move_y = -5.0f * (move_x - 0.5);
			else if (move_x <= 1.0f)
				move_y = 1.25f * (move_x - 1.0f);
		
			move_z = 0.0f;
		}


		glutPostRedisplay();
		glutTimerFunc(50, TimerMovetoright, 1);
	}
}

void TimerChangeSize(int value)
{
	if (stop_animation == 0) {

		change_size += c_s;
		if (change_size >= 2.5f) c_s *= -1.0f;
		else if (change_size <= 0.05f) c_s *= -1.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerChangeSize, 1);
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		path = 1;
		break;
	case '2':
		path = 2;
		break;
	case '3':
		path = 3;
		break;

	case 'T':
		stop_animation = 0;
		glutTimerFunc(50, TimerMovetoright, 1);
		break;
	case 'S':
		stop_animation = 0;
		glutTimerFunc(50, TimerChangeSize, 1);
		break;
	case 'R':
		stop_animation = 0;
		if (r_y >= 1.0f)
			r_y = -1.0f;
		else r_y = 1.0f;
		glutTimerFunc(0, TimerRotate_RY, 1);
		break;
	case 'C':
		stop_animation = 1;
		move_x = 0.0; move_y = 0.0; move_z = 0.0;
		RY = 0.0f;
		change_size = 1.0f;
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

void Mouse(int b, int s, int x, int y)
{
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		obj += 1;
		if (obj == 6)
			obj = 1;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Example13");

	glewExperimental = GL_TRUE;
	glewInit();

	int i = 0;
	for (float x = -1.0f; x <= 1.0f; x += 0.01f)
	{
		sin_g[i][0] = x;
		sin_g[i][1] = 0.5 * glm::sin(x * 10);
		sin_g[i][2] = 0.0;
		i++;	
	}

	i = 0;
	for (float x = -1.0f; x <= 1.0f; x += 0.01f)
	{
		spring_g[i][0] = x;
		spring_g[i][1] = 0.5 * glm::sin(x * 10);
		spring_g[i][2] = 0.5 * glm::cos(x * 10);
		i++;
	}

	i = 0;
	for (float x = -1.0f; x <= 1.0f; x += 0.01f)
	{
		zigzag_g[i][0] = x;

		if (x <= -0.6f)
			zigzag_g[i][1] = 1.25f * (x + 1.0f);
		else if (x <= -0.4f)
			zigzag_g[i][1] = -5.0f * (x + 0.5);
		else if (x <= 0.4f)
			zigzag_g[i][1] = 1.25f * x;
		else if (x <= 0.6f)
			zigzag_g[i][1] = -5.0f * (x - 0.5);
		else if (x <= 1.0f)
			zigzag_g[i][1] = 1.25f * (x - 1.0f);

		zigzag_g[i][2] = 0.0;

		i++;
	}

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}