#pragma warning(disable: 4996)
#include <iostream>
#include <random>
#include <string>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 

GLvoid drawScene(GLvoid);
GLvoid Keyboard(unsigned char key, int x, int y);

GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
GLuint vertexshader, fragmentshader; // ���̴� ��ü

int angle = 0;

std::random_device rd;

std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid{ -1.0,1.0 };
std::uniform_real_distribution<> uid__{ -0.3,0.3 };
std::uniform_real_distribution<> uid_{ 0.01,0.04 };

GLfloat triShape1[3][3] = { 
{ -0.3, -0.85, 0.0  }, { triShape1[0][0] + 0.2,triShape1[0][1], 0.0 }, { triShape1[0][0] + 0.1, triShape1[0][1] + 0.3, 0.0} };

GLfloat triShape2[3][3] = {
{ -0.3, 0.85, 0.0  }, { triShape2[0][0] + 0.2,triShape2[0][1], 0.0 }, { triShape2[0][0] + 0.1, triShape2[0][1] + 0.3, 0.0} };

GLfloat triShape3[3][3] = {
{ 0.3, -0.85, 0.0  }, { triShape3[0][0] + 0.2,triShape3[0][1], 0.0 }, { triShape3[0][0] + 0.1, triShape3[0][1] + 0.3, 0.0} };

GLfloat triShape4[3][3] = {
{ 0.3, 0.85, 0.0  }, { triShape4[0][0] + 0.2,triShape4[0][1], 0.0 }, { triShape4[0][0] + 0.1, triShape4[0][1] + 0.3, 0.0} };

GLfloat triShape5[3][3] = {
{ -0.4, -0.9, 0.0  }, { triShape5[0][0] + 0.2,triShape5[0][1], 0.0 }, { triShape5[0][0] + 0.1, triShape5[0][1] + 0.3, 0.0} };

GLfloat triShape6[3][3] = {
{ 0.4, -0.9, 0.0  }, { triShape6[0][0] + 0.2,triShape6[0][1], 0.0 }, { triShape6[0][0] + 0.1, triShape6[0][1] + 0.3, 0.0} };

GLfloat triShape7[3][3] = {
{ -0.4, 0.9, 0.0  }, { triShape7[0][0] + 0.2,triShape7[0][1], 0.0 }, { triShape7[0][0] + 0.1, triShape7[0][1] + 0.3, 0.0} };

GLfloat triShape8[3][3] = {
{ 0.4, 0.9, 0.0  }, { triShape8[0][0] + 0.2,triShape8[0][1], 0.0 }, { triShape8[0][0] + 0.1, triShape8[0][1] + 0.3, 0.0} };

GLfloat triShape9[3][3] = {
{ -0.3, -0.8, 0.0  }, { triShape9[0][0] + 0.2,triShape9[0][1], 0.0 }, { triShape9[0][0] + 0.1, triShape9[0][1] + 0.3, 0.0} };

GLfloat triShape10[3][3] = {
{ 0.3, 0.8, 0.0  }, { triShape10[0][0] + 0.2,triShape10[0][1], 0.0 }, { triShape10[0][0] + 0.1, triShape10[0][1] + 0.3, 0.0} };


GLfloat small1[3][3] = { 
{ uid__(dre), uid__(dre), 0.0 }, { small1[0][0] + 0.1,small1[0][1], 0.0 }, { small1[0][0] + 0.05, small1[0][1] + 0.15, 0.0} };

GLfloat small2[3][3] = { 
{ uid__(dre), uid__(dre), 0.0 }, { small2[0][0] + 0.1,small2[0][1], 0.0 }, { small2[0][0] + 0.05, small2[0][1] + 0.15, 0.0} };


GLfloat colors[3][3] = { 
{ 0.3,0.0,1.0 }, {colors[0][0],colors[0][1], colors[0][2] }, {1.0, 0.0, 0.0 } };

GLfloat colors1_[3][3] = {
{ uid(dre),uid(dre),uid(dre) }, {colors1_[0][0],colors1_[0][1], colors1_[0][2] }, {1.0, 1.0, 1.0 } };

GLfloat colors2_[3][3] = {
{ uid(dre),uid(dre),uid(dre) }, {colors2_[0][0],colors2_[0][1], colors2_[0][2] }, {1.0, 1.0, 1.0 } };

GLuint vao[12];
GLuint vbo1[2], vbo2[2], vbo3[2], vbo4[2], vbo5[2], vbo6[2], vbo7[2], vbo8[2], vbo9[2], vbo10[2],
		vbo1_[2], vbo2_[2];

bool up[12], right[12];
double speed[12];

// i�δ� �ε��� ,d�� ���� (��,��,�Ʒ�,���������� �浹) 1~4�� �־� triShape�� ������
GLfloat triShapeNum(int i, int d) {
	GLfloat R = 0, L = 0, U = 0, B = 0;
	if (i <= 9) {
		if (i == 0) {
			R = triShape1[0][0] >= triShape1[1][0] ? triShape1[0][0] : triShape1[1][0];
			R = R >= triShape1[2][0] ? R : triShape1[2][0];
			L = triShape1[0][0] <= triShape1[1][0] ? triShape1[0][0] : triShape1[1][0];
			L = L <= triShape1[2][0] ? L : triShape1[2][0];
			U = triShape1[0][1] >= triShape1[1][1] ? triShape1[0][1] : triShape1[1][1];
			U = U >= triShape1[2][1] ? U : triShape1[2][1];
			B = triShape1[0][1] <= triShape1[1][1] ? triShape1[0][1] : triShape1[1][1];
			B = B <= triShape1[2][1] ? B : triShape1[2][1];
		}
		else if (i == 1) {
			R = triShape2[0][0] >= triShape2[1][0] ? triShape2[0][0] : triShape2[1][0];
			R = R >= triShape2[2][0] ? R : triShape2[2][0];
			L = triShape2[0][0] <= triShape2[1][0] ? triShape2[0][0] : triShape2[1][0];
			L = L <= triShape2[2][0] ? L : triShape2[2][0];
			U = triShape2[0][1] >= triShape2[1][1] ? triShape2[0][1] : triShape2[1][1];
			U = U >= triShape2[2][1] ? U : triShape2[2][1];
			B = triShape2[0][1] <= triShape2[1][1] ? triShape2[0][1] : triShape2[1][1];
			B = B <= triShape2[2][1] ? B : triShape2[2][1];
		}
		else if (i == 2) {
			R = triShape3[0][0] >= triShape3[1][0] ? triShape3[0][0] : triShape3[1][0];
			R = R >= triShape3[2][0] ? R : triShape3[2][0];
			L = triShape3[0][0] <= triShape3[1][0] ? triShape3[0][0] : triShape3[1][0];
			L = L <= triShape3[2][0] ? L : triShape3[2][0];
			U = triShape3[0][1] >= triShape3[1][1] ? triShape3[0][1] : triShape3[1][1];
			U = U >= triShape3[2][1] ? U : triShape3[2][1];
			B = triShape3[0][1] <= triShape3[1][1] ? triShape3[0][1] : triShape3[1][1];
			B = B <= triShape3[2][1] ? B : triShape3[2][1];
		}
		else if (i == 3) {
			R = triShape4[0][0] >= triShape4[1][0] ? triShape4[0][0] : triShape4[1][0];
			R = R >= triShape4[2][0] ? R : triShape4[2][0];
			L = triShape4[0][0] <= triShape4[1][0] ? triShape4[0][0] : triShape4[1][0];
			L = L <= triShape4[2][0] ? L : triShape4[2][0];
			U = triShape4[0][1] >= triShape4[1][1] ? triShape4[0][1] : triShape4[1][1];
			U = U >= triShape4[2][1] ? U : triShape4[2][1];
			B = triShape4[0][1] <= triShape4[1][1] ? triShape4[0][1] : triShape4[1][1];
			B = B <= triShape4[2][1] ? B : triShape4[2][1];
		}
		else if (i == 4) {
			R = triShape5[0][0] >= triShape5[1][0] ? triShape5[0][0] : triShape5[1][0];
			R = R >= triShape5[2][0] ? R : triShape5[2][0];
			L = triShape5[0][0] <= triShape5[1][0] ? triShape5[0][0] : triShape5[1][0];
			L = L <= triShape5[2][0] ? L : triShape5[2][0];
			U = triShape5[0][1] >= triShape5[1][1] ? triShape5[0][1] : triShape5[1][1];
			U = U >= triShape5[2][1] ? U : triShape5[2][1];
			B = triShape5[0][1] <= triShape5[1][1] ? triShape5[0][1] : triShape5[1][1];
			B = B <= triShape5[2][1] ? B : triShape5[2][1];
		}
		else if (i == 5) {
			R = triShape6[0][0] >= triShape6[1][0] ? triShape6[0][0] : triShape6[1][0];
			R = R >= triShape6[2][0] ? R : triShape6[2][0];
			L = triShape6[0][0] <= triShape6[1][0] ? triShape6[0][0] : triShape6[1][0];
			L = L <= triShape6[2][0] ? L : triShape6[2][0];
			U = triShape6[0][1] >= triShape6[1][1] ? triShape6[0][1] : triShape6[1][1];
			U = U >= triShape6[2][1] ? U : triShape6[2][1];
			B = triShape6[0][1] <= triShape6[1][1] ? triShape6[0][1] : triShape6[1][1];
			B = B <= triShape6[2][1] ? B : triShape6[2][1];
		}
		else if (i == 6) {
			R = triShape7[0][0] >= triShape7[1][0] ? triShape7[0][0] : triShape7[1][0];
			R = R >= triShape7[2][0] ? R : triShape7[2][0];
			L = triShape7[0][0] <= triShape7[1][0] ? triShape7[0][0] : triShape7[1][0];
			L = L <= triShape7[2][0] ? L : triShape7[2][0];
			U = triShape7[0][1] >= triShape7[1][1] ? triShape7[0][1] : triShape7[1][1];
			U = U >= triShape7[2][1] ? U : triShape7[2][1];
			B = triShape7[0][1] <= triShape7[1][1] ? triShape7[0][1] : triShape7[1][1];
			B = B <= triShape7[2][1] ? B : triShape7[2][1];
		}
		else if (i == 7) {
			R = triShape8[0][0] >= triShape8[1][0] ? triShape8[0][0] : triShape8[1][0];
			R = R >= triShape8[2][0] ? R : triShape8[2][0];
			L = triShape8[0][0] <= triShape8[1][0] ? triShape8[0][0] : triShape8[1][0];
			L = L <= triShape8[2][0] ? L : triShape8[2][0];
			U = triShape8[0][1] >= triShape8[1][1] ? triShape8[0][1] : triShape8[1][1];
			U = U >= triShape8[2][1] ? U : triShape8[2][1];
			B = triShape8[0][1] <= triShape8[1][1] ? triShape8[0][1] : triShape8[1][1];
			B = B <= triShape8[2][1] ? B : triShape8[2][1];
		}
		else if (i == 8) {
			R = triShape9[0][0] >= triShape9[1][0] ? triShape9[0][0] : triShape9[1][0];
			R = R >= triShape9[2][0] ? R : triShape9[2][0];
			L = triShape9[0][0] <= triShape9[1][0] ? triShape9[0][0] : triShape9[1][0];
			L = L <= triShape9[2][0] ? L : triShape9[2][0];
			U = triShape9[0][1] >= triShape9[1][1] ? triShape9[0][1] : triShape9[1][1];
			U = U >= triShape9[2][1] ? U : triShape9[2][1];
			B = triShape9[0][1] <= triShape9[1][1] ? triShape9[0][1] : triShape9[1][1];
			B = B <= triShape9[2][1] ? B : triShape9[2][1];
		}
		else if (i == 9) {
			R = triShape10[0][0] >= triShape10[1][0] ? triShape10[0][0] : triShape10[1][0];
			R = R >= triShape10[2][0] ? R : triShape10[2][0];
			L = triShape10[0][0] <= triShape10[1][0] ? triShape10[0][0] : triShape10[1][0];
			L = L <= triShape10[2][0] ? L : triShape10[2][0];
			U = triShape10[0][1] >= triShape10[1][1] ? triShape10[0][1] : triShape10[1][1];
			U = U >= triShape10[2][1] ? U : triShape10[2][1];
			B = triShape10[0][1] <= triShape10[1][1] ? triShape10[0][1] : triShape10[1][1];
			B = B <= triShape10[2][1] ? B : triShape10[2][1];
		}

		if (d == 1)
			return U;
		else if (d == 2)
			return L;
		else if (d == 3)
			return B;
		else if (d == 4)
			return R;
	}
	else if (i == 10) {
			R = small1[0][0] >= small1[1][0] ? small1[0][0] : small1[1][0];
			R = R >= small1[2][0] ? R : small1[2][0];
			L = small1[0][0] <= small1[1][0] ? small1[0][0] : small1[1][0];
			L = L <= small1[2][0] ? L : small1[2][0];
			U = small1[0][1] >= small1[1][1] ? small1[0][1] : small1[1][1];
			U = U >= small1[2][1] ? U : small1[2][1];
			B = small1[0][1] <= small1[1][1] ? small1[0][1] : small1[1][1];
			B = B <= small1[2][1] ? B : small1[2][1];

		if (d == 1)
			return U;
		else if (d == 2)
			return L;
		else if (d == 3)
			return B;
		else if (d == 4)
			return R;
	}
	else if (i == 11) {
			R = small2[0][0] >= small2[1][0] ? small2[0][0] : small2[1][0];
			R = R >= small2[2][0] ? R : small2[2][0];
			L = small2[0][0] <= small2[1][0] ? small2[0][0] : small2[1][0];
			L = L <= small2[2][0] ? L : small2[2][0];
			U = small2[0][1] >= small2[1][1] ? small2[0][1] : small2[1][1];
			U = U >= small2[2][1] ? U : small2[2][1];
			B = small2[0][1] <= small2[1][1] ? small2[0][1] : small2[1][1];
			B = B <= small2[2][1] ? B : small2[2][1];

		if (d == 1)
			return U;
		else if (d == 2)
			return L;
		else if (d == 3)
			return B;
		else if (d == 4)
			return R;
	}
}

void InitVao(GLuint& vao, GLuint vbo[2], GLfloat triShape[3][3], GLfloat colors[3][3]) {
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), colors, GL_STATIC_DRAW);


	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

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
	//#big�ﰢ��1
	InitVao(vao[0], vbo1, triShape1, colors);

	//#big�ﰢ��2
	InitVao(vao[1], vbo2, triShape2, colors);

	//#big�ﰢ��3
	InitVao(vao[2], vbo3, triShape3, colors);

	//#big�ﰢ��4
	InitVao(vao[3], vbo4, triShape4, colors);

	//#big�ﰢ��5
	InitVao(vao[4], vbo5, triShape5, colors);

	//#big�ﰢ��6
	InitVao(vao[5], vbo6, triShape6, colors);

	//#big�ﰢ��7
	InitVao(vao[6], vbo7, triShape7, colors);

	//#big�ﰢ��8
	InitVao(vao[7], vbo8, triShape8, colors);

	//#big�ﰢ��9
	InitVao(vao[8], vbo9, triShape9, colors);

	//#big�ﰢ��10
	InitVao(vao[9], vbo10, triShape10, colors);


	//#small�ﰢ��1
	InitVao(vao[10], vbo1_, small1, colors1_);

	//#small�ﰢ��2
	InitVao(vao[11], vbo2_, small2, colors2_);
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

//--- �׸��� �ݹ� �Լ�
GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
{
	//--- ����� ���� ����
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < 12; ++i) {
		// ����� VAO �ҷ�����
		glBindVertexArray(vao[i]);

		// �ﰢ�� �׸���
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	glPolygonMode(GL_FRONT, GL_LINE);
	glRectf(-0.45, -0.45, 0.45, 0.45);

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void move(GLfloat tris[3][3], double speed, int d)
{
	if (d == 1)//�ϵ�����
	{
		tris[0][0] += speed; tris[1][0] += speed; tris[2][0] += speed; tris[0][1] += speed; tris[1][1] += speed; tris[2][1] += speed;
	}
	else if (d == 2) //����
	{
		tris[0][0] += speed; tris[1][0] += speed; tris[2][0] += speed; tris[0][1] -= speed; tris[1][1] -= speed; tris[2][1] -= speed;
	}
	else if (d == 3) //����
	{
		tris[0][0] -= speed; tris[1][0] -= speed; tris[2][0] -= speed; tris[0][1] -= speed; tris[1][1] -= speed; tris[2][1] -= speed;
	}
	else if (d == 4) //�ϼ�
	{
		tris[0][0] -= speed; tris[1][0] -= speed; tris[2][0] -= speed; tris[0][1] += speed; tris[1][1] += speed; tris[2][1] += speed;
	}
}

void turn(GLfloat tris[3][3], int i, int d)
{
	if (d == 1)
	{
		tris[0][0] = tris[2][0] - 0.1; tris[1][0] = tris[2][0] + 0.1;
		tris[0][1] = triShapeNum(i, 1); tris[1][1] = triShapeNum(i, 1); tris[2][1] = triShapeNum(i, 1) - 0.3;
	}
	else if (d == 2)
	{
		tris[0][0] = triShapeNum(i, 2); tris[1][0] = triShapeNum(i, 2); tris[2][0] = triShapeNum(i, 2) + 0.3;
		tris[0][1] = tris[2][1] + 0.1; tris[1][1] = tris[2][1] - 0.1;
	}
	else if (d == 3)
	{
		tris[0][0] = tris[2][0] - 0.1; tris[1][0] = tris[2][0] + 0.1;
		tris[0][1] = triShapeNum(i, 3); tris[1][1] = triShapeNum(i, 3); tris[2][1] = triShapeNum(i, 3) + 0.3;
	}
	else if (d == 4)
	{
		tris[0][0] = triShapeNum(i, 4); tris[1][0] = triShapeNum(i, 4); tris[2][0] = triShapeNum(i, 4) - 0.3;
		tris[0][1] = tris[2][1] - 0.1; tris[1][1] = tris[2][1] + 0.1;
	}
}

void turn_(GLfloat tris[3][3], int i, int d)
{
	if (d == 1)
	{
		tris[0][0] = tris[2][0] - 0.05; tris[1][0] = tris[2][0] + 0.05;
		tris[0][1] = triShapeNum(i, 1); tris[1][1] = triShapeNum(i, 1); tris[2][1] = triShapeNum(i, 1) - 0.15;
	}
	else if (d == 2)
	{
		tris[0][0] = triShapeNum(i, 2); tris[1][0] = triShapeNum(i, 2); tris[2][0] = triShapeNum(i, 2) + 0.15;
		tris[0][1] = tris[2][1] + 0.05; tris[1][1] = tris[2][1] - 0.05;
	}
	else if (d == 3)
	{
		tris[0][0] = tris[2][0] - 0.05; tris[1][0] = tris[2][0] + 0.05;
		tris[0][1] = triShapeNum(i, 3); tris[1][1] = triShapeNum(i, 3); tris[2][1] = triShapeNum(i, 3) + 0.15;
	}
	else if (d == 4)
	{
		tris[0][0] = triShapeNum(i, 4); tris[1][0] = triShapeNum(i, 4); tris[2][0] = triShapeNum(i, 4) - 0.15;
		tris[0][1] = tris[2][1] - 0.05; tris[1][1] = tris[2][1] + 0.05;
	}

	if (i == 10 )
	{
		colors1_[0][0] = uid(dre); colors1_[0][1] = uid(dre); colors1_[0][2] = uid(dre); colors1_[1][0] = colors1_[0][0]; colors1_[1][1] = colors1_[0][1]; colors1_[1][2] = colors1_[0][2];
	}
	else if (i == 11)
	{
		colors2_[0][0] = uid(dre); colors2_[0][1] = uid(dre); colors2_[0][2] = uid(dre); colors2_[1][0] = colors2_[0][0]; colors2_[1][1] = colors2_[0][1]; colors2_[1][2] = colors2_[0][2];
	}
}

void TimerFunction(int value)
{
	for (int i = 0; i < 12; ++i) {
		if (up[i] == 1 && right[i] == 1)
		{
			if (i == 0)
				move(triShape1, speed[i], 1);
			else if (i == 1)
				move(triShape2, speed[i], 1);
			else if (i == 2)
				move(triShape3, speed[i], 1);
			else if (i == 3)
				move(triShape4, speed[i], 1);
			else if (i == 4)
				move(triShape5, speed[i], 1);
			else if (i == 5)
				move(triShape6, speed[i], 1);
			else if (i == 6)
				move(triShape7, speed[i], 1);
			else if (i == 7)
				move(triShape8, speed[i], 1);
			else if (i == 8)
				move(triShape9, speed[i], 1);
			else if (i == 9)
				move(triShape10, speed[i], 1);

			else if (i == 10)
				move(small1, speed[i], 1);
			else if (i == 11)
				move(small2, speed[i], 1);
			

			if (i <= 9 && triShapeNum(i, 1) >= -0.45f && triShapeNum(i, 1) <= 0.45f && triShapeNum(i, 4) >= -0.45f && triShapeNum(i, 4) <= 0.45f) {
				if (triShapeNum(i, 2) >= -1.0f && triShapeNum(i, 2) <= -0.45f) {
					right[i] = 0;
					if (i == 0)
						turn(triShape1, i, 4);
					else if (i == 1)
						turn(triShape2, i, 4);
					else if (i == 2)
						turn(triShape3, i, 4);
					else if (i == 3)
						turn(triShape4, i, 4);
					else if (i == 4)
						turn(triShape5, i, 4);
					else if (i == 5)
						turn(triShape6, i, 4);
					else if (i == 6)
						turn(triShape7, i, 4);
					else if (i == 7)
						turn(triShape8, i, 4);
					else if (i == 8)
						turn(triShape9, i, 4);
					else if (i == 9)
						turn(triShape10, i, 4);
				}
				else
				{
					up[i] = 0;
					if (i == 0)
						turn(triShape1, i, 1);
					else if (i == 1)
						turn(triShape2, i, 1);
					else if (i == 2)
						turn(triShape3, i, 1);
					else if (i == 3)
						turn(triShape4, i, 1);
					else if (i == 4)
						turn(triShape5, i, 1);
					else if (i == 5)
						turn(triShape6, i, 1);
					else if (i == 6)
						turn(triShape7, i, 1);
					else if (i == 7)
						turn(triShape8, i, 1);
					else if (i == 8)
						turn(triShape9, i, 1);
					else if (i == 9)
						turn(triShape10, i, 1);
				}
			}
			if (triShapeNum(i, 1) >= 1.0f) {
				up[i] = 0;
				if (i == 0)
					turn(triShape1, i, 1);
				else if (i == 1)
					turn(triShape2, i, 1);
				else if (i == 2)
					turn(triShape3, i, 1);
				else if (i == 3)
					turn(triShape4, i, 1);
				else if (i == 4)
					turn(triShape5, i, 1);
				else if (i == 5)
					turn(triShape6, i, 1);
				else if (i == 6)
					turn(triShape7, i, 1);
				else if (i == 7)
					turn(triShape8, i, 1);
				else if (i == 8)
					turn(triShape9, i, 1);
				else if (i == 9)
					turn(triShape10, i, 1);
				
			}
			if (triShapeNum(i, 1) >= 0.45f && i == 10 || triShapeNum(i, 1) >= 0.45f && i == 11) {
				up[i] = 0;
				if (i == 10)
					turn_(small1, i, 1);
				else if (i == 11)
					turn_(small2, i, 1);

			}
			if (triShapeNum(i, 4) >= 1.0f) {
				right[i] = 0;
				if (i == 0)
					turn(triShape1, i, 4);
				else if (i == 1)
					turn(triShape2, i, 4);
				else if (i == 2)
					turn(triShape3, i, 4);
				else if (i == 3)
					turn(triShape4, i, 4);
				else if (i == 4)
					turn(triShape5, i, 4);
				else if (i == 5)
					turn(triShape6, i, 4);
				else if (i == 6)
					turn(triShape7, i, 4);
				else if (i == 7)
					turn(triShape8, i, 4);
				else if (i == 8)
					turn(triShape9, i, 4);
				else if (i == 9)
					turn(triShape10, i, 4);
				
			}
			if (triShapeNum(i, 4) >= 0.45f && i == 10 || triShapeNum(i, 4) >= 0.45f && i == 11) {
				right[i] = 0;
				if (i == 10)
					turn_(small1, i, 4);
				else if (i == 11)
					turn_(small2, i, 4);

			}
		}
		else if (up[i] == 0 && right[i] == 1)
		{
			if (i == 0)
				move(triShape1, speed[i], 2);
			else if (i == 1)
				move(triShape2, speed[i], 2);
			else if (i == 2)
				move(triShape3, speed[i], 2);
			else if (i == 3)
				move(triShape4, speed[i], 2);
			else if (i == 4)
				move(triShape5, speed[i], 2);
			else if (i == 5)
				move(triShape6, speed[i], 2);
			else if (i == 6)
				move(triShape7, speed[i], 2);
			else if (i == 7)
				move(triShape8, speed[i], 2);
			else if (i == 8)
				move(triShape9, speed[i], 2);
			else if (i == 9)
				move(triShape10, speed[i], 2);

			else if (i == 10)
				move(small1, speed[i], 2);
			else if (i == 11)
				move(small2, speed[i], 2);
			

			if (i <= 9 && triShapeNum(i, 3) >= -0.45f && triShapeNum(i, 3) <= 0.45f && triShapeNum(i, 4) >= -0.45f && triShapeNum(i, 4) <= 0.45f) {
				if (triShapeNum(i, 2) >= -1.0f && triShapeNum(i, 2) <= -0.45f) {
					right[i] = 0;
					if (i == 0)
						turn(triShape1, i, 4);
					else if (i == 1)
						turn(triShape2, i, 4);
					else if (i == 2)
						turn(triShape3, i, 4);
					else if (i == 3)
						turn(triShape4, i, 4);
					else if (i == 4)
						turn(triShape5, i, 4);
					else if (i == 5)
						turn(triShape6, i, 4);
					else if (i == 6)
						turn(triShape7, i, 4);
					else if (i == 7)
						turn(triShape8, i, 4);
					else if (i == 8)
						turn(triShape9, i, 4);
					else if (i == 9)
						turn(triShape10, i, 4);
				}
				else
				{
					up[i] = 1;
					if (i == 0)
						turn(triShape1, i, 3);
					else if (i == 1)
						turn(triShape2, i, 3);
					else if (i == 2)
						turn(triShape3, i, 3);
					else if (i == 3)
						turn(triShape4, i, 3);
					else if (i == 4)
						turn(triShape5, i, 3);
					else if (i == 5)
						turn(triShape6, i, 3);
					else if (i == 6)
						turn(triShape7, i, 3);
					else if (i == 7)
						turn(triShape8, i, 3);
					else if (i == 8)
						turn(triShape9, i, 3);
					else if (i == 9)
						turn(triShape10, i, 3);
				}
			}
			if (triShapeNum(i, 3) <= -1.0f) {
				up[i] = 1;
				if (i == 0)
					turn(triShape1, i, 3);
				else if (i == 1)
					turn(triShape2, i, 3);
				else if (i == 2)
					turn(triShape3, i, 3);
				else if (i == 3)
					turn(triShape4, i, 3);
				else if (i == 4)
					turn(triShape5, i, 3);
				else if (i == 5)
					turn(triShape6, i, 3);
				else if (i == 6)
					turn(triShape7, i, 3);
				else if (i == 7)
					turn(triShape8, i, 3);
				else if (i == 8)
					turn(triShape9, i, 3);
				else if (i == 9)
					turn(triShape10, i, 3);
				
			}
			if (triShapeNum(i, 3) <= -0.45f && i == 10 || triShapeNum(i, 3) <= -0.45f && i == 11) {
				up[i] = 1;
				if (i == 10)
					turn_(small1, i, 3);
				else if (i == 11)
					turn_(small2, i, 3);

			}
			if (triShapeNum(i, 4) >= 1.0f) {
				right[i] = 0;
				if (i == 0)
					turn(triShape1, i, 4);
				else if (i == 1)
					turn(triShape2, i, 4);
				else if (i == 2)
					turn(triShape3, i, 4);
				else if (i == 3)
					turn(triShape4, i, 4);
				else if (i == 4)
					turn(triShape5, i, 4);
				else if (i == 5)
					turn(triShape6, i, 4);
				else if (i == 6)
					turn(triShape7, i, 4);
				else if (i == 7)
					turn(triShape8, i, 4);
				else if (i == 8)
					turn(triShape9, i, 4);
				else if (i == 9)
					turn(triShape10, i, 4);

			}
			if (triShapeNum(i, 4) >= 0.45f && i == 10 || triShapeNum(i, 4) >= 0.45f && i == 11) {
				right[i] = 0;
				if (i == 10)
					turn_(small1, i, 4);
				else if (i == 11)
					turn_(small2, i, 4);

			}
		}
		else if (up[i] == 0 && right[i] == 0)
		{
			if (i == 0)
				move(triShape1, speed[i], 3);
			else if (i == 1)
				move(triShape2, speed[i], 3);
			else if (i == 2)
				move(triShape3, speed[i], 3);
			else if (i == 3)
				move(triShape4, speed[i], 3);
			else if (i == 4)
				move(triShape5, speed[i], 3);
			else if (i == 5)
				move(triShape6, speed[i], 3);
			else if (i == 6)
				move(triShape7, speed[i], 3);
			else if (i == 7)
				move(triShape8, speed[i], 3);
			else if (i == 8)
				move(triShape9, speed[i], 3);
			else if (i == 9)
				move(triShape10, speed[i], 3);

			else if (i == 10)
				move(small1, speed[i], 3);
			else if (i == 11)
				move(small2, speed[i], 3);
			

			if (i <= 9 && triShapeNum(i, 3) >= -0.45f && triShapeNum(i, 3) <= 0.45f && triShapeNum(i, 2) >= -0.45f && triShapeNum(i, 2) <= 0.45f) {
				if (triShapeNum(i, 4) <= 1.0f && triShapeNum(i, 4) >= 0.45f) {
					right[i] = 1;
					if (i == 0)
						turn(triShape1, i, 2);
					else if (i == 1)
						turn(triShape2, i, 2);
					else if (i == 2)
						turn(triShape3, i, 2);
					else if (i == 3)
						turn(triShape4, i, 2);
					else if (i == 4)
						turn(triShape5, i, 2);
					else if (i == 5)
						turn(triShape6, i, 2);
					else if (i == 6)
						turn(triShape7, i, 2);
					else if (i == 7)
						turn(triShape8, i, 2);
					else if (i == 8)
						turn(triShape9, i, 2);
					else if (i == 9)
						turn(triShape10, i, 2);
				}
				else
				{
					up[i] = 1;
					if (i == 0)
						turn(triShape1, i, 3);
					else if (i == 1)
						turn(triShape2, i, 3);
					else if (i == 2)
						turn(triShape3, i, 3);
					else if (i == 3)
						turn(triShape4, i, 3);
					else if (i == 4)
						turn(triShape5, i, 3);
					else if (i == 5)
						turn(triShape6, i, 3);
					else if (i == 6)
						turn(triShape7, i, 3);
					else if (i == 7)
						turn(triShape8, i, 3);
					else if (i == 8)
						turn(triShape9, i, 3);
					else if (i == 9)
						turn(triShape10, i, 3);
				}
			}
			if (triShapeNum(i, 3) <= -1.0f) {
				up[i] = 1;
				if (i == 0)
					turn(triShape1, i, 3);
				else if (i == 1)
					turn(triShape2, i, 3);
				else if (i == 2)
					turn(triShape3, i, 3);
				else if (i == 3)
					turn(triShape4, i, 3);
				else if (i == 4)
					turn(triShape5, i, 3);
				else if (i == 5)
					turn(triShape6, i, 3);
				else if (i == 6)
					turn(triShape7, i, 3);
				else if (i == 7)
					turn(triShape8, i, 3);
				else if (i == 8)
					turn(triShape9, i, 3);
				else if (i == 9)
					turn(triShape10, i, 3);
			}
			if (triShapeNum(i, 3) <= -0.45f && i == 10 || triShapeNum(i, 3) <= -0.45f && i == 11) {
				up[i] = 1;
				if (i == 10)
					turn_(small1, i, 3);
				else if (i == 11)
					turn_(small2, i, 3);
			}
			if (triShapeNum(i, 2) <= -1.0f) {
				right[i] = 1;
				if (i == 0)
					turn(triShape1, i, 2);
				else if (i == 1)
					turn(triShape2, i, 2);
				else if (i == 2)
					turn(triShape3, i, 2);
				else if (i == 3)
					turn(triShape4, i, 2);
				else if (i == 4)
					turn(triShape5, i, 2);
				else if (i == 5)
					turn(triShape6, i, 2);
				else if (i == 6)
					turn(triShape7, i, 2);
				else if (i == 7)
					turn(triShape8, i, 2);
				else if (i == 8)
					turn(triShape9, i, 2);
				else if (i == 9)
					turn(triShape10, i, 2);
			}
			if (triShapeNum(i, 2) <= -0.45f && i == 10 || triShapeNum(i, 2) <= -0.45f && i == 11) {
				right[i] = 1;
				if (i == 10)
					turn_(small1, i, 2);
				else if (i == 11)
					turn_(small2, i, 2);

			}
		}
		else if (up[i] == 1 && right[i] == 0)
		{
			if (i == 0)
				move(triShape1, speed[i], 4);
			else if (i == 1)
				move(triShape2, speed[i], 4);
			else if (i == 2)
				move(triShape3, speed[i], 4);
			else if (i == 3)
				move(triShape4, speed[i], 4);
			else if (i == 4)
				move(triShape5, speed[i], 4);
			else if (i == 5)
				move(triShape6, speed[i], 4);
			else if (i == 6)
				move(triShape7, speed[i], 4);
			else if (i == 7)
				move(triShape8, speed[i], 4);
			else if (i == 8)
				move(triShape9, speed[i], 4);
			else if (i == 9)
				move(triShape10, speed[i], 4);

			else if (i == 10)
				move(small1, speed[i], 4);
			else if (i == 11)
				move(small2, speed[i], 4);
			

			if (i <= 9 && triShapeNum(i, 1) >= -0.45f && triShapeNum(i, 1) <= 0.45f && triShapeNum(i, 2) >= -0.45f && triShapeNum(i, 2) <= 0.45f) {
				if (triShapeNum(i, 4) <= 1.0f && triShapeNum(i, 4) >= 0.45f) {
					right[i] = 1;
					if (i == 0)
						turn(triShape1, i, 2);
					else if (i == 1)
						turn(triShape2, i, 2);
					else if (i == 2)
						turn(triShape3, i, 2);
					else if (i == 3)
						turn(triShape4, i, 2);
					else if (i == 4)
						turn(triShape5, i, 2);
					else if (i == 5)
						turn(triShape6, i, 2);
					else if (i == 6)
						turn(triShape7, i, 2);
					else if (i == 7)
						turn(triShape8, i, 2);
					else if (i == 8)
						turn(triShape9, i, 2);
					else if (i == 9)
						turn(triShape10, i, 2);
				}
				else
				{
					up[i] = 0;
					if (i == 0)
						turn(triShape1, i, 1);
					else if (i == 1)
						turn(triShape2, i, 1);
					else if (i == 2)
						turn(triShape3, i, 1);
					else if (i == 3)
						turn(triShape4, i, 1);
					else if (i == 4)
						turn(triShape5, i, 1);
					else if (i == 5)
						turn(triShape6, i, 1);
					else if (i == 6)
						turn(triShape7, i, 1);
					else if (i == 7)
						turn(triShape8, i, 1);
					else if (i == 8)
						turn(triShape9, i, 1);
					else if (i == 9)
						turn(triShape10, i, 1);
				}
			}
			if (triShapeNum(i, 1) >= 1.0f) {
				up[i] = 0;
				if (i == 0)
					turn(triShape1, i, 1);
				else if (i == 1)
					turn(triShape2, i, 1);
				else if (i == 2)
					turn(triShape3, i, 1);
				else if (i == 3)
					turn(triShape4, i, 1);
				else if (i == 4)
					turn(triShape5, i, 1);
				else if (i == 5)
					turn(triShape6, i, 1);
				else if (i == 6)
					turn(triShape7, i, 1);
				else if (i == 7)
					turn(triShape8, i, 1);
				else if (i == 8)
					turn(triShape9, i, 1);
				else if (i == 9)
					turn(triShape10, i, 1);

			}
			if (triShapeNum(i, 1) >= 0.45f && i == 10 || triShapeNum(i, 1) >= 0.45f && i == 11) {
				up[i] = 0;
				if (i == 10)
					turn_(small1, i, 1);
				else if (i == 11)
					turn_(small2, i, 1);
			}
			if (triShapeNum(i, 2) <= -1.0f) {
				right[i] = 1;
				if (i == 0)
					turn(triShape1, i, 2);
				else if (i == 1)
					turn(triShape2, i, 2);
				else if (i == 2)
					turn(triShape3, i, 2);
				else if (i == 3)
					turn(triShape4, i, 2);
				else if (i == 4)
					turn(triShape5, i, 2);
				else if (i == 5)
					turn(triShape6, i, 2);
				else if (i == 6)
					turn(triShape7, i, 2);
				else if (i == 7)
					turn(triShape8, i, 2);
				else if (i == 8)
					turn(triShape9, i, 2);
				else if (i == 9)
					turn(triShape10, i, 2);
			}
			if (triShapeNum(i, 2) <= -0.45f && i == 10 || triShapeNum(i, 2) <= -0.45f && i == 11) {
				right[i] = 1;
				if (i == 10)
					turn_(small1, i, 2);
				else if (i == 11)
					turn_(small2, i, 2);
			}
		}
	}

	InitBuffer();

	glutPostRedisplay();
	glutTimerFunc(50, TimerFunction, 1);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:	//esc
		exit(0);
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Example8");

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	for (int i = 0; i < 12; ++i) {
		up[i] = uid(dre) >= 0.0 ? TRUE : FALSE;
		right[i] = uid(dre) >= 0.0 ? TRUE : FALSE;
		speed[i] = uid_(dre);
	}

	glutDisplayFunc(drawScene);
	glutTimerFunc(0, TimerFunction, 1);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}