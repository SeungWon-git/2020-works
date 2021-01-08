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
	{-0.25 ,-0.25 ,-0.25},{0.25 ,0.25 ,-0.25},{0.25 ,-0.25 ,-0.25},
	{-0.25 ,-0.25 ,-0.25},{-0.25 ,0.25 ,-0.25},{0.25 ,0.25 ,-0.25},
	{-0.25 ,-0.25 ,-0.25},{-0.25 ,0.25 ,0.25},{-0.25 ,0.25 ,-0.25},
	{-0.25 ,-0.25 ,-0.25},{-0.25 ,-0.25 ,0.25},{-0.25 ,0.25 ,0.25},
	{-0.25 ,0.25 ,-0.25},{0.25 ,0.25 ,0.25},{0.25 ,0.25 ,-0.25},
	{-0.25 ,0.25 ,-0.25},{-0.25 ,0.25 ,0.25},{0.25 ,0.25 ,0.25},
	{0.25 ,-0.25 ,-0.25},{0.25 ,0.25 ,-0.25},{0.25 ,0.25 ,0.25},
	{0.25 ,-0.25 ,-0.25},{0.25 ,0.25 ,0.25},{0.25 ,-0.25 ,0.25},
	{-0.25 ,-0.25 ,-0.25},{0.25 ,-0.25 ,-0.25},{0.25 ,-0.25 ,0.25},
	{-0.25 ,-0.25 ,-0.25},{0.25 ,-0.25 ,0.25},{-0.25 ,-0.25 ,0.25},
	{-0.25 ,-0.25 ,0.25},{0.25 ,-0.25 ,0.25},{0.25 ,0.25 ,0.25},
	{-0.25 ,-0.25 ,0.25},{0.25 ,0.25 ,0.25},{-0.25 ,0.25 ,0.25}
};

GLfloat pyramidShape[18][3] = {
	{-0.25,-0.25, -0.25},{0.0, 0.2, 0.0},{0.25, -0.25, -0.25},
	{-0.25,-0.25, -0.25},{-0.25, -0.25, 0.25},{0.0, 0.2, 0.0},
	{0.25,-0.25, -0.25},{0.0, 0.2, 0.0},{0.25, -0.25, 0.25},
	{-0.25, -0.25, -0.25},{0.25, -0.25,-0.25 },{0.25, -0.25, 0.25},
	{-0.25, -0.25, -0.25},{0.25, -0.25, 0.25},{-0.25, -0.25, 0.25},
	{-0.25, -0.25, 0.25},{0.25,-0.25, 0.25},{0.0, 0.2, 0.0}
};

GLfloat linex[2][3] = { {-1.0,0.0,0.0},{1.0,0.0,0.0} };
GLfloat liney[2][3] = { {0.0,-1.0,0.0},{0.0,1.0,0.0} };

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid_color{ 0.0,1.0 };

GLfloat cubeColor[36][3] = {
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)}
};

GLfloat pyramidColor[18][3] = {
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},
	{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)},{uid_color(dre), uid_color(dre), uid_color(dre)}
};

GLuint c_vao, c_vbo[2];
GLuint p_vao, p_vbo[2];
GLuint lx_vao, ly_vao, lx_vbo[2], ly_vbo[2];

bool cube = 1;
bool hide = 1;
GLclampf r_x = 1.0f;
GLclampf r_y = 1.0f;
bool stop_animation = 1;
bool wire = 0;
GLfloat X = 0.0f, Y = 0.0f, Z = 0.0f;
GLfloat RX = 30.0f, RY = -30.0f;



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
	glBindBuffer(GL_ARRAY_BUFFER, c_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), cubeColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);
	glGenBuffers(2, p_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof(GLfloat), pyramidShape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof(GLfloat), pyramidColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

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
	if (hide == 1)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	glUseProgram(s_program);

	glm::mat4 model = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model)); 
	
	glBindVertexArray(lx_vao);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(ly_vao);
	glDrawArrays(GL_LINES, 0, 2);

	model = glm::translate(model, glm::vec3(X, Y, Z));
	model = glm::rotate(model, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	modelLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	
	if (wire == 0) {
		if (cube == 1) {
			glBindVertexArray(c_vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else {
			glBindVertexArray(p_vao);
			glDrawArrays(GL_TRIANGLES, 0, 18);
		}
	}
	else
	{
		if (cube == 1) {
			glBindVertexArray(c_vao);
			glDrawArrays(GL_LINE_STRIP, 0, 36);
		}
		else {
			glBindVertexArray(p_vao);
			glDrawArrays(GL_LINE_STRIP, 0, 18);
		}
	}

	
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerRotate_RX(int value)
{
	if (stop_animation == 0) {

		RX += r_x;
		if (RX == 360.0f)
			RX = 0.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerRotate_RX, 1);
	}
}

void TimerRotate_RY(int value)
{
	if (stop_animation == 0) {

		RY += r_y;
		if (RY == 360.0f)
			RY = 0.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerRotate_RY, 1);
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		cube = 1;
		break;
	case 'p':
		cube = 0;
		break;

	case 'h':
		if (hide == 0)
			hide = 1;
		else hide = 0;
		break;
	case 'w':
		wire = 1;
		break;
	case 'W':
		wire = 0;
		break;

	case 'x':
		stop_animation = 0;
		r_x = 1.0f;
		glutTimerFunc(0, TimerRotate_RX, 1);
		break;
	case 'X':
		stop_animation = 0;
		r_x = -1.0f;
		glutTimerFunc(0, TimerRotate_RX, 1);
		break;
	case 'y':
		stop_animation = 0;
		r_y = 1.0f;
		glutTimerFunc(0, TimerRotate_RY, 1);
		break;
	case 'Y':
		stop_animation = 0;
		r_y = -1.0f;
		glutTimerFunc(0, TimerRotate_RY, 1);
		break;
	

	case 's':
		stop_animation = 1;
		RX = 30.0f;
		RY = -30.0f;
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

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		X -= 0.1f;
		break;
	case GLUT_KEY_RIGHT:
		X += 0.1f;
		break;
	case GLUT_KEY_UP:
		Y += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		Y -= 0.1f;
		break;

		InitBuffer();
	}
	glutPostRedisplay();
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Example12");

	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMainLoop();
}