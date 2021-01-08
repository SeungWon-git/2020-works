#pragma warning(disable: 4996)
#include <random>
#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid{ 0.01,0.05 };
std::uniform_real_distribution<> uid_loc{ -1.4,1.4 };

//s
#define hexahedronpoints	36	//-> �� ���� ���� = �ﰢ��(������) ���� * 3(������)
#define pyramidpoints		18

GLfloat hexa_s[hexahedronpoints][3] = {};
GLfloat floor_s[12][3] = {
	{ -0.25,-0.25,-0.25 }, { 0.0,-0.25,0.0 }, {0.25,-0.25,-0.25},	//��
	{ 0.25,-0.25,-0.25 }, { 0.0,-0.25,0.0 }, {0.25,-0.25,0.25},		//��
	{ 0,-0.25,0.0 }, { -0.25,-0.25,0.25 }, {0.25,-0.25,0.25},		//��
	{-0.25,-0.25,-0.25},{-0.25,-0.25,0.25},{0,-0.25,0}				//��
};
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat back_s[6][3] = {
	{2.5,2.5,-3} ,{-2.5,-2.5,-3}, {-2.5,2.5,-3},
	{-2.5,-2.5,-3} ,{2.5,2.5,-3}, {2.5,-2.5,-3}
};


//vn
GLfloat hexa_vn[hexahedronpoints][3] = {};
GLfloat floor_vn[12][3] = {
	{0,1,0},{0,1,0},{0,1,0},
	{0,1,0},{0,1,0},{0,1,0},
	{0,1,0},{0,1,0},{0,1,0},
	{0,1,0},{0,1,0},{0,1,0}
};
GLfloat pyramid_vn[pyramidpoints][3] = {};
GLfloat back_vn[6][3] = {
	{0,0,1} ,{0,0,1}, {0,0,1},
	{0,0,1} ,{0,0,1}, {0,0,1}
};

//vt
GLfloat hexa_vt[hexahedronpoints][2] = {};
GLfloat floor_vt[12][2] = {
	{0,0},{1,1},{0,0},{1,1},
	{0,0},{1,1},{0,0},{1,1},
	{0,0},{1,1},{0,0},{1,1}
};
GLfloat pyramid_vt[pyramidpoints][2] = {};
GLfloat back_vt[6][2] = {
	{1,1} ,{0,0}, {0,1},
	{0,0} ,{1,1}, {1,0}
};


//vao,vbo
GLuint ����_vao, floor_vao, robot_h_vao[5], robot_b_vao[5], robot_Larm_vao[5], robot_Rarm_vao[5], robot_Lleg_vao[5], robot_Rleg_vao[5], robot_nose_vao[5], back_vao,
����_vbo[2], floor_vbo[2], robot_h_vbo[2], robot_b_vbo[2], robot_Larm_vbo[2], robot_Rarm_vbo[2], robot_Lleg_vbo[2], robot_Rleg_vbo[2], robot_nose_vbo[2], back_vbo[3];

//���� ����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 2.3f);			//ī�޶� ��ġ 
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);	//ī�޶� �ٶ󺸴� ������
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//ī�޶��� ����� ����Ű�� ����

bool perspective = 1;
int perspective1_3 = 3;
bool stop_animation = 1;

GLfloat frontscreen_y = 0, frontscreen_r = 0;	//�� �帷 �ȱ�

GLfloat move_x[5], move_y[5], move_z[5];	//�� �̵�
GLfloat direction[5] = { 360.0f,360.0f,360.0f,360.0f,360.0f };//���� �ٶ󺸴� ����[����] ( 0 -> ��, -90 -> ��, 90 -> ��, 180 -> �� )
GLfloat walk[5] = {0,0,0,0,0 };	//�� �ȱ� ����

GLfloat cp_cw = 0.0f;		//ī�޶��� ����
GLfloat CP_Y = 0.0f, MP_Y = 0.0f;		//ī�޶��� ����
GLfloat cd_cw = 0.0f;		//ī�޶� �ٶ󺸴� ����
GLfloat CD_Y = 270.0f;		//ī�޶� �ٶ󺸴� ����

bool follow[4] = { 0, };	//������� ����
bool capture[4] = { 0, };	//�ѹ� ������ ��� ������� ����

// �̹���
unsigned int texture_bg, texture_black, texture_white, texture_grey, texture_red, texture_peach, texture_orange, texture_purple, texture_lightpurple, texture_blue, texture_brown, texture_mint, texture_lightgreen, texture_pink, texture_skyblue, texture_darkwhite;


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

void ReadObj(FILE* objFile, GLfloat(*obj_v)[3], GLfloat(*obj_vn)[3], GLfloat(*obj_vt)[2])
{
	using namespace glm;

	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
	char count[100];
	int vertexNum = 0; int vertexnormalNum = 0; int vertextextureNum = 0;
	int faceNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'v' && count[1] == 'n' && count[2] == '\0')
			vertexnormalNum += 1;
		else if (count[0] == 'v' && count[1] == 't' && count[2] == '\0')
			vertextextureNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // �迭 �ʱ�ȭ
	}
	rewind(objFile);	// �б� ��ġ �ٽ� �� ������

	//--- 2. �޸� �Ҵ�
	vec4* vertex, * vertexnormal, * vertextexture;
	vec4* face_v, * face_vn, * face_vt;
	int vertIndex = 0; int vertnormalIndex = 0; int verttextureIndex = 0;
	int faceIndex = 0;

	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
	vertexnormal = (vec4*)malloc(sizeof(vec4) * vertexnormalNum);
	vertextexture = (vec4*)malloc(sizeof(vec4) * vertextextureNum);
	face_v = (vec4*)malloc(sizeof(vec4) * faceNum);
	face_vn = (vec4*)malloc(sizeof(vec4) * faceNum);
	face_vt = (vec4*)malloc(sizeof(vec4) * faceNum);

	//--- 3. �Ҵ�� �޸𸮿� �� ���ؽ�, ���̽� ���� �Է�
	char bind[100];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f", &vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			//Ȯ�� -//std::cout << "v["<< vertIndex <<"] - "<<vertex[vertIndex].x << ", " << vertex[vertIndex].y << ", " << vertex[vertIndex].z << std::endl;
			vertIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0') {
			fscanf(objFile, "%f %f %f", &vertexnormal[vertnormalIndex].x, &vertexnormal[vertnormalIndex].y, &vertexnormal[vertnormalIndex].z);
			//std::cout << "vn[" << vertnormalIndex << "] - " << vertexnormal[vertnormalIndex].x << ", " << vertexnormal[vertnormalIndex].y << ", " << vertexnormal[vertnormalIndex].z << std::endl;
			vertnormalIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 't' && bind[2] == '\0') {
			fscanf(objFile, "%f %f", &vertextexture[verttextureIndex].x, &vertextexture[verttextureIndex].y);
			verttextureIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%f/%f/%f %f/%f/%f %f/%f/%f", &face_v[faceIndex].x, &face_vt[faceIndex].x, &face_vn[faceIndex].x, &face_v[faceIndex].y, &face_vt[faceIndex].y, &face_vn[faceIndex].y, &face_v[faceIndex].z, &face_vt[faceIndex].z, &face_vn[faceIndex].z);
			//std::cout  << "f[" << faceIndex << "] - "<<face_v[faceIndex].x << "/" << NULL << "/" << face_vn[faceIndex].x << " " << face_v[faceIndex].y << "/" << NULL << "/" << face_vn[faceIndex].y << " " << face_v[faceIndex].z << "/" << NULL << "/" << face_vn[faceIndex].z << std::endl;
			faceIndex++;
		}
	}

	//--- 4. obj ���ؽ� �ּҿ� ����
	//�ּҰ�
	int j = 0;
	for (int i = 0; i < faceNum; ++i)
	{
		int num = face_v[i].x - 1;
		obj_v[j][0] = (GLfloat)vertex[num].x; obj_v[j][1] = vertex[num].y; obj_v[j][2] = vertex[num].z;
		++j;
		//std::cout << "obj_v[" << j - 1 << "] - " << obj_v[j - 1][0] << ", " << obj_v[j - 1][1] << ", " << obj_v[j - 1][2] << std::endl;
		num = face_v[i].y - 1;
		obj_v[j][0] = vertex[num].x; obj_v[j][1] = vertex[num].y; obj_v[j][2] = vertex[num].z;
		++j;
		//std::cout << "obj_v[" << j - 1 << "] - " << obj_v[j - 1][0] << ", " << obj_v[j - 1][1] << ", " << obj_v[j - 1][2] << std::endl;
		num = face_v[i].z - 1;
		obj_v[j][0] = vertex[num].x; obj_v[j][1] = vertex[num].y; obj_v[j][2] = vertex[num].z;
		++j;
		//std::cout << "obj_v[" << j - 1 << "] - " << obj_v[j - 1][0] << ", " << obj_v[j - 1][1] << ", " << obj_v[j - 1][2] << std::endl;
	}
	//������
	j = 0;
	for (int i = 0; i < faceNum; ++i)
	{
		int num = face_vn[i].x - 1;
		obj_vn[j][0] = (GLfloat)vertexnormal[num].x; obj_vn[j][1] = vertexnormal[num].y; obj_vn[j][2] = vertexnormal[num].z;
		++j;
		//std::cout << "obj_vn[" << j - 1 << "] - " << obj_vn[j - 1][0] << ", " << obj_vn[j - 1][1] << ", " << obj_vn[j - 1][2] << std::endl;
		num = face_vn[i].y - 1;
		obj_vn[j][0] = vertexnormal[num].x; obj_vn[j][1] = vertexnormal[num].y; obj_vn[j][2] = vertexnormal[num].z;
		++j;
		//std::cout << "obj_vn[" << j - 1 << "] - " << obj_vn[j - 1][0] << ", " << obj_vn[j - 1][1] << ", " << obj_vn[j - 1][2] << std::endl;
		num = face_vn[i].z - 1;
		obj_vn[j][0] = vertexnormal[num].x; obj_vn[j][1] = vertexnormal[num].y; obj_vn[j][2] = vertexnormal[num].z;
		++j;
		//std::cout << "obj_vn[" << j - 1 << "] - " << obj_vn[j - 1][0] << ", " << obj_vn[j - 1][1] << ", " << obj_vn[j - 1][2] << std::endl;
	}
	//�ؽ�ó ��
	j = 0;
	for (int i = 0; i < faceNum; ++i)
	{
		int num = face_vt[i].x - 1;
		obj_vt[j][0] = (GLfloat)vertextexture[num].x; obj_vt[j][1] = vertextexture[num].y;
		++j;
		num = face_vt[i].y - 1;
		obj_vt[j][0] = vertextexture[num].x; obj_vt[j][1] = vertextexture[num].y;
		++j;
		num = face_vt[i].z - 1;
		obj_vt[j][0] = vertextexture[num].x; obj_vt[j][1] = vertextexture[num].y;
		++j;
	}

	//--- 5.�޸� ��ȯ
	free(vertex);
	free(vertexnormal);
	free(vertextexture);
	free(face_v);
	free(face_vn);
	free(face_vt);
}


void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

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

	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

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

// �븻���� �ֱ������� �����׸�Ʈ���� ���X (���� ���ÿ� ���)
void InitVao(GLuint& vao, GLuint vbo[3], int num, GLfloat Shape[][3], GLfloat Normal[][3], GLfloat TextureCoord[][2]) {
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(3, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

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
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), Normal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//--- 3��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (�ؽ�ó)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), TextureCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
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
		ReadObj(file, hexa_s,hexa_vn,hexa_vt);

	strcpy(filename, "�簢��(�Ƕ�̵�).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, pyramid_s,pyramid_vn,pyramid_vt);

	fclose(file);	//���ϴݱ�

	//�κ� ȭ��� ��ġ ����
	move_x[0] = 0; move_y[0] = -0.17f; move_z[0] = 0;
	for (int i = 1; i < 5; ++i) {
		move_x[i] = uid_loc(dre); move_y[i] = -0.17f; move_z[i] = uid_loc(dre);
	}


	//�� ��ġ ����
	//����,�ٴ�,���,�κ� �Ӹ�
	InitVao(����_vao, ����_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);
	InitVao(floor_vao, floor_vbo, 12, floor_s, floor_vn,floor_vt);
	InitVao(back_vao, back_vbo, 6, back_s, back_vn, back_vt);

	for (int i = 0; i < 5; ++i) {
		if (i == 0) {
			InitVao(robot_h_vao[i], robot_h_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//��
			for (int i = 0; i < pyramidpoints; ++i) {
				pyramid_s[i][1] += 0.25;
			}
			InitVao(robot_nose_vao[i], robot_nose_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

			//����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][1] -= 0.38;
			}
			InitVao(robot_b_vao[i], robot_b_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//����-���ֻ�
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] -= 1.5; hexa_s[i][1] -= 0.21;
			}
			InitVao(robot_Larm_vao[i], robot_Larm_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//������-�����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] += 3.0;
			}
			InitVao(robot_Rarm_vao[i], robot_Rarm_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//�޴ٸ�-����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] -= 1.5; hexa_s[i][0] += 0.7;
			}
			InitVao(robot_Lleg_vao[i], robot_Lleg_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//�����ٸ�-�Ķ���
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] -= 1.4;
			}
			InitVao(robot_Rleg_vao[i], robot_Rleg_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//���� ����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] += 0.7; hexa_s[i][1] += 0.59;
			}
			for (int i = 0; i < pyramidpoints; ++i) {
				pyramid_s[i][1] -= 0.25;
			}
		}
		else
		{
			InitVao(robot_h_vao[i], robot_h_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//��
			for (int i = 0; i < pyramidpoints; ++i) {
				pyramid_s[i][1] += 0.25;
			}
			InitVao(robot_nose_vao[i], robot_nose_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

			//����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][1] -= 0.38;
			}
			InitVao(robot_b_vao[i], robot_b_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//����-���ֻ�
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] -= 1.5; hexa_s[i][1] -= 0.21;
			}
			InitVao(robot_Larm_vao[i], robot_Larm_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//������-�����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] += 3.0;
			}
			InitVao(robot_Rarm_vao[i], robot_Rarm_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//�޴ٸ�-����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] -= 1.5; hexa_s[i][0] += 0.7;
			}
			InitVao(robot_Lleg_vao[i], robot_Lleg_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//�����ٸ�-�Ķ���
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] -= 1.4;
			}
			InitVao(robot_Rleg_vao[i], robot_Rleg_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);

			//���� ����
			for (int i = 0; i < hexahedronpoints; ++i) {
				hexa_s[i][0] += 0.7; hexa_s[i][1] += 0.59;
			}
			for (int i = 0; i < pyramidpoints; ++i) {
				pyramid_s[i][1] -= 0.25;
			}
		}
	}

	//���̴� ó��
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
}

void InitTexture() {
	BITMAPINFO* bmp;
	int widthImage, heightImage, numberOfChannel;
	
	//���
	glGenTextures(1, &texture_bg); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_bg); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	unsigned char* data = stbi_load("res/sky.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//����
	glGenTextures(1, &texture_brown); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_brown); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/����.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//������
	glGenTextures(1, &texture_black); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_black); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/������.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//���������
	glGenTextures(1, &texture_darkwhite); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_darkwhite); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/���������.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//�����
	glGenTextures(1, &texture_purple); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_purple); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/�����.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//������
	glGenTextures(1, &texture_red); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_red); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/������.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//�챸��
	glGenTextures(1, &texture_peach); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_peach); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/�챸��.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//���λ�
	glGenTextures(1, &texture_lightgreen); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_lightgreen); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/���λ�.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//���ֻ�
	glGenTextures(1, &texture_lightpurple); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_lightpurple); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/���ֻ�.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//��Ȳ��
	glGenTextures(1, &texture_orange); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_orange); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/��Ȳ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//�Ķ���
	glGenTextures(1, &texture_blue); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_blue); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/�Ķ���.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//��ũ��
	glGenTextures(1, &texture_pink); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_pink); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/��ũ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//�ϴû�
	glGenTextures(1, &texture_skyblue); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_skyblue); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/�ϴû�.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//ȸ��
	glGenTextures(1, &texture_grey); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_grey); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/ȸ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//��Ʈ��
	glGenTextures(1, &texture_mint); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_mint); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/��Ʈ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//���
	glGenTextures(1, &texture_white); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_white); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("res/���.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	glUseProgram(s_program);
	int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����

	stbi_image_free(data);	//---�޸� ��ȯ
}


GLvoid drawScene()
{
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);	// ���� ����
	glFrontFace(GL_CW);			// �ð������ �ո�
	glEnable(GL_CULL_FACE);		// �޸�(�ð�ݴ�������� winding�� �ո�) ����



	unsigned int modelLoc = glGetUniformLocation(s_program, "model");	//--- ���ؽ� ���̴����� �𵨸� ��ȯ ��� �������� �޾ƿ´�.
	unsigned int viewLoc = glGetUniformLocation(s_program, "view");		//--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.
	unsigned int projLoc = glGetUniformLocation(s_program, "projection");//--- ���ؽ� ���̴����� ���� ��ȯ ��� �������� �޾ƿ´�.
	glm::mat4 mTransform = glm::mat4(1.0f);		  // ������ķ� �ʱ�ȭ
	glm::mat4 vTransform = glm::mat4(1.0f);		  // ������ķ� �ʱ�ȭ
	glm::mat4 pTransform = glm::mat4(1.0f);		  // ������ķ� �ʱ�ȭ


	vTransform = glm::mat4(1.0f);	// ī�޶� ��ȯ
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLoc = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	pTransform = glm::mat4(1.0f);	// ���� ��ȯ
	if (perspective == 1)	//���� ����
		pTransform = glm::perspective(glm::radians(70.0f), window_w / window_h, 0.1f, 50.0f);
	else					//���� ����
		pTransform = glm::ortho(-2.0, 2.0, -2.0, 2.0, 0.1, 50.0);
	projLoc = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	{//���
		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		mTransform = glm::rotate(mTransform, glm::radians(MP_Y), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::scale(mTransform, glm::vec3(1.5, 1.5, 1.0));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_bg);

		glBindVertexArray(back_vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	{//����
		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		mTransform = glm::scale(mTransform, glm::vec3(6.0, 1.5, 6.0));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindVertexArray(����_vao);

		glBindTexture(GL_TEXTURE_2D, texture_white);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, texture_darkwhite);
		glDrawArrays(GL_TRIANGLES, 6, 6);
		//���� �ȱ׸���
		glBindTexture(GL_TEXTURE_2D, texture_darkwhite);
		glDrawArrays(GL_TRIANGLES, 18, 6);
		glBindTexture(GL_TEXTURE_2D, texture_white);
		glDrawArrays(GL_TRIANGLES, 30, 6);	//�ٴڸ� �� �׸���
	}

	{//�ٴڸ�
		glFrontFace(GL_CCW);			// �ݽð������ �ո�(����ġ)

		glBindVertexArray(floor_vao);
		//��
		glBindTexture(GL_TEXTURE_2D, texture_lightgreen);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//��
		glBindTexture(GL_TEXTURE_2D, texture_orange);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		//��
		glBindTexture(GL_TEXTURE_2D, texture_mint);
		glDrawArrays(GL_TRIANGLES, 6, 3);
		//��
		glBindTexture(GL_TEXTURE_2D, texture_pink);
		glDrawArrays(GL_TRIANGLES, 9, 3);
	}

	if (frontscreen_r > -90)
	{//�ո�
		mTransform = glm::rotate(mTransform, glm::radians(frontscreen_r), glm::vec3(1.0, 0.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0, frontscreen_y, 0));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, texture_white);

		glBindVertexArray(����_vao);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	}

	glDisable(GL_CULL_FACE);		// �޸�(�ð�ݴ�������� winding�� �ո�) ���� ����

	for (int i = 0; i < 5; ++i)
	{//�κ�
		if (i == 0) {	//���ΰ�
			{	//�Ӹ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.1, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_peach);

				glBindVertexArray(robot_h_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{	//��
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.1, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_brown);

				glBindVertexArray(robot_nose_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);
			}
			{	//����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.25, 0.2, 0.25));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_red);

				glBindVertexArray(robot_b_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{	//����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_purple);

				glBindVertexArray(robot_Larm_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				//�޴ٸ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.06, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_blue);

				glBindVertexArray(robot_Lleg_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{	//������
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_lightpurple);

				glBindVertexArray(robot_Rarm_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				//�����ٸ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.06, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_skyblue);

				glBindVertexArray(robot_Rleg_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
		}
		else
		{	//������
			{	//�Ӹ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.1, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_white);

				glBindVertexArray(robot_h_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{	//��
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.1, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_red);

				glBindVertexArray(robot_nose_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);
			}
			{	//����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.25, 0.2, 0.25));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_grey);

				glBindVertexArray(robot_b_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{	//����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_white);

				glBindVertexArray(robot_Larm_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				//�޴ٸ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.06, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_black);

				glBindVertexArray(robot_Lleg_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{	//������
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_white);

				glBindVertexArray(robot_Rarm_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				//�����ٸ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.06, 0));
				mTransform = glm::translate(mTransform, glm::vec3(move_x[i], move_y[i], move_z[i]));
				mTransform = glm::rotate(mTransform, glm::radians(direction[i]), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(walk[i]), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, texture_black);

				glBindVertexArray(robot_Rleg_vao[i]);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
		}

	}



	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerOpen_Screen(int value)	//�帷����
{
	if (stop_animation == 0) {
		if (value == 1) {
			if (frontscreen_y < 0.5) {
				frontscreen_y += 0.01;

				glutPostRedisplay();
				glutTimerFunc(30, TimerOpen_Screen, 1);
			}
			else {
				glutTimerFunc(0, TimerOpen_Screen, 2);
			}
		}
		else if (value == 2) {
			if (frontscreen_r > -90) {
				frontscreen_r -= 1;

				glutPostRedisplay();
				glutTimerFunc(20, TimerOpen_Screen, 2);
			}
			else {
				glutTimerFunc(0, TimerOpen_Screen, 0);	//����
			}
		}
	}
}

void Timer_Move_ahead_main(int value) {	//���ΰ��κ� ������ �̵�
	if (stop_animation == 0) {
		//3��Ī
		if (perspective1_3 == 3) {
			for (int i = 0; i < 1; ++i) {
				if (direction[i] == 0) {//��
					move_z[i] += 0.01;

					if (move_z[i] > 1.45)
						direction[i] = 180;
				}
				else if (direction[i] == 180) {	//��
					move_z[i] -= 0.01;

					if (move_z[i] < -1.45)
						direction[i] = 0;
				}
				else if (direction[i] == -90) {	//��
					move_x[i] -= 0.01;

					if (move_x[i] < -1.45)
						direction[i] = 90;
				}
				else if (direction[i] == 90) {	//��
					move_x[i] += 0.01;

					if (move_x[i] > 1.45)
						direction[i] = -90;
				}
			}
		}
		//1��Ī
		else if (perspective1_3 == 1) {
			for (int i = 0; i < 1; ++i) {
				if (direction[i] == 0) {//��
					move_z[i] += 0.01;
					cameraPos.z = move_z[i];
					cameraDirection.z += 0.01;

					if (move_z[i] > 1.45) {
						direction[i] = 180;
						CD_Y = 270;
						cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y)) + move_x[0];
						cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + move_z[0];
					}
				}
				else if (direction[i] == 180) {	//��
					move_z[i] -= 0.01;
					cameraPos.z = move_z[i];
					cameraDirection.z -= 0.01;

					if (move_z[i] < -1.45) {
						direction[i] = 0;
						CD_Y = 90;
						cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y)) + move_x[0];
						cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + move_z[0];
					}
				}
				else if (direction[i] == -90) {	//��
					move_x[i] -= 0.01;
					cameraPos.x = move_x[i]; 
					cameraDirection.x -= 0.01;

					if (move_x[i] < -1.45) {
						direction[i] = 90;
						CD_Y = 0;
						cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y)) + move_x[0];
						cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + move_z[0];
					}
				}
				else if (direction[i] == 90) {	//��
					move_x[i] += 0.01;
					cameraPos.x = move_x[i];
					cameraDirection.x += 0.01;

					if (move_x[i] > 1.45) {
						direction[i] = -90;
						CD_Y = 180;
						cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y)) + move_x[0];
						cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + move_z[0];
					}
				}
			}
		}
		
		//������� �ϱ�
		GLfloat dist = 0;
		GLfloat x,z;

		for (int i = 1; i < 5; ++i) {
			x = move_x[0] - move_x[i];
			z = move_z[0] - move_z[i];
			dist = sqrt(x * x + z * z);

			if (dist <= 0.1) {
				if (capture[i - 1] == 0)
					cout << "captured: " << i - 1 << endl;
				capture[i - 1] = 1;
			}
			else if (dist <= 0.5)
				follow[i - 1] = 1;
			else {
				GLfloat a = uid(dre);

				if (follow[i - 1] == 1) {
					if (a <= 0.02)
						direction[i] = 0;
					else if (a <= 0.03)
						direction[i] = 180;
					else if (a <= 0.04)
						direction[i] = -90;
					else if (a <= 0.05)
						direction[i] = 90;
				}

				follow[i - 1] = 0;
			}
		}


		glutPostRedisplay();
		glutTimerFunc(20, Timer_Move_ahead_main, 1);
	}
}

void Timer_Move_ahead_1(int value) {	//��1 ������ �̵�
	if (stop_animation == 0 && follow[0] != 1 && capture[0] != 1) {
		for (int i = 1; i < 2; ++i) {
			if (direction[i] == 0) {//��
				move_z[i] += 0.006;

				if (move_z[i] > 1.45)
					direction[i] = 180;
			}
			else if (direction[i] == 180) {	//��
				move_z[i] -= 0.006;

				if (move_z[i] < -1.45)
					direction[i] = 0;
			}
			else if (direction[i] == -90) {	//��
				move_x[i] -= 0.006;

				if (move_x[i] < -1.45)
					direction[i] = 90;
			}
			else if (direction[i] == 90) {	//��
				move_x[i] += 0.006;

				if (move_x[i] > 1.45)
					direction[i] = -90;
			}

			glutPostRedisplay();
			glutTimerFunc(20, Timer_Move_ahead_1, 1);
		}
	}
	else if (stop_animation == 0 && capture[0] == 1 || stop_animation == 0 && follow[0] == 1)
	{
		GLfloat dist = 0;
		GLfloat dx, dz;

		dx = move_x[0] - move_x[1];
		dz = move_z[0] - move_z[1];
		dist = sqrt(dx * dx + dz * dz);

		move_x[1] += dx * 0.006 / dist;
		move_z[1] += dz * 0.006 / dist;
	
		GLfloat rad;
		if (dz < 0) {
			rad = atan2(abs(dz), dx);
			direction[1] = rad * 180.0f / 3.14159265 + 90;
		}
		else
		{
			rad = atan2(dz, dx);
			direction[1] = rad * 180.0f / 3.14159265 * (-1) + 90;
		}

		glutPostRedisplay();
		glutTimerFunc(20, Timer_Move_ahead_1, 1);
	}
}
void Timer_Move_ahead_2(int value) {	//��2 ������ �̵�
	if (stop_animation == 0 && follow[1] != 1 && capture[1] != 1) {
		for (int i = 2; i < 3; ++i) {
			if (direction[i] == 0) {//��
				move_z[i] += 0.006;

				if (move_z[i] > 1.45)
					direction[i] = 180;
			}
			else if (direction[i] == 180) {	//��
				move_z[i] -= 0.006;

				if (move_z[i] < -1.45)
					direction[i] = 0;
			}
			else if (direction[i] == -90) {	//��
				move_x[i] -= 0.006;

				if (move_x[i] < -1.45)
					direction[i] = 90;
			}
			else if (direction[i] == 90) {	//��
				move_x[i] += 0.006;

				if (move_x[i] > 1.45)
					direction[i] = -90;
			}

			glutPostRedisplay();
			glutTimerFunc(20, Timer_Move_ahead_2, 1);
		}
	}
	else if (stop_animation == 0 && capture[1] == 1 || stop_animation == 0 && follow[1] == 1)
	{
		GLfloat dist = 0;
		GLfloat dx, dz;

		dx = move_x[0] - move_x[2];
		dz = move_z[0] - move_z[2];
		dist = sqrt(dx * dx + dz * dz);

		move_x[2] += dx * 0.006 / dist;
		move_z[2] += dz * 0.006 / dist;

		GLfloat rad;
		if (dz < 0) {
			rad = atan2(abs(dz), dx);
			direction[2] = rad * 180.0f / 3.14159265 + 90;
		}
		else
		{
			rad = atan2(dz, dx);
			direction[2] = rad * 180.0f / 3.14159265 * (-1) + 90;
		}

		glutPostRedisplay();
		glutTimerFunc(20, Timer_Move_ahead_2, 1);
	}
}
void Timer_Move_ahead_3(int value) {	//��3 ������ �̵�
	if (stop_animation == 0 && follow[2] != 1 && capture[2] != 1) {
		for (int i = 3; i < 4; ++i) {
			if (direction[i] == 0) {//��
				move_z[i] += 0.006;

				if (move_z[i] > 1.45)
					direction[i] = 180;
			}
			else if (direction[i] == 180) {	//��
				move_z[i] -= 0.006;

				if (move_z[i] < -1.45)
					direction[i] = 0;
			}
			else if (direction[i] == -90) {	//��
				move_x[i] -= 0.006;

				if (move_x[i] < -1.45)
					direction[i] = 90;
			}
			else if (direction[i] == 90) {	//��
				move_x[i] += 0.006;

				if (move_x[i] > 1.45)
					direction[i] = -90;
			}

			glutPostRedisplay();
			glutTimerFunc(20, Timer_Move_ahead_3, 1);
		}
	}
	else if (stop_animation == 0 && capture[2] == 1 || stop_animation == 0 && follow[2] == 1)
	{
		GLfloat dist = 0;
		GLfloat dx, dz;

		dx = move_x[0] - move_x[3];
		dz = move_z[0] - move_z[3];
		dist = sqrt(dx * dx + dz * dz);

		move_x[3] += dx * 0.006 / dist;
		move_z[3] += dz * 0.006 / dist;

		GLfloat rad;
		if (dz < 0) {
			rad = atan2(abs(dz), dx);
			direction[3] = rad * 180.0f / 3.14159265 + 90;
		}
		else
		{
			rad = atan2(dz, dx);
			direction[3] = rad * 180.0f / 3.14159265 * (-1) + 90;
		}

		glutPostRedisplay();
		glutTimerFunc(20, Timer_Move_ahead_3, 1);
	}
}
void Timer_Move_ahead_4(int value) {	//��4 ������ �̵�
	if (stop_animation == 0 && follow[3] != 1 && capture[3] != 1) {
		for (int i = 4; i < 5; ++i) {
			if (direction[i] == 0) {//��
				move_z[i] += 0.006;

				if (move_z[i] > 1.45)
					direction[i] = 180;
			}
			else if (direction[i] == 180) {	//��
				move_z[i] -= 0.006;

				if (move_z[i] < -1.45)
					direction[i] = 0;
			}
			else if (direction[i] == -90) {	//��
				move_x[i] -= 0.006;

				if (move_x[i] < -1.45)
					direction[i] = 90;
			}
			else if (direction[i] == 90) {	//��
				move_x[i] += 0.006;

				if (move_x[i] > 1.45)
					direction[i] = -90;
			}

			glutPostRedisplay();
			glutTimerFunc(20, Timer_Move_ahead_4, 1);
		}
	}
	else if (stop_animation == 0 && capture[3] == 1 || stop_animation == 0 && follow[3] == 1)
	{
		GLfloat dist = 0;
		GLfloat dx, dz;

		dx = move_x[0] - move_x[4];
		dz = move_z[0] - move_z[4];
		dist = sqrt(dx * dx + dz * dz);

		move_x[4] += dx * 0.006 / dist;
		move_z[4] += dz * 0.006 / dist;

		GLfloat rad;
		if (dz < 0) {
			rad = atan2(abs(dz), dx);
			direction[4] = rad * 180.0f / 3.14159265 + 90;
		}
		else
		{
			rad = atan2(dz, dx);
			direction[4] = rad * 180.0f / 3.14159265 * (-1) + 90;
		}

		glutPostRedisplay();
		glutTimerFunc(20, Timer_Move_ahead_4, 1);
	}
}

void Timer_Walk_main(int value) {	//�κ� �ȴٸ� �ȱ� �ִϸ��̼�
	if (stop_animation == 0) {
		for (int i = 0; i < 1; ++i) {
			if (value == 1) {
				walk[i] += 1;

				if (walk[i] < 30.0f) {
					glutPostRedisplay();
					glutTimerFunc(10, Timer_Walk_main, 1);
				}
				else
					glutTimerFunc(10, Timer_Walk_main, 0);
			}
			else if (value == 0) {
				walk[i] -= 1;

				if (walk[i] > -30.0f) {
					glutPostRedisplay();
					glutTimerFunc(10, Timer_Walk_main, 0);
				}
				else
					glutTimerFunc(10, Timer_Walk_main, 1);
			}
		}
	}
}

void Timer_Walk_1(int value) {	//��1 �ȴٸ� �ȱ� �ִϸ��̼�
	if (stop_animation == 0) {
		int i = 1;
		if (value == 1) {
			walk[i] += 1;

			if (walk[i] < 30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_1, 1);
			}
			else
				glutTimerFunc(10, Timer_Walk_1, 0);
		}
		else if (value == 0) {
			walk[i] -= 1;

			if (walk[i] > -30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_1, 0);
			}
			else
				glutTimerFunc(10, Timer_Walk_1, 1);
		}
	}
}
void Timer_Walk_2(int value) {	//��2 �ȴٸ� �ȱ� �ִϸ��̼�
	if (stop_animation == 0) {
		int i = 2;
		if (value == 1) {
			walk[i] += 1;

			if (walk[i] < 30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_2, 1);
			}
			else
				glutTimerFunc(10, Timer_Walk_2, 0);
		}
		else if (value == 0) {
			walk[i] -= 1;

			if (walk[i] > -30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_2, 0);
			}
			else
				glutTimerFunc(10, Timer_Walk_2, 1);
		}
	}
}
void Timer_Walk_3(int value) {	//��3 �ȴٸ� �ȱ� �ִϸ��̼�
	if (stop_animation == 0) {
		int i = 3;
		if (value == 1) {
			walk[i] += 1;

			if (walk[i] < 30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_3, 1);
			}
			else
				glutTimerFunc(10, Timer_Walk_3, 0);
		}
		else if (value == 0) {
			walk[i] -= 1;

			if (walk[i] > -30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_3, 0);
			}
			else
				glutTimerFunc(10, Timer_Walk_3, 1);
		}
	}
}
void Timer_Walk_4(int value) {	//��4 �ȴٸ� �ȱ� �ִϸ��̼�
	if (stop_animation == 0) {
		int i = 4;
		if (value == 1) {
			walk[i] += 1;

			if (walk[i] < 30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_4, 1);
			}
			else
				glutTimerFunc(10, Timer_Walk_4, 0);
		}
		else if (value == 0) {
			walk[i] -= 1;

			if (walk[i] > -30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk_4, 0);
			}
			else
				glutTimerFunc(10, Timer_Walk_4, 1);
		}
	}
}

void Timer_Random_direction(int value) //3�ʸ��� �� ���� �������� ������
{
	if (stop_animation == 0) {
		for (int i = 1; i < 5; ++i) {
			GLfloat a = uid(dre);

			if (follow[i - 1] != 1 && capture[i - 1] != 1) {
				if (a <= 0.02)
					direction[i] = 0;
				else if (a <= 0.03)
					direction[i] = 180;
				else if (a <= 0.04)
					direction[i] = -90;
				else if (a <= 0.05)
					direction[i] = 90;
			}
		}

		glutPostRedisplay();
		glutTimerFunc(3000, Timer_Random_direction, 1);
	}
}

void Timer_Jump_main(int value) {
	//3��Ī
	if (perspective1_3 == 3) {
		if (value == 1) {
			move_y[0] += 0.01;

			if (move_y[0] < -0.07f) {
				glutPostRedisplay();
				glutTimerFunc(30, Timer_Jump_main, 1);
			}
			else
				glutTimerFunc(30, Timer_Jump_main, 0);
		}
		else if (value == 0) {
			move_y[0] -= 0.01;

			if (move_y[0] > -0.17f) {
				glutPostRedisplay();
				glutTimerFunc(30, Timer_Jump_main, 0);
			}
		}
	}
	//1��Ī
	else if (perspective1_3 == 1) {
		if (value == 1) {
			move_y[0] += 0.01;
			cameraPos.y = move_y[0];
			cameraDirection.y += 0.01;

			if (move_y[0] < -0.07f) {
				glutPostRedisplay();
				glutTimerFunc(30, Timer_Jump_main, 1);
			}
			else
				glutTimerFunc(30, Timer_Jump_main, 0);
		}
		else if (value == 0) {
			move_y[0] -= 0.01;
			cameraPos.y = move_y[0];
			cameraDirection.y -= 0.01;

			if (move_y[0] > -0.17f) {
				glutPostRedisplay();
				glutTimerFunc(30, Timer_Jump_main, 0);
			}
		}
	}
}

//ī�޶� ȭ���� �߽�y ���� ����
void TimerRotate_CAMERA_Y(int value)	
{
	if (stop_animation == 0) {

		CP_Y = cp_cw;	//�������� ���� ��ӿ�Ѵ�.
		MP_Y += cp_cw;

		float temp_x = cameraPos.x, temp_z = cameraPos.z;

		cameraPos.x = temp_x * glm::cos(glm::radians(CP_Y)) + temp_z * glm::sin(glm::radians(CP_Y));	//cw��ŭ ����(ȸ��)���ٴ� ���̹Ƿ�
		cameraPos.z = temp_z * glm::cos(glm::radians(CP_Y)) - temp_x * glm::sin(glm::radians(CP_Y));

		glutPostRedisplay();
	}

}

//ī�޶�y ���� ����
void TimerRevolution_CAMERA_Y(int value)	
{
	if (stop_animation == 0) {
		if (perspective1_3 == 3) {
			CD_Y += cd_cw;	//�������� ���� ��ӿ�Ѵ�.

			cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y));
			cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + 3.0f;
		}
		else if (perspective1_3 == 1) {
			CD_Y += cd_cw;	//�������� ���� ��ӿ�Ѵ�.

			cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y)) + move_x[0];
			cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + move_z[0];
		}

		glutPostRedisplay();
	}

}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		perspective1_3 = 1;
		cameraPos.x = move_x[0]; cameraPos.y = move_y[0]; cameraPos.z = move_z[0];
		if (direction[0] == 360.0f || direction[0] == 0.0f)
		{
			cameraDirection.x = move_x[0]; cameraDirection.y = move_y[0]; cameraDirection.z = 3;
			CD_Y = 90;
		}
		else if (direction[0] == 180)
		{
			cameraDirection.x = move_x[0]; cameraDirection.y = move_y[0]; cameraDirection.z = -3;
			CD_Y = 270;
		}
		else if (direction[0] == -90)
		{
			cameraDirection.x = -3; cameraDirection.y = move_y[0]; cameraDirection.z = move_z[0];
			CD_Y = 180;
		}
		else if (direction[0] == 90)
		{
			cameraDirection.x = 3; cameraDirection.y = move_y[0]; cameraDirection.z = move_z[0];
			CD_Y = 0;
		}
		break;
	case '3':
		perspective1_3 = 3;
		cameraPos.x = 0; cameraPos.y = 0.5; cameraPos.z = 2.3;
		cameraDirection.x = 0; cameraDirection.y = 0; cameraDirection.z = 0;
		break;

	case 'p':
	case 'P':
		if (perspective == 1)
			perspective = 0;
		else perspective = 1;
		break;

	case 'w'://��
		if (direction[0] == 360) {
			stop_animation = 0;
			direction[0] = 180.0f;
			glutTimerFunc(0, Timer_Move_ahead_main, 1);
			glutTimerFunc(0, Timer_Walk_main, 1);
		}
		else
			direction[0] = 180.0f;

		if (perspective1_3 == 1) {
			cameraDirection.x = move_x[0]; cameraDirection.y = move_y[0]; cameraDirection.z = -3;
			CD_Y = 270;
		}
		break;
	case 's'://��
		if (direction[0] == 360) {
			stop_animation = 0;
			direction[0] = 0.0f;
			glutTimerFunc(0, Timer_Move_ahead_main, 1);
			glutTimerFunc(0, Timer_Walk_main, 1);
		}
		else
			direction[0] = 0.0f;

		if (perspective1_3 == 1) {
			cameraDirection.x = move_x[0]; cameraDirection.y = move_y[0]; cameraDirection.z = 3;
			CD_Y = 90;
		}
		break;
	case 'a'://��
		if (direction[0] == 360) {
			stop_animation = 0;
			direction[0] = -90.0f;
			glutTimerFunc(0, Timer_Move_ahead_main, 1);
			glutTimerFunc(0, Timer_Walk_main, 1);
		}
		else
			direction[0] = -90.0f;

		if (perspective1_3 == 1) {
			cameraDirection.x = -3; cameraDirection.y = move_y[0]; cameraDirection.z = move_z[0];
			CD_Y = 180;
		}
		break;
	case 'd'://��
		if (direction[0] == 360) {
			stop_animation = 0;
			direction[0] = 90.0f;
			glutTimerFunc(0, Timer_Move_ahead_main, 1);
			glutTimerFunc(0, Timer_Walk_main, 1);
		}
		else
			direction[0] = 90.0f;

		if (perspective1_3 == 1) {
			cameraDirection.x = 3; cameraDirection.y = move_y[0]; cameraDirection.z = move_z[0];
			CD_Y = 0;
		}
		break;
	case 'j':
	case 'J':
		if (move_y[0] == -0.17f) {
			stop_animation = 0;
			glutTimerFunc(0, Timer_Jump_main, 1);
		}
		break;

	case 'o':
	case 'O':
		stop_animation = 0;
		if (direction[1] == 360) {
			for (int i = 1; i < 5; ++i) {
				GLfloat a = uid(dre);

				if (a <= 0.02)
					direction[i] = 0;
				else if (a <= 0.03)
					direction[i] = 180;
				else if (a <= 0.04)
					direction[i] = -90;
				else if (a <= 0.05)
					direction[i] = 90;
			}
			glutTimerFunc(0, TimerOpen_Screen, 1);
			glutTimerFunc(0, Timer_Move_ahead_1, 1);
			glutTimerFunc(0, Timer_Move_ahead_2, 1);
			glutTimerFunc(0, Timer_Move_ahead_3, 1);
			glutTimerFunc(0, Timer_Move_ahead_4, 1);
			glutTimerFunc(0, Timer_Walk_1, 1);
			glutTimerFunc(0, Timer_Walk_2, 1);
			glutTimerFunc(0, Timer_Walk_3, 1);
			glutTimerFunc(0, Timer_Walk_4, 1);

			glutTimerFunc(0, Timer_Random_direction, 1);
		}
		break;

	case 'y':
		cd_cw = 3.0f;
		stop_animation = 0;
		glutTimerFunc(0, TimerRevolution_CAMERA_Y, 1);
		break;
	case 'Y':
		cd_cw = -3.0f;
		stop_animation = 0;
		glutTimerFunc(0, TimerRevolution_CAMERA_Y, 1);
		break;
	case 'c':
		cp_cw = 1.0f;
		stop_animation = 0;
		glutTimerFunc(0, TimerRotate_CAMERA_Y, 1);
		break;
	case 'C':
		cp_cw = -1.0f;
		stop_animation = 0;
		glutTimerFunc(0, TimerRotate_CAMERA_Y, 1);
		break;

	case 'i':
	case 'I':	//�ʱ� ����������
		stop_animation = 1;
		for (int i = 0; i < 4; ++i)
		{
			capture[i] = 0;
			follow[i] = 0;
		}
		for (int i = 0; i < 5; ++i)
			direction[i] = 360.0f;
		perspective = 1;
		perspective1_3 = 3;
		for (int i = 0; i < 5; ++i)
			walk[i] = 0;
		frontscreen_y = 0, frontscreen_r = 0;
		move_x[0] = 0; move_y[0] = -0.17f; move_z[0] = 0.0f;
		for (int i = 1; i < 5; ++i) {
			move_x[i] = uid_loc(dre); move_y[i] = -0.17f; move_z[i] = uid_loc(dre);
		}
		cameraPos.x = 0.0f; cameraPos.y = 0.5f; cameraPos.z = 2.3f; cameraDirection.x = 0.0f; cameraDirection.y = 0; cameraDirection.z = 0.0f;
		CP_Y = 0.0f; cp_cw = 0; CD_Y = 270.0f; cd_cw = 0;
		break;

	case 'q':
	case 'Q':
		exit(0);
		break;
	case 27:
		exit(0);
		break;

	}
	glutPostRedisplay();
}

void Mouse(int b, int s, int x, int y)
{

	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("1/3 Camera");

	cout << "<Ű���� �Է�>" << endl << endl;
	cout << "-�帷:" << endl;
	cout << "o/O: �帷���� (�κ��� �����̱� ����)" << endl << endl;
	cout << "-ī�޶� ������ȯ:" << endl;
	cout << "1/3: 1��Ī ����/3��Ī ����" << endl << endl;
	cout << "-ī�޶� �̵�:" << endl;
	cout << "w/a/s/d: ī�޶� ��/��/��/�� �̵�\nj/J: ����" << endl;
	cout << "y/Y: ī�޶� ���� y�� ��/�� ���� ȸ��" << endl;
	cout << "c/C: ȭ���� ������ ���Ͽ� y�� ��/�� ���� ȸ��" << endl;
	cout << "i/I: ��� �� �ʱ�ȭ" << endl;
	cout << "q/esc: ���α׷� ����" << endl << endl;
	

	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitTexture();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	//glutMouseFunc(Mouse);
	glutMainLoop();
}