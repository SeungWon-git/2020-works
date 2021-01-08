#pragma warning(disable: 4996)
#include <random>
#include <iostream>
#include <fstream>

#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//�Ҹ����
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace std;


std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid{ 0.01,0.05 };

//���(�ּ�)
#define hexahedronpoints	36	//-> �� ���� ���� = �ﰢ��(������) ���� * 3(������)
#define pyramidpoints		18
#define spherepoints		960*3

GLfloat hexa_s[hexahedronpoints][3] = {};
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat sphere_s[spherepoints][3] = {};
GLfloat ui1_s[6][3] = {
	{0.08,0.08,0} ,{-0.08,-0.08,0}, {-0.08,0.08,0},
	{-0.08,-0.08,0} ,{0.08,0.08,0}, {0.08,-0.08,0} };
GLfloat ui2_s[6][3] = {
	{0.07,0.07,0} ,{-0.07,-0.07,0}, {-0.07,0.07,0},
	{-0.07,-0.07,0} ,{0.07,0.07,0}, {0.07,-0.07,0} };

GLfloat hexa����_s[hexahedronpoints][3] = {};
GLfloat hexa�㸮_s[hexahedronpoints][3] = {};
GLfloat hexa���_s[hexahedronpoints][3] = {};
GLfloat hexa���_s[hexahedronpoints][3] = {};
GLfloat hexa���_s[hexahedronpoints][3] = {};
GLfloat hexa�Ϲ�_s[hexahedronpoints][3] = {};
GLfloat hexa�ָ�1_s[hexahedronpoints][3] = {};
GLfloat hexa�ָ�2l_s[hexahedronpoints][3] = {}; GLfloat hexa�ָ�2r_s[hexahedronpoints][3] = {};
GLfloat hexa�ָ�3_s[hexahedronpoints][3] = {};
GLfloat hexa�����_s[hexahedronpoints][3] = {};
GLfloat hexa���Ƹ�_s[hexahedronpoints][3] = {};
GLfloat hexa��_s[hexahedronpoints][3] = {};

GLfloat ���հ�_s[hexahedronpoints][3] = {}; 
GLfloat ������V_s[6][3] = {};

//����
GLfloat hexa_vn[hexahedronpoints][3] = {};
GLfloat pyramid_vn[pyramidpoints][3] = {};
GLfloat sphere_vn[spherepoints][3] = {};
GLfloat ui_vn[6][3] = {
	{0,0,1} ,{0,0,1}, {0,0,1},
	{0,0,1} ,{0,0,1}, {0,0,1} };

GLfloat hexa����_vn[hexahedronpoints][3] = {};
GLfloat hexa�㸮_vn[hexahedronpoints][3] = {};
GLfloat hexa���_vn[hexahedronpoints][3] = {};
GLfloat hexa���_vn[hexahedronpoints][3] = {};
GLfloat hexa���_vn[hexahedronpoints][3] = {};
GLfloat hexa�Ϲ�_vn[hexahedronpoints][3] = {};
GLfloat hexa�ָ�1_vn[hexahedronpoints][3] = {};
GLfloat hexa�ָ�2l_vn[hexahedronpoints][3] = {};
GLfloat hexa�ָ�3_vn[hexahedronpoints][3] = {};
GLfloat hexa�����_vn[hexahedronpoints][3] = {};
GLfloat hexa���Ƹ�_vn[hexahedronpoints][3] = {};
GLfloat hexa��_vn[hexahedronpoints][3] = {};

GLfloat ���հ�_vn[hexahedronpoints][3] = {};
GLfloat ������V_vn[6][3] = {};


//�ؽ�ó ��ǥ
GLfloat hexa_vt[hexahedronpoints][2];
GLfloat pyramid_vt[pyramidpoints][2];
GLfloat sphere_vt[spherepoints][2] = {};
GLfloat ui_vt[6][2] = {
	{1,1} ,{0,0}, {0,1},
	{0,0} ,{1,1}, {1,0} };

GLfloat hexa����_vt[hexahedronpoints][2] = {};
GLfloat hexa�㸮_vt[hexahedronpoints][2] = {};
GLfloat hexa���_vt[hexahedronpoints][2] = {};
GLfloat hexa���_vt[hexahedronpoints][2] = {};
GLfloat hexa���_vt[hexahedronpoints][2] = {};
GLfloat hexa�Ϲ�_vt[hexahedronpoints][2] = {};
GLfloat hexa�ָ�1_vt[hexahedronpoints][2] = {};
GLfloat hexa�ָ�2l_vt[hexahedronpoints][2] = {};
GLfloat hexa�ָ�3_vt[hexahedronpoints][2] = {};
GLfloat hexa�����_vt[hexahedronpoints][2] = {};
GLfloat hexa���Ƹ�_vt[hexahedronpoints][2] = {};
GLfloat hexa��_vt[hexahedronpoints][2] = {};

GLfloat ���հ�_vt[hexahedronpoints][2] = {};
GLfloat ������V_vt[6][2] = {};


//vao,vbo
GLuint hexa_vao, hexa_vbo[3];
GLuint pyramid_vao, pyramid_vbo[3];
GLuint sphere_vao, sphere_vbo[3];
GLuint ui1_vao, ui1_vbo[3];
GLuint ui2_vao, ui2_vbo[3];

GLuint hexa����_vao, hexa����_vbo[3];
GLuint hexa�㸮_vao, hexa�㸮_vbo[3];
GLuint hexa���_vao, hexa���_vbo[3];
GLuint hexa���_vao, hexa���_vbo[3];
GLuint hexa���_vao, hexa���_vbo[3];
GLuint hexa�Ϲ�_vao, hexa�Ϲ�_vbo[3];
GLuint hexa�ָ�1_vao, hexa�ָ�1_vbo[3];
GLuint hexa�ָ�2l_vao, hexa�ָ�2l_vbo[3]; GLuint hexa�ָ�2r_vao, hexa�ָ�2r_vbo[3];
GLuint hexa�ָ�3_vao, hexa�ָ�3_vbo[3];
GLuint hexa�����_vao, hexa�����_vbo[3];
GLuint hexa���Ƹ�_vao, hexa���Ƹ�_vbo[3];
GLuint hexa��_vao, hexa��_vbo[3];

GLuint ���հ�_vao, ���հ�_vbo[3];
GLuint ������V_vao, ������V_vbo[3];


//���� ����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 1000.0f;
GLfloat window_h = 800.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, -1.5f, 6.0f);			//ī�޶� ��ġ
glm::vec3 cameraDirection = glm::vec3(0.0f, -1.5f, 0.0f);	//ī�޶� �ٶ󺸴� ������
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//ī�޶��� ����� ����Ű�� ����

//�Ϲ� ����
//GLfloat move_x = 0, move_y = 0.0f, move_z = 0.0f;	//�̵�

bool start = 0;
float startlogo_size = 1.0;
bool click = 0;
float c_size = 0;
float x0_d = 0;
int character = 0;
bool projection = 1;
bool animation = 0;


//---���� ����
GLfloat 
	//��ݽ�
	��_x = 0, ��_y = 0,
	����_y = 0, 
	�޻��_x = 0, �޻��_z = 0, �������_x = 0, �������_z = 0,
	���Ϲ�_x = 0, ���Ϲ�_z = 0, �����Ϲ�_x = 0, �����Ϲ�_z = 0,
	���ָ�_x = 0, ���ָ�_y = 0, ���ָ�_z = 0, �����ָ�_x = 0, �����ָ�_y = 0, �����ָ�_z = 0,
	//�Ϲݽ�
	���_y = 0,
	�������_x = 0, �������_z = 0, ���������_x = 0, ���������_z = 0,
	�����Ƹ�_x = 0, �����Ƹ�_z = 0, �������Ƹ�_x = 0, �������Ƹ�_z = 0,
	�޹�_x = 0, ������_x = 0;



//����
GLfloat light_r = 1.0f, light_g = 1.0f, light_b = 1.0f;	//�������
GLfloat light_x = 0.0, light_y = 5.0, light_z = 10.0;

// �̹���
unsigned int
������, �����, ������, �����Ǻλ�, ��Ƽ_�Ķ���, ȸ��, ���, ����, ���λ�, ���������, �Ķ���,
��_f, ��_r, ��_l, ��_bo, ��_u, ��_ba, ��_��V,
��_f, ��_r, ��_l, ��_bo, ��_u, ��_ba, ��_��V,
��_f, ��_r, ��_l, ��_bo, ��_u, ��_ba,
��_f, ��_r, ��_l, ��_bo, ��_u, ��_ba,
��_f, ��_r, ��_l, ��_bo, ��_u, ��_ba,
����ȭ��, �ΰ�, ����,
ui_��ȭ, ui_��ȭ, ui_�ð�, ui_�ݽð�, ui_������, ui_�ʱ�ȭ, 
ui����_��, ui����_��, ui����_��,
ui_ĳ��, ui_��Ʈ��, ui_����,
ui_�ִϸ��̼�, ui_����,
ui_��ü,	 ui_��x, ui_��y, ui_����y, ui_�޻�x, ui_�޻�z, ui_����x, ui_����z, ui_����x, ui_����y, ui_����z, ui_����x, ui_����z, ui_����x, ui_����z, ui_����x, ui_����y, ui_����z,
ui_��ü,	 ui_��y, ui_����x, ui_����z, ui_����x, ui_����z, ui_�޹�x, ui_����x, ui_����z, ui_����x, ui_����z, ui_����x,
ui_���, ui_�ҷ�����;

struct UI_coord {
	int x, y;
	float a;
};

UI_coord ui = { 0,1,0 };

class Body {
	//---�� ����
	int model;



	//---���� ����
	GLfloat
		//��ݽ�
		��_x = 0, ��_y = 0,
		����_y = 0,
		�޻��_x = 0, �޻��_z = 0, �������_x = 0, �������_z = 0,
		���Ϲ�_x = 0, ���Ϲ�_z = 0, �����Ϲ�_x = 0, �����Ϲ�_z = 0,
		���ָ�_x = 0, ���ָ�_y = 0, ���ָ�_z = 0, �����ָ�_x = 0, �����ָ�_y = 0, �����ָ�_z = 0,
		//�Ϲݽ�
		���_y = 0,
		�������_x = 0, �������_z = 0, ���������_x = 0, ���������_z = 0,
		�����Ƹ�_x = 0, �����Ƹ�_z = 0, �������Ƹ�_x = 0, �������Ƹ�_z = 0,
		�޹�_x = 0, ������_x = 0;
};





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

	fclose(objFile);

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

void AssignObj_Variable() {
	// obj�б�
	//------------------------------------�⺻--------------------------------------------
	char filename[30] = "obj/������ü.obj";
	FILE* file = fopen(filename, "rb");

	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa_s, hexa_vn, hexa_vt);

	strcpy(filename, "obj/�簢��(�Ƕ�̵�).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, pyramid_s, pyramid_vn, pyramid_vt);

	//------------------------------------��--------------------------------------------

	strcpy(filename, "obj/������ü-����.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa����_s, hexa����_vn, hexa����_vt);

	strcpy(filename, "obj/������ü-�㸮.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa�㸮_s, hexa�㸮_vn, hexa�㸮_vt);
	
	strcpy(filename, "obj/������ü-���.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa���_s, hexa���_vn, hexa���_vt);

	strcpy(filename, "obj/������ü-���.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa���_s, hexa���_vn, hexa���_vt);

	strcpy(filename, "obj/������ü-���.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa���_s, hexa���_vn, hexa���_vt);

	strcpy(filename, "obj/������ü-�Ϲ�.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

	strcpy(filename, "obj/������ü-�ָ�1.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);

	strcpy(filename, "obj/������ü-�ָ�2l.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);

	strcpy(filename, "obj/������ü-�ָ�2r.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);

	strcpy(filename, "obj/������ü-�ָ�3.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

	strcpy(filename, "obj/������ü-�����.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa�����_s, hexa�����_vn, hexa�����_vt);

	strcpy(filename, "obj/������ü-���Ƹ�.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

	strcpy(filename, "obj/������ü-��.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa��_s, hexa��_vn, hexa��_vt);

	//------------------------------------Ư�� ���--------------------------------------------
	
	//---��¡��
	strcpy(filename, "obj/������ü-��¡���հ�.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, ���հ�_s, ���հ�_vn, ���հ�_vt);

	strcpy(filename, "obj/������ü-������V.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, ������V_s, ������V_vn, ������V_vt);


	fclose(file);
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

void InitBuffer()
{
	InitVao(hexa_vao, hexa_vbo, hexahedronpoints, hexa_s, hexa_vn, hexa_vt);
	InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
	InitVao(ui1_vao, ui1_vbo, 6, ui1_s, ui_vn, ui_vt);
	InitVao(ui2_vao, ui2_vbo, 6, ui2_s, ui_vn, ui_vt);

	InitVao(hexa����_vao, hexa����_vbo, hexahedronpoints, hexa����_s, hexa����_vn, hexa����_vt);
	InitVao(hexa�㸮_vao, hexa�㸮_vbo, hexahedronpoints, hexa�㸮_s, hexa�㸮_vn, hexa�㸮_vt);
	InitVao(hexa���_vao, hexa���_vbo, hexahedronpoints, hexa���_s, hexa���_vn, hexa���_vt);
	InitVao(hexa���_vao, hexa���_vbo, hexahedronpoints, hexa���_s, hexa���_vn, hexa���_vt);
	InitVao(hexa���_vao, hexa���_vbo, hexahedronpoints, hexa���_s, hexa���_vn, hexa���_vt);
	InitVao(hexa�Ϲ�_vao, hexa�Ϲ�_vbo, hexahedronpoints, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
	InitVao(hexa�ָ�1_vao, hexa�ָ�1_vbo, hexahedronpoints, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
	InitVao(hexa�ָ�2l_vao, hexa�ָ�2l_vbo, hexahedronpoints, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt); InitVao(hexa�ָ�2r_vao, hexa�ָ�2r_vbo, hexahedronpoints, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
	InitVao(hexa�ָ�3_vao, hexa�ָ�3_vbo, hexahedronpoints, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
	InitVao(hexa�����_vao, hexa�����_vbo, hexahedronpoints, hexa�����_s, hexa�����_vn, hexa�����_vt);
	InitVao(hexa���Ƹ�_vao, hexa���Ƹ�_vbo, hexahedronpoints, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
	InitVao(hexa��_vao, hexa��_vbo, hexahedronpoints, hexa��_s, hexa��_vn, hexa��_vt);

	InitVao(���հ�_vao, ���հ�_vbo, hexahedronpoints, ���հ�_s, ���հ�_vn, ���հ�_vt);
	InitVao(������V_vao, ���հ�_vbo, 6, ������V_s, ������V_vn, ������V_vt);
}

void InitTexture() {
	BITMAPINFO* bmp;
	int widthImage, heightImage, numberOfChannel;

	unsigned char* data;

	// -------------------------------------- [�ܻ�]---------------------------------------------------------
	{
		//������
		glGenTextures(1, &������); //--- �ؽ�ó ����
		glBindTexture(GL_TEXTURE_2D, ������); //--- �ؽ�ó ���ε�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
		data = stbi_load("res/������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
		//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
		//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)
		//�����
		glGenTextures(1, &�����);
		glBindTexture(GL_TEXTURE_2D, �����);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//������
		glGenTextures(1, &������);
		glBindTexture(GL_TEXTURE_2D, ������);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//�����Ǻλ�
		glGenTextures(1, &�����Ǻλ�);
		glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�����Ǻλ�.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//��Ƽ-�Ķ���
		glGenTextures(1, &��Ƽ_�Ķ���);
		glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��Ƽ-�Ķ���.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ȸ��
		glGenTextures(1, &ȸ��);
		glBindTexture(GL_TEXTURE_2D, ȸ��);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ȸ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//���
		glGenTextures(1, &���);
		glBindTexture(GL_TEXTURE_2D, ���);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/���.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//����
		glGenTextures(1, &����);
		glBindTexture(GL_TEXTURE_2D, ����);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//���λ�
		glGenTextures(1, &���λ�);
		glBindTexture(GL_TEXTURE_2D, ���λ�);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/���λ�.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//���������
		glGenTextures(1, &���������);
		glBindTexture(GL_TEXTURE_2D, ���������);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/���������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//�Ķ���
		glGenTextures(1, &�Ķ���);
		glBindTexture(GL_TEXTURE_2D, �Ķ���);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�Ķ���.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	// -------------------------------------- [�޴�]---------------------------------------------------------
	{
		//����ȭ��
		glGenTextures(1, &����ȭ��);
		glBindTexture(GL_TEXTURE_2D, ����ȭ��);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/Realsteel.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//�ΰ�
		glGenTextures(1, &�ΰ�);
		glBindTexture(GL_TEXTURE_2D, �ΰ�);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/Realsteel-logo.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//����
		glGenTextures(1, &����);
		glBindTexture(GL_TEXTURE_2D, ����);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	// -------------------------------------- [ui]---------------------------------------------------------
	{
		glGenTextures(1, &ui_��ȭ);
		glBindTexture(GL_TEXTURE_2D, ui_��ȭ);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_��ȭ);
		glBindTexture(GL_TEXTURE_2D, ui_��ȭ);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�ð�);
		glBindTexture(GL_TEXTURE_2D, ui_�ð�);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�ð����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�ݽð�);
		glBindTexture(GL_TEXTURE_2D, ui_�ݽð�);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�ݽð����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_������);
		glBindTexture(GL_TEXTURE_2D, ui_������);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�ʱ�ȭ);
		glBindTexture(GL_TEXTURE_2D, ui_�ʱ�ȭ);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�ʱ�ȭ.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui����_��);
		glBindTexture(GL_TEXTURE_2D, ui����_��);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui����_��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui����_��);
		glBindTexture(GL_TEXTURE_2D, ui����_��);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui����_��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui����_��);
		glBindTexture(GL_TEXTURE_2D, ui����_��);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui����_��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_ĳ��);
		glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_ĳ������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_��Ʈ��);
		glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_��Ʈ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//-------[��Ʈ��]
		glGenTextures(1, &ui_��ü);
		glBindTexture(GL_TEXTURE_2D, ui_��ü);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_��ü.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_��ü);
		glBindTexture(GL_TEXTURE_2D, ui_��ü);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_��ü.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//---[��ü]
		glGenTextures(1, &ui_��x);
		glBindTexture(GL_TEXTURE_2D, ui_��x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_��x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_��y);
		glBindTexture(GL_TEXTURE_2D, ui_��y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_��y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����y);
		glBindTexture(GL_TEXTURE_2D, ui_����y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_����y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�޻�x);
		glBindTexture(GL_TEXTURE_2D, ui_�޻�x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�޻��x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�޻�z);
		glBindTexture(GL_TEXTURE_2D, ui_�޻�z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�޻��z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���Ϲ�x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���Ϲ�z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���ָ�x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����y);
		glBindTexture(GL_TEXTURE_2D, ui_����y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���ָ�y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���ָ�z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�������x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�������z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�����Ϲ�x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�����Ϲ�z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�����ָ�x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����y);
		glBindTexture(GL_TEXTURE_2D, ui_����y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�����ָ�y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�����ָ�z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//---[��ü]
		glGenTextures(1, &ui_��y);
		glBindTexture(GL_TEXTURE_2D, ui_��y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�������x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�������z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�����Ƹ�x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�����Ƹ�z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�޹�x);
		glBindTexture(GL_TEXTURE_2D, ui_�޹�x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�޹�x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���������x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���������z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�������Ƹ�x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����z);
		glBindTexture(GL_TEXTURE_2D, ui_����z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�������Ƹ�z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����x);
		glBindTexture(GL_TEXTURE_2D, ui_����x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_������x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		//---����
		glGenTextures(1, &ui_����);
		glBindTexture(GL_TEXTURE_2D, ui_����);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_���);
		glBindTexture(GL_TEXTURE_2D, ui_���);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_���.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�ҷ�����);
		glBindTexture(GL_TEXTURE_2D, ui_�ҷ�����);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�ҷ�����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_�ִϸ��̼�);
		glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_�ִϸ��̼�.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_����);
		glBindTexture(GL_TEXTURE_2D, ui_����);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_����.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	// -------------------------------------- [��]---------------------------------------------------------
	//-----------�±�V
	{
		//u
		glGenTextures(1, &��_u);
		glBindTexture(GL_TEXTURE_2D, ��_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &��_bo);
		glBindTexture(GL_TEXTURE_2D, ��_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &��_f);
		glBindTexture(GL_TEXTURE_2D, ��_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�±�Vf.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &��_ba);
		glBindTexture(GL_TEXTURE_2D, ��_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�±�Vb.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &��_r);
		glBindTexture(GL_TEXTURE_2D, ��_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�±�Vr.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &��_l);
		glBindTexture(GL_TEXTURE_2D, ��_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�±�Vl.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//�±�V ����V
		glGenTextures(1, &��_��V);
		glBindTexture(GL_TEXTURE_2D, ��_��V);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�±�V-����V.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------��¡��Z
	{
		//u
		glGenTextures(1, &��_u);
		glBindTexture(GL_TEXTURE_2D, ��_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ȸ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &��_bo);
		glBindTexture(GL_TEXTURE_2D, ��_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ȸ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &��_f);
		glBindTexture(GL_TEXTURE_2D, ��_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��¡��Zf.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &��_ba);
		glBindTexture(GL_TEXTURE_2D, ��_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��¡��Zb.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &��_r);
		glBindTexture(GL_TEXTURE_2D, ��_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��¡��Zr.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &��_l);
		glBindTexture(GL_TEXTURE_2D, ��_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��¡��Zl.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
		//����V
		glGenTextures(1, &��_��V);
		glBindTexture(GL_TEXTURE_2D, ��_��V);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��¡��-����V.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------����
	{
		//u
		glGenTextures(1, &��_u);
		glBindTexture(GL_TEXTURE_2D, ��_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/������.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &��_bo);
		glBindTexture(GL_TEXTURE_2D, ��_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�����Ǻλ�.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &��_f);
		glBindTexture(GL_TEXTURE_2D, ��_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/����f.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &��_ba);
		glBindTexture(GL_TEXTURE_2D, ��_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/����b.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &��_r);
		glBindTexture(GL_TEXTURE_2D, ��_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/����r.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &��_l);
		glBindTexture(GL_TEXTURE_2D, ��_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/����l.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------�͹̳�����
	{
		//u
		glGenTextures(1, &��_u);
		glBindTexture(GL_TEXTURE_2D, ��_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ȸ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &��_bo);
		glBindTexture(GL_TEXTURE_2D, ��_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ȸ��.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &��_f);
		glBindTexture(GL_TEXTURE_2D, ��_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�͹̳�����f.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &��_ba);
		glBindTexture(GL_TEXTURE_2D, ��_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�͹̳�����b.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &��_r);
		glBindTexture(GL_TEXTURE_2D, ��_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�͹̳�����r.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &��_l);
		glBindTexture(GL_TEXTURE_2D, ��_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/�͹̳�����l.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------��Ƽ�ӽ�
	{
		//u
		glGenTextures(1, &��_u);
		glBindTexture(GL_TEXTURE_2D, ��_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��Ƽ-�Ķ���.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &��_bo);
		glBindTexture(GL_TEXTURE_2D, ��_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��Ƽ-�Ķ���.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &��_f);
		glBindTexture(GL_TEXTURE_2D, ��_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��Ƽ�ӽ�f.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &��_ba);
		glBindTexture(GL_TEXTURE_2D, ��_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��Ƽ�ӽ�b.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &��_r);
		glBindTexture(GL_TEXTURE_2D, ��_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��Ƽ�ӽ�r.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &��_l);
		glBindTexture(GL_TEXTURE_2D, ��_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/��Ƽ�ӽ�l.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }

	glUseProgram(s_program);
	int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture ������ ���÷��� ��ġ�� ������
	glUniform1i(tLocation, 0); //--- ���÷��� 0�� �������� ����

	stbi_image_free(data);	//---�޸� ��ȯ
}

void MoveDown(GLuint* vao, GLuint *vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][1] -= d;
		s[i][1] = round(s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveUp(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][1] += d;
		s[i][1] = round(s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveLeft(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][0] -= d;
		s[i][0] = round(s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveRight(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][0] += d;
		s[i][0] = round(s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveBack(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][2] -= d;
		s[i][2] = round(s[i][2] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveForward(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][2] += d;
		s[i][2] = round(s[i][2] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

GLvoid drawScene()
{
	if(start == 0)
		glClearColor(0.0, 0.0, 0.0, 0.0f);
	else
		glClearColor(0.2, 0.2, 0.2, 0.0f);
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

	vTransform = glm::mat4(1.0f);	// ī�޶� ��ȯ
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLoc = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	pTransform = glm::mat4(1.0f);	// ���� ��ȯ
	if (projection == 1)	//���� ����
		pTransform = glm::perspective(glm::radians(45.0f), window_w / window_h, 0.1f, 50.0f);
	else					//���� ����
		pTransform = glm::ortho(-3.0, 3.0, -3.0, 3.0, 0.1, 50.0);
	projLoc = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	//����
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos");		//--- lightPos �� ����
	glUniform3f(lightPosLocation, light_x, light_y, light_z);
	int lightColorLocation = glGetUniformLocation(s_program, "lightColor");	//--- lightColor �� ����: (1.0, 1.0, 1.0) ���
	glUniform3f(lightColorLocation, light_r, light_g, light_b);
	int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color�� ����
	glUniform3f(objColorLocation, 1, 1, 1);	// --- ��� ����

	//�ؽ��� Ȱ��ȭ
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);	//����
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (start == 0)
	{
		//���
		glBindVertexArray(hexa_vao);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::translate(mTransform, glm::vec3(0, -1.5, 4.8));
		mTransform = glm::scale(mTransform, glm::vec3(2.5, 1.1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ����ȭ��);
		glDrawArrays(GL_TRIANGLES, 30, 6);	///-��

		//�ΰ�
		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::translate(mTransform, glm::vec3(0, -1.15, 4.801));
		mTransform = glm::scale(mTransform, glm::vec3(1.3, 0.5, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, �ΰ�);
		glDrawArrays(GL_TRIANGLES, 30, 6);	///-��

		//���͹�ư
		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::translate(mTransform, glm::vec3(0.0, -1.7, 5.25));
		mTransform = glm::scale(mTransform, glm::vec3(0.75 * startlogo_size, 0.25 * startlogo_size, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ����);
		glDrawArrays(GL_TRIANGLES, 30, 6);	///-��
	}
	else
	{
		//�����
		glBindVertexArray(hexa_vao);
		{
			//��-�ٴ�
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -3.55, 0));
			mTransform = glm::scale(mTransform, glm::vec3(50, 1.0, 50));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ���������);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		{
			//��-��4��
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.75, -1.3, 11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, ������);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.75, -1.3, 11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, ������);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.75, -1.3, -11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, ������);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.75, -1.3, -11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, ������);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		{
			//��-������-������
			glBindTexture(GL_TEXTURE_2D, ������);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.1, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.1, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, 0.1, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, 0.1, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//��-�߰���1-���
			glBindTexture(GL_TEXTURE_2D, ���);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -0.8, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -0.8, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, -0.8, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, -0.8, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//��-�߰���2-���
			glBindTexture(GL_TEXTURE_2D, ���);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -1.6, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -1.6, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, -1.6, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, -1.6, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//��-�ǾƷ���-�Ķ���
			glBindTexture(GL_TEXTURE_2D, �Ķ���);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -2.5, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -2.5, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, -2.5, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, -2.5, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//UI

		//������ �������̽�
		//x=0
		if (ui.x == 0 and click == 0) {
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_����);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			{
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76 - (ui.y - 1) * 0.2, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			{
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=0,y=1
		else if (ui.x == 1 and (ui.y/10) == 1 || ui.x == 0 and ui.y == 1 and click == 1) {
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_����);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ��_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ��_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ��_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ��_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ��_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			if (click == 1 and (ui.y/10) == 1)
			{
				if (c_size < 1) {
					glBindVertexArray(ui1_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75 - (ui.y - 11) * 0.2, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
			else {
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75 - (ui.y - 11) * 0.2, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=0,y=2
		else if (ui.x == 1 and (ui.y / 10) == 2  and c_size <= 1 || ui.x == 0 and ui.y == 2 and click == 1) {
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_����);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_��ü);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_��ü);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			if (click == 1 and ui.y == 21)
			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else {
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95 - (ui.y - 21) * 0.2, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=1,y=21
		else if (ui.x == 2 and (ui.y / 100) == 21 || ui.x == 1 and ui.y == 21 and click == 1) {
			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.55 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��ü);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��ü);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				//------����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.35, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_�޻�x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_�޻�z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.8, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.25, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.4, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.4, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -2.4, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (click == 1 and ui.y == 2101)
			{
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if(click == 0) {
				if (ui.y <= 2102) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2101) * 0.15, -1.35, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2103) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2105) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2104) * 0.15, -1.65, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2107) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2106) * 0.15, -1.8, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2110) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2108) * 0.15, -1.95, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2112) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2111) * 0.15, -2.1, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2114) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2113) * 0.15, -2.25, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2117) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2115) * 0.15, -2.4, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.56 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				//------����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.8, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.25, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.4, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.4, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -2.4, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=1,y=22
		else if (ui.x == 2 and (ui.y / 100) == 22 || ui.x == 1 and ui.y == 22 and click == 1) {
			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.55 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��ü);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��ü);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				//------����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_��y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.5, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_�޹�x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_����x);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (click == 1 and ui.y == 2201)
			{
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (click == 0){
				if (ui.y <= 2201) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 , -1.35, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2203) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2202) * 0.15, -1.5, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2205) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2204) * 0.15, -1.65, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2206) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2208) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2207) * 0.15, -1.95, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2210) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2209) * 0.15, -2.1, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2211) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui����_��);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.56 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				//------����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.5, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=0,y=3
		else if (ui.x == 1 and (ui.y / 10) == 3 || ui.x == 0 and ui.y == 3 and click == 1) {
		glBindVertexArray(ui1_vao);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_����);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindVertexArray(ui2_vao);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_���);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_�ҷ�����);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		if (click == 1 and (ui.y / 10) == 3)
		{
			if (c_size != 1) {
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui����_��);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		else {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15 - (ui.y - 31) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		}
		else {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15 - (ui.y - 31) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		{
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		}
		//x=0,y=4
		else if (ui.x == 1 and (ui.y / 10) == 4 || ui.x == 0 and ui.y == 4 and click == 1) {
		glBindVertexArray(ui1_vao);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_ĳ��);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_��Ʈ��);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_����);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_�ִϸ��̼�);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindVertexArray(ui2_vao);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_����);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//�� ��ȯ
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ���λ�);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		if (animation == 1) {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35 - (ui.y - 41) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (click == 1 and (ui.y / 10) == 4)
		{
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35 - (ui.y - 41) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		{
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui����_��);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		}

		//-----------�±�V
		if (character == 1) {
			{//�Ӹ�
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-�Ʒ�

				glBindTexture(GL_TEXTURE_2D, ��_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-��

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//��1
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += (0.1 +0.03);
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] += (0.8 - 0.2);
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.7, 0.7, 0.7));
				mTransform = glm::translate(mTransform, glm::vec3(0.03, 0.09, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.7, 0.7, 0.7));
				mTransform = glm::translate(mTransform, glm::vec3(-0.03, 0.09, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= (0.1 + 0.03)*2;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += (0.1 + 0.03);
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] -= (0.8 -0.2);
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//��2
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//����
				glBindVertexArray(hexa����_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, 30);

				glBindTexture(GL_TEXTURE_2D, ��_��V);
				glDrawArrays(GL_TRIANGLES, 30, 6);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveDown(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveUp(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//�Ϲ�
				glBindVertexArray(hexa�Ϲ�_vao);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveDown(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveUp(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				//---����
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][1] += 0.15;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.4, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.85, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.4, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.85, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][1] -= 0.15;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//�ָ�
				MoveDown(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveDown(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveDown(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2l_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveRight(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveDown(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveRight(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2r_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveUp(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
			}
			{//�㸮
				glBindVertexArray(hexa�㸮_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����
				glBindVertexArray(hexa�����_vao);

				MoveDown(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
			}
			{//���Ƹ�
				glBindVertexArray(hexa���Ƹ�_vao);

				MoveDown(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
			}
			{//��
				glBindVertexArray(hexa��_vao);

				MoveDown(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveForward(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޹�_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(������_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveBack(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
			}
		}
		//-----------��¡��Z
		else if (character == 2) {
			{//�Ӹ�
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-�Ʒ�

				glBindTexture(GL_TEXTURE_2D, ��_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-��

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//�հ�
				glBindVertexArray(���հ�_vao);

				MoveUp(&���հ�_vao, ���հ�_vbo, 0.15 + 0.25 + 0.075, ���հ�_s, ���հ�_vn, ���հ�_vt);
				MoveLeft(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);
				MoveRight(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveDown(&���հ�_vao, ���հ�_vbo, 0.15 + 0.25 + 0.075, ���հ�_s, ���հ�_vn, ���հ�_vt);
				MoveLeft(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);
			}
			{//��
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//�Ҽ���3�ڸ����� �ݿø�
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//����
				glBindVertexArray(hexa����_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveDown(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveUp(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//�Ϲ�
				glBindVertexArray(hexa�Ϲ�_vao);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveDown(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveUp(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
			}
			{//�ָ�
				MoveDown(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveDown(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveDown(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2l_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveRight(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveDown(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveRight(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2r_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveUp(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
			}
			{//�㸮
				glBindVertexArray(hexa�㸮_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����
				glBindVertexArray(hexa�����_vao);

				MoveDown(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
			}
			{//���Ƹ�
				glBindVertexArray(hexa���Ƹ�_vao);

				MoveDown(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
			}
			{//��
				glBindVertexArray(hexa��_vao);

				MoveDown(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveForward(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޹�_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(������_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ����);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveBack(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
			}
			{//��¡�� ����V
				glBindVertexArray(������V_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.45, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��_��V);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//-----------��Ƽ�ӽ� ������
		else if (character == 3) {
			{//�Ӹ�
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-�Ʒ�

				glBindTexture(GL_TEXTURE_2D, ��_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-��

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//������
				glBindVertexArray(���հ�_vao);

				MoveUp(&���հ�_vao, ���հ�_vbo, 0.15 + 0.25 + 0.075, ���հ�_s, ���հ�_vn, ���հ�_vt);
				MoveLeft(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 1, 0.3));
				mTransform = glm::translate(mTransform, glm::vec3(-0.25, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);
				MoveRight(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 1, 0.3));
				mTransform = glm::translate(mTransform, glm::vec3(0.25, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveDown(&���հ�_vao, ���հ�_vbo, 0.15 + 0.25 + 0.075, ���հ�_s, ���հ�_vn, ���հ�_vt);
				MoveLeft(&���հ�_vao, ���հ�_vbo, 0.2, ���հ�_s, ���հ�_vn, ���հ�_vt);
			}
			{//����
				glBindVertexArray(hexa����_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveDown(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveUp(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//�Ϲ�
				glBindVertexArray(hexa�Ϲ�_vao);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveDown(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveUp(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
			}
			{//�ָ�
				MoveDown(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveDown(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveDown(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2l_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveRight(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveDown(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveRight(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2r_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveUp(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
			}
			{//�㸮
				glBindVertexArray(hexa�㸮_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����
				glBindVertexArray(hexa�����_vao);

				MoveDown(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
			}
			{//���Ƹ�
				glBindVertexArray(hexa���Ƹ�_vao);

				MoveDown(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
			}
			{//��
				glBindVertexArray(hexa��_vao);

				MoveDown(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveForward(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޹�_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(������_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��Ƽ_�Ķ���);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveBack(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
			}
		}
		//-----------����
		else if (character == 4) {
			{//�Ӹ�
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-�Ʒ�

				glBindTexture(GL_TEXTURE_2D, ��_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-��

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{// �Ӹ�ī��
				glBindVertexArray(pyramid_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 1, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(-20.0f), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.4, 0.37, 0.3));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 1));
				mTransform = glm::rotate(mTransform, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-120.0f), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.43, 0.1, 0.51));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

			}
			{//����
				glBindVertexArray(hexa����_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveDown(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveUp(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//�Ϲ�
				glBindVertexArray(hexa�Ϲ�_vao);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveDown(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveUp(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
			}
			{//�ָ�
				MoveDown(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveDown(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveDown(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2l_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveRight(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveDown(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveRight(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2r_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveUp(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
			}
			{//�㸮
				glBindVertexArray(hexa�㸮_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ���λ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����
				glBindVertexArray(hexa�����_vao);

				MoveDown(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, �����Ǻλ�);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
			}
			{//���Ƹ�
				glBindVertexArray(hexa���Ƹ�_vao);

				MoveDown(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
			}
			{//��
				glBindVertexArray(hexa��_vao);

				MoveDown(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveForward(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޹�_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(������_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ������);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveBack(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
			}
		}
		//-----------�͹̳����� t-850
		else if (character == 5) {
			{//�Ӹ�
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(��_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ��_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-��

				glBindTexture(GL_TEXTURE_2D, ��_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-�Ʒ�

				glBindTexture(GL_TEXTURE_2D, ��_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-��

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//����
				glBindVertexArray(hexa����_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveDown(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveRight(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa���_vao, hexa���_vbo, 0.65, hexa���_s, hexa���_vn, hexa���_vt);
				MoveUp(&hexa���_vao, hexa���_vbo, 0.28, hexa���_s, hexa���_vn, hexa���_vt);
			}
			{//�Ϲ�
				glBindVertexArray(hexa�Ϲ�_vao);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveDown(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveRight(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.65, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
				MoveUp(&hexa�Ϲ�_vao, hexa�Ϲ�_vbo, 0.3, hexa�Ϲ�_s, hexa�Ϲ�_vn, hexa�Ϲ�_vt);
			}
			{//�ָ�
				MoveDown(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveDown(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveDown(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2l_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�޻��_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveRight(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.65, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�2l_vao, hexa�ָ�2l_vbo, 0.05, hexa�ָ�2l_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveRight(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);

				MoveDown(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveRight(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);

				glBindVertexArray(hexa�ָ�1_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�2r_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa�ָ�3_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(����_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ϲ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����ָ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.15, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveLeft(&hexa�ָ�1_vao, hexa�ָ�1_vbo, 0.65, hexa�ָ�1_s, hexa�ָ�1_vn, hexa�ָ�1_vt);
				MoveUp(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.05, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveLeft(&hexa�ָ�2r_vao, hexa�ָ�2r_vbo, 0.65, hexa�ָ�2r_s, hexa�ָ�2l_vn, hexa�ָ�2l_vt);
				MoveUp(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.05, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
				MoveLeft(&hexa�ָ�3_vao, hexa�ָ�3_vbo, 0.65, hexa�ָ�3_s, hexa�ָ�3_vn, hexa�ָ�3_vt);
			}
			{//�㸮
				glBindVertexArray(hexa�㸮_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���
				glBindVertexArray(hexa���_vao);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����
				glBindVertexArray(hexa�����_vao);

				MoveDown(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveRight(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa�����_vao, hexa�����_vbo, 0.3, hexa�����_s, hexa�����_vn, hexa�����_vt);
				MoveLeft(&hexa�����_vao, hexa�����_vbo, 0.2, hexa�����_s, hexa�����_vn, hexa�����_vt);
			}
			{//���Ƹ�
				glBindVertexArray(hexa���Ƹ�_vao);

				MoveDown(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveRight(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.3, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
				MoveLeft(&hexa���Ƹ�_vao, hexa���Ƹ�_vbo, 0.2, hexa���Ƹ�_s, hexa���Ƹ�_vn, hexa���Ƹ�_vt);
			}
			{//��
				glBindVertexArray(hexa��_vao);

				MoveDown(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveForward(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�����Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�޹�_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveRight(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(���_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(���������_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(�������Ƹ�_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(������_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ȸ��);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
				MoveLeft(&hexa��_vao, hexa��_vbo, 0.2, hexa��_s, hexa��_vn, hexa��_vt);
				MoveBack(&hexa��_vao, hexa��_vbo, 0.1, hexa��_s, hexa��_vn, hexa��_vt);
			}
		}

		{
			//����ü �� ���߿� �׷�����
			//UI
			//ȭ��ǥ 
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(-0.1, -1.85, 4.9));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_��ȭ);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.1, -1.85, 4.9));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_��ȭ);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(-0.45, -1.85, 4.9));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_�ʱ�ȭ);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(-0.55, -1.1, 4.75));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_������);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.89, -0.63, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_�ݽð�);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(1.06, -0.63, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_�ð�);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}


	//glDisable(GL_BLEND);	//�����Ȱ��ȭ

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

//value == 1: â ��������(Ŭ���� ĭ�� Ȯ��), 0: ���󺸱�
void TimerX0Down(int value) {
	if (ui.x == 1 and (ui.y / 10) == 1) {
		if (value == 1 and x0_d > -0.8 or value == 1 and c_size < 1) {
			if (x0_d > -0.8)
				x0_d -= 0.05;
			if (c_size < 1)
				c_size += 0.05;
			glutTimerFunc(30, TimerX0Down, 1);
		}
		else click = 0;
	}
	else if (ui.x == 0 and ui.y == 1) {
		if (value == 0 and x0_d < -0.05 or value == 0 and c_size > 0.05) {
			if (x0_d < -0.05)
				x0_d += 0.05;
			if (c_size > 0.05)
				c_size -= 0.05;
			glutTimerFunc(30, TimerX0Down, 0);
		}
		else click = 0;
	}

	else if (ui.x == 1 and (ui.y / 10) == 2 and c_size <= 1) {
		if (value == 1 and x0_d > -0.2 or value == 1 and c_size < 1) {
			if (x0_d > -0.2)
				x0_d -= 0.05;
			if (c_size < 1)
				c_size += 0.05;
			glutTimerFunc(30, TimerX0Down, 1);
		}
		else {
			x0_d = -0.2;
			c_size = 1.0;
			click = 0;
		}
	}
	else if (ui.x == 0 and ui.y == 2) {
		if (value == 0 and x0_d < -0.05 or value == 0 and c_size > 0.05) {
			if (x0_d < -0.05)
				x0_d += 0.05;
			if (c_size > 0.05)
				c_size -= 0.05;
			glutTimerFunc(30, TimerX0Down, 0);
		}
		else click = 0;
	}

	else if (ui.x == 2 and (ui.y / 100) == 21) {
		if (value == 1 and x0_d > -1.15 or value == 1 and c_size < 2) {
			if (x0_d > -1.15)
				x0_d -= 0.05;
			if (c_size < 2)
				c_size += 0.05;
			glutTimerFunc(25, TimerX0Down, 1);
		}
		else click = 0;
	}
	else if (ui.x == 1 and ui.y == 21 and c_size > 1) {
		if (value == 0 and x0_d < -0.25 or value == 0 and c_size > 1.05) {
			if (x0_d < -0.25)
				x0_d += 0.05;
			if (c_size > 1.05)
				c_size -= 0.05;
			glutTimerFunc(25, TimerX0Down, 0);
		}
		else {
			x0_d = -0.2;
			c_size = 1.0;
			click = 0;
		}
	}

	else if (ui.x == 2 and (ui.y / 100) == 22) {
		if (value == 1 and x0_d > -1.05 or value == 1 and c_size < 2) {
			if (x0_d > -1.05)
				x0_d -= 0.05;
			if (c_size < 2)
				c_size += 0.05;
			glutTimerFunc(25, TimerX0Down, 1);
		}
		else click = 0;
	}
	else if (ui.x == 1 and ui.y == 22 and c_size > 1) {
		if (value == 0 and x0_d < -0.25 or value == 0 and c_size > 1.05) {
			if (x0_d < -0.25)
				x0_d += 0.05;
			if (c_size > 1.05)
				c_size -= 0.05;
			glutTimerFunc(25, TimerX0Down, 0);
		}
		else {
			x0_d = -0.2;
			c_size = 1.0;
			click = 0;
		}
	}

	else if (ui.x == 1 and (ui.y / 10) == 3) {
		if (value == 1 and x0_d > -0.2 or value == 1 and c_size < 1) {
			if (x0_d > -0.2)
				x0_d -= 0.05;
			if (c_size < 1)
				c_size += 0.05;
			glutTimerFunc(30, TimerX0Down, 1);
		}
		else {
			c_size = 1;
			x0_d = -0.2;
			click = 0;
		}
	}
	else if (ui.x == 0 and ui.y == 3) {
		if (value == 0 and x0_d < -0.05 or value == 0 and c_size > 0.05) {
			if (x0_d < -0.05)
				x0_d += 0.05;
			if (c_size > 0.05)
				c_size -= 0.05;
			glutTimerFunc(30, TimerX0Down, 0);
		}
		else click = 0;
	}

	else if (ui.x == 1 and (ui.y / 10) == 4) {
		if (value == 1 and c_size < 1) {
			if (c_size < 1)
				c_size += 0.05;
			glutTimerFunc(30, TimerX0Down, 1);
		}
		else {
			c_size = 1;
			click = 0;
		}
	}
	else if (ui.x == 0 and ui.y == 4) {
		if (value == 0 and c_size > 0.05) {
			if (c_size > 0.05)
				c_size -= 0.05;
			glutTimerFunc(30, TimerX0Down, 0);
		}
		else click = 0;
	}

	glutPostRedisplay();
}

void TimerRed(int value) {
	if(value > 0){
		value -= 1;

		glutTimerFunc(100, TimerRed, value);
		glutPostRedisplay();
	}
	else
		click = 0;
}

void TimerAnimation_1(int value) {
	if (value < 42) {
		if (����_y > (-3) * 6)					����_y -= 3;							 
		if (�޻��_x > (-3) * 4)			�޻��_x -= 3;
		if (�޻��_z < 3 * 5)			�޻��_z += 3;
		if (���Ϲ�_x > (-3) * 42)		���Ϲ�_x -= 3;
		if (���Ϲ�_z < 3 * 3)		���Ϲ�_z += 3;
		if (�������_x > (-3) * 10)		�������_x -= 3;							 
		if (�������_z < 3 * 3)		�������_z += 3;							 
		if (�����Ϲ�_x > (-3) * 40)		�����Ϲ�_x -= 3;						 
		if (�����Ϲ�_z > (-3) * 3)		�����Ϲ�_z -= 3;	
		if (���������_z < 3 * (3))	���������_z += 3;
		if (�������Ƹ�_x < 3 * 3)		�������Ƹ�_x += 3;
		if (�������Ƹ�_z > -3 * 2)		�������Ƹ�_z -= 3;
		if (�������_x < 3 * 3)		�������_x += 3;
		if (�������_z > -3 * 2)		�������_z -= 3;
		if (�����Ƹ�_x < 3 * 12)	�����Ƹ�_x += 3;
		if (�����Ƹ�_z < 3 * 2)	�����Ƹ�_z += 3;
			
		value++;
		glutTimerFunc(30, TimerAnimation_1, value);
	}
	else if (value < 62) {
		if (����_y > (-3) * 3 -18)			����_y -= 3;
		if (�������_x > (-3) * 20-30)		�������_x -= 3;
		if (�����Ϲ�_x < 3 * 40-120)		�����Ϲ�_x += 6;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else if (value < 82) {
		if (����_y < 3 * 3-18-9)			����_y += 3;
		if (�������_x < 3 * 20-30-3*20)		�������_x += 3;
		if (�����Ϲ�_x > (-3) * 40)		�����Ϲ�_x -= 6;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else if (value < 105) {
		if (����_y < (3) * 10 - 18)			����_y += 3;
		if (�޻��_x > (-3) * 23 - 12)		�޻��_x -= 3;
		if (�޻��_z > (-3) * 5 + 15)		�޻��_z -= 3;
		if (���Ϲ�_x < 3 * 40 - 126)		���Ϲ�_x += 6;
		if (���_y < (3) * 5 )			���_y += 3;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else if (value < 128) {
		if (����_y > (-3) * 10 - 18 +3*10)			����_y -= 3;
		if (�޻��_x < (3) * 23 - 12 -3*23)		�޻��_x += 3;
		if (�޻��_z < (3) * 5 )			�޻��_z += 3;
		if (���Ϲ�_x > (-3) * 40 - 6)		���Ϲ�_x -= 6;
		if (���_y > (-3) * 5)			���_y -= 3;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else
		animation = 0;
	
	glutPostRedisplay();
	
}

//ī�޶� ȭ���� �߽�y ���� ����
void TimerRotate_CAMERA_Y(int value)
{
	float cp;

	//�������� ���� ��ӿ�Ѵ�.
	if (value == 1)
		cp = 3;
	else
		cp = -3;	

	float temp_cx = cameraPos.x, temp_cz = cameraPos.z;
	float temp_lx = light_x, temp_lz = light_z;

	//ī�޶� ȸ��
	cameraPos.x = temp_cx * glm::cos(glm::radians(cp)) + temp_cz * glm::sin(glm::radians(cp));	//cp��ŭ ����(ȸ��)���ٴ� ���̹Ƿ�
	cameraPos.z = temp_cz * glm::cos(glm::radians(cp)) - temp_cx * glm::sin(glm::radians(cp));
	//���� ȸ��
	light_x = temp_lx * glm::cos(glm::radians(cp)) + temp_lz * glm::sin(glm::radians(cp));	//cp��ŭ ����(ȸ��)���ٴ� ���̹Ƿ�
	light_z = temp_lz * glm::cos(glm::radians(cp)) - temp_lx * glm::sin(glm::radians(cp));

	glutPostRedisplay();
}

void TimerLogo_size(int value)
{
	if (start == 0) {
		if (value == 0) {
			if (startlogo_size <= 1.5) {
				startlogo_size += 0.01;
				glutPostRedisplay();
				glutTimerFunc(20, TimerLogo_size, 0);
			}
			else {
				glutPostRedisplay();
				glutTimerFunc(20, TimerLogo_size, 1);
			}
		}
		else
		{
			if (startlogo_size >= 0.7) {
				startlogo_size -= 0.01;
				glutPostRedisplay();
				glutTimerFunc(20, TimerLogo_size, 1);
			}
			else {
				glutPostRedisplay();
				glutTimerFunc(20, TimerLogo_size, 0);
			}
		}
	}
}

//right - 1: ������Ŭ��, 0: ����Ŭ��
void Move_X(int* x, int* y,bool right) {
	if (*x == 0) 
		return;
	else if (*x == 1) {
		if (right == 1)
			return;
		else {
			*y /= 10;
			*x -= 1;
			click = 1;
			glutTimerFunc(0, TimerX0Down, 0);
		}
	}
	else if (*x == 2) {
		if (right == 1)
			return;
		else {
			*y /= 100;
			*x -= 1;
			click = 1;
			glutTimerFunc(0, TimerX0Down, 0);
		}
	}
}

//up - 1: ����Ŭ��, 0: �Ʒ���Ŭ��
void Move_Y(int* x, int* y, bool up) {
	if (*x == 0)
	{
		if (up == 1) {
			if (*y == 1)
				*y = 4;
			else
				*y -= 1;
		}
		else {
			if (*y == 4)
				*y = 1;
			else
				*y += 1;
		}
	}
	else if (*x == 1) {
		if ((*y / 10)==1) {
			if (up == 1) {
				if (*y == 11) {
					*y = 15;
				}
				else
					*y -= 1;
			}
			else {
				if (*y == 15)
					*y = 11;
				else
					*y += 1;
			}
		}
		else{
			if (up == 1) {
				if (*y == 21 || *y == 31 || *y == 41)
					*y += 1;
				else
					*y -= 1;
			}
			else {
				if (*y == 22 || *y == 32 || *y == 42)
					*y -= 1;
				else
					*y += 1;
			}
		}
	}
	else {
		if (*y < 2200) {
			if (up == 1) {
				if (*y == 2101)
					*y = 2117;
				else
					*y -= 1;
			}
			else {
				if (*y == 2117)
					*y = 2101;
				else
					*y += 1;
			}
		}
		else {
			if (up == 1) {
				if (*y == 2201)
					*y = 2211;
				else
					*y -= 1;
			}
			else {
				if (*y == 2211)
					*y = 2201;
				else
					*y += 1;
			}
		}

	}
}

void Enter(int* x, int* y) {
	if (*x == 0) {
		*y *= 10;
		*y += 1;
		*x += 1;
		click = 1;
		glutTimerFunc(0, TimerX0Down, 1);
	}
	else if (*x == 1 and (*y / 10) == 2) {
		*y *= 100;
		*y += 1;
		*x += 1;
		click = 1;
		glutTimerFunc(0, TimerX0Down, 1);
	}
	else if (*x == 1 and (*y / 10) == 1) {
		character = *y - 10;
		click = 1;
		if(character == 1)
			PlaySound(L"wav/�±�V.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);	
		else if (character == 2)
			PlaySound(L"wav/��¡��Z.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		else if (character == 3)
			PlaySound(L"wav/Ʈ��������.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		else if (character == 4)
			PlaySound(L"wav/����.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		else if (character == 5)
			PlaySound(L"wav/�͹̳�����.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		glutTimerFunc(0, TimerRed, 7);
	}
	else if (*x == 1 and (*y / 10) == 3) {
		if (*y == 31){
			ofstream out{ "Pose.txt" };
			out<< character << ' ';
			out << ��_x << ' '; out << ��_y << ' ';
			out << ����_y << ' ';
			out << �޻��_x << ' '; out << �޻��_z << ' ';
			out << ���Ϲ�_x << ' '; out << ���Ϲ�_z << ' ';
			out << ���ָ�_x << ' '; out << ���ָ�_y << ' '; out << ���ָ�_z << ' ';
			out << �������_x << ' '; out << �������_z << ' ';
			out << �����Ϲ�_x << ' '; out << �����Ϲ�_z << ' ';
			out << �����ָ�_x << ' '; out << �����ָ�_y << ' '; out << �����ָ�_z << ' ';
			out << ���_y << ' ';
			out << �������_x << ' '; out << �������_z << ' ';
			out << �����Ƹ�_x << ' '; out << �����Ƹ�_z << ' ';
			out << �޹�_x << ' ';
			out << ���������_x << ' '; out << ���������_z << ' ';
			out << �������Ƹ�_x << ' '; out << �������Ƹ�_z << ' ';
			out << ������_x << ' ';
		}
		else{
			ifstream in{ "Pose.txt" };
			in >> character;
			in >> ��_x; in >> ��_y;
			in >> ����_y;
			in >> �޻��_x ; in >> �޻��_z;
			in >> ���Ϲ�_x ; in >> ���Ϲ�_z;
			in >> ���ָ�_x ; in >> ���ָ�_y; in >> ���ָ�_z;
			in >> �������_x ;in >> �������_z;
			in >> �����Ϲ�_x; in >> �����Ϲ�_z;
			in >> �����ָ�_x; in >> �����ָ�_y; in >> �����ָ�_z;
			in >> ���_y ;
			in >> �������_x; in >> �������_z;
			in >> �����Ƹ�_x; in >> �����Ƹ�_z;
			in >> �޹�_x;
			in >> ���������_x; in >> ���������_z;
			in >> �������Ƹ�_x; in >> �������Ƹ�_z;
			in >> ������_x;

			if (character == 1)
				PlaySound(L"wav/�±�V.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 2)
				PlaySound(L"wav/��¡��Z.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 3)
				PlaySound(L"wav/Ʈ��������.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 4)
				PlaySound(L"wav/����.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 5)
				PlaySound(L"wav/�͹̳�����.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		
		click = 1;
		glutTimerFunc(0, TimerRed, 7);
	}
	else if (*x == 1 and (*y / 10) == 4) {
		animation = 1;

		//��ݽ�
		��_x = 0, ��_y = 0,
		����_y = 0,
		�޻��_x = 0, �޻��_z = 0, �������_x = 0, �������_z = 0,
		���Ϲ�_x = 0, ���Ϲ�_z = 0, �����Ϲ�_x = 0, �����Ϲ�_z = 0,
		���ָ�_x = 0, ���ָ�_y = 0, ���ָ�_z = 0, �����ָ�_x = 0, �����ָ�_y = 0, �����ָ�_z = 0,
		//�Ϲݽ�
		���_y = 0,
		�������_x = 0, �������_z = 0, ���������_x = 0, ���������_z = 0,
		�����Ƹ�_x = 0, �����Ƹ�_z = 0, �������Ƹ�_x = 0, �������Ƹ�_z = 0,
		�޹�_x = 0, ������_x = 0;
		
		
		glutTimerFunc(500, TimerAnimation_1, 0);
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	if (start == 1) {
		switch (key) {
		case '\r':
			if (click == 0 and animation == 0)
				Enter(&ui.x, &ui.y);
			break;

		case 'p':
		case 'P':
			if (projection == 1)
				projection = 0;
			else projection = 1;
			break;

		/*case 'a':
			move_x -= 0.05;
			break;
		case 'd':
			move_x += 0.05;
			break;
		case 'A':
			move_x -= 0.01;
			break;
		case 'D':
			move_x += 0.01;
			break;
		case 'w':
			move_y += 0.05;
			break;
		case 's':
			move_y -= 0.05;
			break;
		case 'W':
			move_y += 0.01;
			break;
		case 'S':
			move_y -= 0.01;
			break;
		case 'z':
			move_z -= 0.05;
			break;
		case 'Z':
			move_z += 0.05;
			break;
		case '[':
			move_z -= 0.01;
			break;
		case ']':
			move_z += 0.01;
			break;*/

		case '+': 
			if (character != 0) {
				{//��ݽ� ���� ����
					if (ui.y == 2101) ��_x -= 3;
					else if (ui.y == 2102) ��_y -= 3;
					else if (ui.y == 2103) ����_y -= 3;
					else if (ui.y == 2104) �޻��_x -= 3;
					else if (ui.y == 2105) �޻��_z -= 3;
					else if (ui.y == 2106) ���Ϲ�_x -= 3;
					else if (ui.y == 2107) ���Ϲ�_z -= 3;
					else if (ui.y == 2108) ���ָ�_x -= 3;
					else if (ui.y == 2109) ���ָ�_y -= 3;
					else if (ui.y == 2110) ���ָ�_z -= 3;
					else if (ui.y == 2111) �������_x -= 3;
					else if (ui.y == 2112) �������_z -= 3;
					else if (ui.y == 2113) �����Ϲ�_x -= 3;
					else if (ui.y == 2114) �����Ϲ�_z -= 3;
					else if (ui.y == 2115) �����ָ�_x -= 3;
					else if (ui.y == 2116) �����ָ�_y -= 3;
					else if (ui.y == 2117) �����ָ�_z -= 3;
				}
				{//�Ϲݽ� ���� ����
					if (ui.y == 2201) ���_y -= 3;
					else if (ui.y == 2202) �������_x -= 3;
					else if (ui.y == 2203) �������_z -= 3;
					else if (ui.y == 2204) �����Ƹ�_x -= 3;
					else if (ui.y == 2205) �����Ƹ�_z -= 3;
					else if (ui.y == 2206) �޹�_x -= 3;
					else if (ui.y == 2207) ���������_x -= 3;
					else if (ui.y == 2208) ���������_z -= 3;
					else if (ui.y == 2209) �������Ƹ�_x -= 3;
					else if (ui.y == 2210) �������Ƹ�_z -= 3;
					else if (ui.y == 2211) ������_x -= 3;

				}
			}
			break;
		case '-':
			if (character != 0) {
				{//��ݽ� ���� ����
					if (ui.y == 2101) ��_x += 3;
					else if (ui.y == 2102) ��_y += 3;
					else if (ui.y == 2103) ����_y += 3;
					else if (ui.y == 2104) �޻��_x += 3;
					else if (ui.y == 2105) �޻��_z += 3;
					else if (ui.y == 2106) ���Ϲ�_x += 3;
					else if (ui.y == 2107) ���Ϲ�_z += 3;
					else if (ui.y == 2108) ���ָ�_x += 3;
					else if (ui.y == 2109) ���ָ�_y += 3;
					else if (ui.y == 2110) ���ָ�_z += 3;
					else if (ui.y == 2111) �������_x += 3;
					else if (ui.y == 2112) �������_z += 3;
					else if (ui.y == 2113) �����Ϲ�_x += 3;
					else if (ui.y == 2114) �����Ϲ�_z += 3;
					else if (ui.y == 2115) �����ָ�_x += 3;
					else if (ui.y == 2116) �����ָ�_y += 3;
					else if (ui.y == 2117) �����ָ�_z += 3;
				}
				{//�Ϲݽ� ���� ����
					if (ui.y == 2201) ���_y += 3;
					else if (ui.y == 2202) �������_x += 3;
					else if (ui.y == 2203) �������_z += 3;
					else if (ui.y == 2204) �����Ƹ�_x += 3;
					else if (ui.y == 2205) �����Ƹ�_z += 3;
					else if (ui.y == 2206) �޹�_x += 3;
					else if (ui.y == 2207) ���������_x += 3;
					else if (ui.y == 2208) ���������_z += 3;
					else if (ui.y == 2209) �������Ƹ�_x += 3;
					else if (ui.y == 2210) �������Ƹ�_z += 3;
					else if (ui.y == 2211) ������_x += 3;

				}
			}
			break;

		case 'n':
		case 'N':
			ui.a += 3;
			TimerRotate_CAMERA_Y(1);
			break;
		case 'm':
		case 'M':
			ui.a -= 3;
			glutTimerFunc(0, TimerRotate_CAMERA_Y, 0);
			break;

		case 'C':
		case 'c':
			animation = 0;
			projection = 1;
			//move_x = 0; move_y = 0; move_z = 0;
			cameraPos.x = 0; cameraPos.y = -1.5; cameraPos.z = 6.0;
			light_x = 0; light_y = 5.0; light_z = 10.0;
			ui.x = 0; ui.y = 1; ui.a = 0;
			character = 0;
			click = 0;
			c_size = 0;
			x0_d = 0;
			//��ݽ�
			��_x = 0, ��_y = 0,
			����_y = 0,
			�޻��_x = 0, �޻��_z = 0, �������_x = 0, �������_z = 0,
			���Ϲ�_x = 0, ���Ϲ�_z = 0, �����Ϲ�_x = 0, �����Ϲ�_z = 0,
			���ָ�_x = 0, ���ָ�_y = 0, ���ָ�_z = 0, �����ָ�_x = 0, �����ָ�_y = 0, �����ָ�_z = 0,
			//�Ϲݽ�
			���_y = 0,
			�������_x = 0, �������_z = 0, ���������_x = 0, ���������_z = 0,
			�����Ƹ�_x = 0, �����Ƹ�_z = 0, �������Ƹ�_x = 0, �������Ƹ�_z = 0,
			�޹�_x = 0, ������_x = 0;
			PlaySound(NULL, 0, 0);
			break;

		case 32:
			//printf("%.2f,%.2f,%.2f\n", move_x, move_y, move_z);
			//printf("%d, %d\n", ui.x, ui.y);
			break;

		case 27:
			exit(0);
			break;

			//InitBuffer();
		}
	}
	else
	{
		if (key == '\r') {
			start = 1;
		}
		else if (key == 27)
			exit(0);
	}
	
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	if (start == 1) {
		switch (key) {
		case GLUT_KEY_RIGHT:
			if(click == 0 and animation == 0)
				Move_X(&ui.x, &ui.y, 1);
			break;
		case GLUT_KEY_LEFT:
			if (click == 0 and animation == 0)
				Move_X(&ui.x, &ui.y, 0);
			break;
		case GLUT_KEY_UP:
			if (click == 0 and animation == 0)
				Move_Y(&ui.x, &ui.y, 1);
			break;
		case GLUT_KEY_DOWN:
			if (click == 0 and animation == 0)
				Move_Y(&ui.x, &ui.y, 0);
			break;
		}

		glutPostRedisplay();
	}
}

void Mouse(int b, int s, int x, int y)
{
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(10, 0);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Real Steel");

	glewExperimental = GL_TRUE;
	glewInit();

	AssignObj_Variable();
	InitShader();
	InitBuffer();
	InitTexture();

	glutTimerFunc(0, TimerLogo_size, 0);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}