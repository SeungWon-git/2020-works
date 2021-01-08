#pragma warning(disable: 4996)
#include <random>
#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>


std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid{ 0.01,0.05 };

//�ּ�
#define hexahedronpoints	36	//-> �� ���� ���� = �ﰢ��(������) ���� * 3(������)
#define pyramidpoints		18

GLfloat hexahedron_s[hexahedronpoints][3] = {};
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat linex_s[2][3] = { {-1.0,0.0,0.0},{1.0,0.0,0.0} };
GLfloat liney_s[2][3] = { {0.0,-1.0,0.0},{0.0,1.0,0.0} };
GLfloat linez_s[2][3] = { {0.0,0.0,-1.0},{0.0,0.0,1.0} };

//����
GLfloat hexahedron_c[hexahedronpoints][3] = {};
GLfloat pyramid_c[pyramidpoints][3] = {};
GLfloat linex_c[2][3] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };
GLfloat liney_c[2][3] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };
GLfloat linez_c[2][3] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };

//vao,vbo
GLuint ����_vao, hexahedron_vbo[2];
GLuint pyramid_vao, pyramid_vbo[2];
GLuint linex_vao, linex_vbo[2], liney_vao, liney_vbo[2], linez_vao, linez_vbo[2];

//���� ����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);			//ī�޶� ��ġ
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);	//ī�޶� �ٶ󺸴� ������
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//ī�޶��� ����� ����Ű�� ����

bool perspective = 1;
bool pyramid = 0;
bool stop_rup = 1, stop_rside = 1, stop_pside = 1;
bool stop_animation = 1;

GLfloat move_x = 0, move_y = -0.1f, move_z = -0.5f;	//�̵�

GLclampf change_size = 2.3f; //ũ�����

GLclampf c_s = 0.01f;	//����/��â ��ȯ

GLfloat RUP = 0.0f;		//����ü ���� ����
GLclampf rside = 0.0f;	//����ü ������ȯ
GLfloat RSIDE = 0.0f;	//����ü ���� ����
GLclampf pside = 0.0f;	//�簢�� ������ȯ
GLfloat PSIDE = 0.0f;	//�簢�� ���� ����

GLclampf r_y = 0.0f;	//���� ������ȯ
GLfloat RY = 0.0f;		//���� ����





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

void ReadObj(FILE* objFile, GLfloat(*obj)[3])
{
	using namespace glm;

	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
	char count[100];
	int vertexNum = 0;
	int faceNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // �迭 �ʱ�ȭ
	}
	rewind(objFile);	// �б� ��ġ �ٽ� �� ������

	//--- 2. �޸� �Ҵ�
	vec4* vertex;
	vec4* face;
	int vertIndex = 0;
	int faceIndex = 0;
	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
	face = (vec4*)malloc(sizeof(vec4) * faceNum);

	//--- 3. �Ҵ�� �޸𸮿� �� ���ؽ�, ���̽� ���� �Է�
	char bind[100];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f", &vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			//Ȯ�� -//std::cout << "v["<< vertIndex <<"] - "<<vertex[vertIndex].x << ", " << vertex[vertIndex].y << ", " << vertex[vertIndex].z << std::endl;
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%f%s%f%s%f", &face[faceIndex].x, &count, &face[faceIndex].y, &count, &face[faceIndex].z);
			//Ȯ�� -//std::cout  << "f[" << faceIndex << "] - "<<face[faceIndex].x << ", " << face[faceIndex].y << ", " << face[faceIndex].z << std::endl;
			faceIndex++;
		}
	}

	//--- 4. obj ���ؽ� �ּҿ� ����
	int j = 0;
	for (int i = 0; i < faceNum; ++i)
	{
		int num = face[i].x - 1;
		obj[j][0] = (GLfloat)vertex[num].x; obj[j][1] = vertex[num].y; obj[j][2] = vertex[num].z;
		++j;
		//std::cout << "obj[" << j - 1 << "] - " << obj[j - 1][0] << ", " << obj[j - 1][1] << ", " << obj[j - 1][2] << std::endl;
		num = face[i].y - 1;
		obj[j][0] = vertex[num].x; obj[j][1] = vertex[num].y; obj[j][2] = vertex[num].z;
		++j;
		//std::cout << "obj[" << j - 1 << "] - " << obj[j - 1][0] << ", " << obj[j - 1][1] << ", " << obj[j - 1][2] << std::endl;
		num = face[i].z - 1;
		obj[j][0] = vertex[num].x; obj[j][1] = vertex[num].y; obj[j][2] = vertex[num].z;
		++j;
		//std::cout << "obj[" << j - 1 << "] - " << obj[j - 1][0] << ", " << obj[j - 1][1] << ", " << obj[j - 1][2] << std::endl;
	}

	//--- 5.�޸� ��ȯ
	free(vertex);
	free(face);
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
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
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
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void InitShader()
{
	// obj�б�
	char filename[30] = "������ü.obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexahedron_s);

	strcpy(filename,"�簢��(�Ƕ�̵�).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, pyramid_s);

	fclose(file);

	// �� �ʱ�ȭ
	for (int i = 0; i < hexahedronpoints; ++i) {
		if (i < 6) {
			hexahedron_c[i][0] = 1.0f; hexahedron_c[i][1] = 0.0f; hexahedron_c[i][2] = 1.0f;//��-����
			if (i < 3) {
				pyramid_c[i][0] = 1.0f; pyramid_c[i][1] = 0.0f; pyramid_c[i][2] = 1.0f;
			}
			else {
				pyramid_c[i][0] = 0.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 0.0f;
			}
		}
		else if (i < 12) {
			hexahedron_c[i][0] = 0.0f; hexahedron_c[i][1] = 1.0f; hexahedron_c[i][2] = 0.0f;//��-�ʷ�
			if (i < 9) {
				pyramid_c[i][0] = 1.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 0.0f;
			}
			else {
				pyramid_c[i][0] = 0.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 1.0f;
			}
		}
		else if (i < 18) {
			hexahedron_c[i][0] = 0.0f; hexahedron_c[i][1] = 0.0f; hexahedron_c[i][2] = 1.0f;//��-�Ķ�
			if (i < 15) {
				pyramid_c[i][0] = 0.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 1.0f;
			}
			else {
				pyramid_c[i][0] = 1.0f; pyramid_c[i][1] = 0.0f; pyramid_c[i][2] = 0.0f;
			}
		}
		else if (i < 24) {
			hexahedron_c[i][0] = 1.0f; hexahedron_c[i][1] = 1.0f; hexahedron_c[i][2] = 0.0f;//����-���
		}
		else if (i < 30) {
			hexahedron_c[i][0] = 0.0f; hexahedron_c[i][1] = 1.0f; hexahedron_c[i][2] = 1.0f;//�Ʒ�-�ϴ�
		}
		else if (i < 36) {
			hexahedron_c[i][0] = 1.0f; hexahedron_c[i][1] = 0.0f; hexahedron_c[i][2] = 0.0f;//��-����
		}
	}

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

void InitVao(GLuint& vao, GLuint vbo[2], int num, GLfloat Shape[][3], GLfloat colors[][3]) {
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// ���� triShape ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	// Shape �迭�� ������: num * 3 *float
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), Shape, GL_STATIC_DRAW);
	// ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);

	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// ���� colors���� ���ؽ� ������ �����Ѵ�.
	// colors �迭�� ������: num * 3 *float
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	// ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);
}

void InitBuffer()
{
	InitVao(����_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexahedron_c);

	InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

	InitVao(linex_vao, linex_vbo, 2, linex_s, linex_c);
	InitVao(liney_vao, liney_vbo, 2, liney_s, liney_c);
	InitVao(linez_vao, linez_vbo, 2, linez_s, linez_c);
}



GLvoid drawScene()
{
	glClearColor(1.0, 1.0, 1.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);	// ���� ����

	//glEnable(GL_CULL_FACE);		// �޸�����

	unsigned int modelLoc = glGetUniformLocation(s_program, "model");	//--- ���ؽ� ���̴����� �𵨸� ��ȯ ��� �������� �޾ƿ´�.
	unsigned int viewLoc = glGetUniformLocation(s_program, "view");		//--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.
	unsigned int projLoc = glGetUniformLocation(s_program, "projection");//--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.
	glm::mat4 mTransform = glm::mat4(1.0f);		  // ������ķ� �ʱ�ȭ
	glm::mat4 vTransform = glm::mat4(1.0f);		  // ������ķ� �ʱ�ȭ
	glm::mat4 pTransform = glm::mat4(1.0f);		  // ������ķ� �ʱ�ȭ

	mTransform = glm::mat4(1.0f);	//�� ��ȯ
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	vTransform = glm::mat4(1.0f);	// ī�޶� ��ȯ
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLoc = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	pTransform = glm::mat4(1.0f);	// ���� ��ȯ
	if (perspective == 1)	//���� ����
		pTransform = glm::perspective(glm::radians(45.0f), window_w / window_h, 0.1f, 50.0f);
	else					//���� ����
		pTransform = glm::ortho(-2.0, 2.0, -2.0, 2.0, 0.1, 50.0);
	projLoc = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	//x,y,z��
	glBindVertexArray(linex_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(liney_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(linez_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	//����
	if (pyramid == 0) {	//����ü
		glBindVertexArray(����_vao);

		glDrawArrays(GL_TRIANGLES, 0, 12);	//0��~11������	
		glDrawArrays(GL_TRIANGLES, 18, 12);	//18��~29������

		{//���� ȸ�� �ִϸ��̼� (�߿�- ȸ���� ������ x,y,z�� �߽����ιۿ� ���ϹǷ� ���ð��� ����(���ϴ� �࿡ ����)���༭ ȸ����Ų�� �ٽ� ����ġ�� ������Ѵ�.)
			for (int i = 12; i < 18; ++i)	//�ش� �ε����� ���ð� ����(y���� 0����-> x�࿡ ȸ���Ұ��̱� ������)
				hexahedron_s[i][1] -= 0.25;

			InitVao(����_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexahedron_c);	//vao ������Ʈ

			mTransform = glm::translate(mTransform, glm::vec3(0.0, 0.25, 0.0));	//����ü ���� �̵�(������ ���� �̵��ϰԵ� - ���ڸ��� ��ġ�ϴ� ��Ȱ)
			mTransform = glm::rotate(mTransform, glm::radians(RUP), glm::vec3(1.0, 0.0, 0.0));	//���� x�࿡���� ȸ��
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 12, 6);	//12��~17������(����)

			for (int i = 12; i < 18; ++i)	//�ش� �ε����� ���ð� ���󺹱� - Ȥ�� �Ŀ� �� ��ǥ�� �׳� ������ �ֱ� ������
				hexahedron_s[i][1] += 0.25;
		}

		{//���� ȸ�� �ִϸ��̼�
			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] += 0.25; hexahedron_s[i][2] -= 0.25;
			}

			InitVao(����_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexahedron_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0.25));
			mTransform = glm::rotate(mTransform, glm::radians(RSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 30, 6);	//30��~35������(����)

			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] -= 0.25; hexahedron_s[i][2] += 0.25;
			}
		}
	}
	else {	//�簢��(�Ƕ�̵�)
		glBindVertexArray(pyramid_vao);

		glDrawArrays(GL_TRIANGLES, 9, 6);	//9��~14������

		{//���� ȸ�� �ִϸ��̼�
			for (int i = 0; i < 3; ++i) {
				pyramid_s[i][1] += 0.15; pyramid_s[i][2] += 0.25;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.15, -0.25));
			mTransform = glm::rotate(mTransform, glm::radians(-PSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 3);	//0��~2������

			for (int i = 0; i < 3; ++i) {
				pyramid_s[i][1] -= 0.15; pyramid_s[i][2] -= 0.25;
			}
		}
		{//���� ȸ�� �ִϸ��̼�
			for (int i = 3; i < 6; ++i) {
				pyramid_s[i][0] += 0.25; pyramid_s[i][1] += 0.15;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(-0.25, -0.15, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(PSIDE), glm::vec3(0.0, 0.0, 1.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 3, 3);	//3��~5������

			for (int i = 3; i < 6; ++i) {
				pyramid_s[i][0] -= 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//���� ȸ�� �ִϸ��̼�
			for (int i = 6; i < 9; ++i) {
				pyramid_s[i][0] -= 0.25; pyramid_s[i][1] += 0.15;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.25, -0.15, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(-PSIDE), glm::vec3(0.0, 0.0, 1.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 6, 3);	//6��~8������

			for (int i = 6; i < 9; ++i) {
				pyramid_s[i][0] += 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//���� ȸ�� �ִϸ��̼�
			for (int i = 15; i < 18; ++i) {
				pyramid_s[i][1] += 0.15; pyramid_s[i][2] -= 0.25;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.15, 0.25));
			mTransform = glm::rotate(mTransform, glm::radians(PSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 15, 3);	//15��~17������

			for (int i = 15; i < 18; ++i) {
				pyramid_s[i][1] -= 0.15; pyramid_s[i][2] += 0.25;
			}
		}

	}


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerRotate_RUP(int value) {
	if (stop_animation == 0 && stop_rup != 1) {

		RUP += 1.0f;

		glutPostRedisplay();
		glutTimerFunc(30, TimerRotate_RUP, 1);
	}
}

void TimerRotate_RSIDE(int value) {
	if (stop_animation == 0 && stop_rside != 1) {

		RSIDE += rside;

		if (RSIDE == 60)
			rside = -1.0f;
		else if (RSIDE == 0)
			rside = 1.0f;

		glutPostRedisplay();
		glutTimerFunc(20, TimerRotate_RSIDE, 1);
	}
}

void TimerRotate_PSIDE(int value) {
	if (stop_animation == 0 && stop_pside != 1) {

		PSIDE += pside;

		if (PSIDE == 180)
			pside = -1.0f;
		else if (PSIDE == 0)
			pside = 1.0f;

		glutPostRedisplay();
		glutTimerFunc(30, TimerRotate_PSIDE, 1);
	}
}

void Timer_Rotate_Model_Y(int value)
{
	if (stop_animation == 0 && r_y != 0) {

		RY += r_y;

		if (RY == 360.0f)
			RY = 0.0f;
		if (RY == 0.0f)
			RY = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(30, Timer_Rotate_Model_Y, 1);
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
	case 'p':
	case 'P':
		if (perspective == 1)
			perspective = 0;
		else perspective = 1;
		break;

	case 'a':
		move_x -= 0.05;
		break;
	case 'd':
		move_x += 0.05;
		break;
	case 'w':
		move_y += 0.05;
		break;
	case 's':
		move_y -= 0.05;
		break;
	case 'z':
		move_z -= 0.05;
		break;
	case 'Z':
		move_z += 0.05;
		break;

	case 'y':
	case 'Y':
		stop_animation = 0;
		if (r_y != -1.0f) {
			r_y = -1.0f;
			glutTimerFunc(0, Timer_Rotate_Model_Y, 1);
		}
		else 
			r_y = 0.0f;
		break;
	case 't':
	case 'T':
		stop_animation = 0;
		if (stop_rup == 1) {
			stop_rup = 0;
			glutTimerFunc(0, TimerRotate_RUP, 1);
		}
		else 
			stop_rup = 1;
		break;
	case 'f':
	case 'F':
		stop_animation = 0;
		if (stop_rside == 1) {
			stop_rside = 0;
			glutTimerFunc(0, TimerRotate_RSIDE, 1);
		}
		else
			stop_rside = 1;
		break;
	case 'o':
	case 'O':
		stop_animation = 0;
		if (stop_pside == 1) {
			stop_pside = 0;
			glutTimerFunc(0, TimerRotate_PSIDE, 1);
		}
		else
			stop_pside = 1;
		break;

	case 'S':
		stop_animation = 0;
		glutTimerFunc(0, TimerChangeSize, 1);
		break;
	case 'C':	//�ʱ� ����������
		stop_animation = 1;
		move_x = 0; move_y = -0.1f; move_z = -0.5f;
		RUP = 0.0f; RSIDE = 0.0f; rside = 0; PSIDE = 0.0f; pside = 0;
		RY = 0.0f; r_y = 0;
		stop_rup = 1; stop_rside = 1; stop_pside = 1;
		change_size = 2.3f;
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
	if(b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		if (pyramid == 0)
			pyramid = 1;
		else pyramid = 0;	
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example16");

	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}