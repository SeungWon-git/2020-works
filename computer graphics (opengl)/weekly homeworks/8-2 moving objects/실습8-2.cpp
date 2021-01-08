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

//�ּ�(���)
#define hexahedronpoints	36	//-> �� ���� ���� = �ﰢ��(������) ���� * 3(������)
#define pyramidpoints		18
#define spherepoints		240

GLfloat hexa_s[hexahedronpoints][3] = {};
GLfloat floor_s[12][3] = {
	{ -0.25,-0.25,-0.25 }, { 0.0,-0.25,0.0 }, {0.25,-0.25,-0.25},	//��
	{ 0.25,-0.25,-0.25 }, { 0.0,-0.25,0.0 }, {0.25,-0.25,0.25},		//��
	{ 0,-0.25,0.0 }, { -0.25,-0.25,0.25 }, {0.25,-0.25,0.25},		//��
	{-0.25,-0.25,-0.25},{-0.25,-0.25,0.25},{0,-0.25,0}				//��
};
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat sphere_s[spherepoints][3] = {};



//����
GLfloat ����_c[hexahedronpoints][3] = {};
GLfloat floor_c[12][3] = {
	{0.5,0.75,0.27},{0.5,0.75,0.27},{0.5,0.75,0.27},	//��-����
	{1,0.5,0},{1,0.5,0},{1,0.5,0},						//����-��Ȳ
	{0.31,0.73,0.87},{0.31,0.73,0.87},{0.31,0.73,0.87},	//��-�ϴ�
	{1,0.2,0.6},{1,0.2,0.6},{1,0.2,0.6}					//��-��ũ
};

GLfloat robot_h_c[hexahedronpoints][3] = {};
GLfloat robot_b_c[hexahedronpoints][3] = {};
GLfloat robot_Larm_c[hexahedronpoints][3] = {}; GLfloat robot_Rarm_c[hexahedronpoints][3] = {};
GLfloat robot_Lleg_c[hexahedronpoints][3] = {}; GLfloat robot_Rleg_c[hexahedronpoints][3] = {};
GLfloat robot_nose_c[pyramidpoints][3] = {};

GLfloat hexa_brown_c[hexahedronpoints][3] = {};
GLfloat hexa_red_c[hexahedronpoints][3] = {};
GLfloat hexa_green_c[hexahedronpoints][3] = {};
GLfloat hexa_blue_c[hexahedronpoints][3] = {};
GLfloat hexa_white_c[hexahedronpoints][3] = {};
GLfloat sphere_green_c[spherepoints][3] = {};
GLfloat sphere_yellow_c[spherepoints][3] = {};

//vao,vbo
GLuint 
//vao
����_vao, floor_vao, 
robot_h_vao, robot_b_vao, robot_Larm_vao, robot_Rarm_vao, robot_Lleg_vao, robot_Rleg_vao, robot_nose_vao,
hexa_brown_vao, hexa_red_vao, hexa_green_vao, hexa_blue_vao, hexa_white_vao, hexa_black_vao,
sphere_green_vao, sphere_yellow_vao, sphere_black_vao,
ö���κ�_����_vao, ö���κ�_������_vao, ö���κ�_����_vao,
��ġ������_��_vao, ��ġ������_�����_vao,
���׸ӽ�_�ٸ�_vao,
//vbo
����_vbo[2], floor_vbo[2], 
robot_h_vbo[2], robot_b_vbo[2], robot_Larm_vbo[2], robot_Rarm_vbo[2], robot_Lleg_vbo[2], robot_Rleg_vbo[2], robot_nose_vbo[2], 
hexa_brown_vbo[2], hexa_red_vbo[2], hexa_green_vbo[2], hexa_blue_vbo[2], hexa_white_vbo[2], hexa_black_vbo[2],
sphere_green_vbo[2], sphere_yellow_vbo[2], sphere_black_vbo[2],
ö���κ�_����_vbo[2], ö���κ�_������_vbo[2], ö���κ�_����_vbo[2],
��ġ������_��_vbo[2], ��ġ������_�����_vbo[2],
���׸ӽ�_�ٸ�_vbo[2]
;

//���� ����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 2.3f);			//ī�޶� ��ġ 
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);	//ī�޶� �ٶ󺸴� ������
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//ī�޶��� ����� ����Ű�� ����

bool perspective = 1;
bool stop_animation = 1;
bool stop_objs = 1;

GLfloat frontscreen_y = 0, frontscreen_r = 0;	//�� �帷 �ȱ�

GLfloat move_x = 0, move_y = -0.17f, move_z = 0.5f;	//�� �̵�
GLfloat direction = 360.0f;//���� �ٶ󺸴� ����[����] ( 0 -> ��, -90 -> ��, 90 -> ��, 180 -> �� )
GLfloat walk = 0.0f;	//�� �ȱ� ����

GLfloat tree_leaf_size = 0.2f;	//������ ũ��
bool tree_leaf_size_up = 1;		//������ ũ�� ����OX

GLfloat ö������ = 0.0f;

GLfloat ��ġ���������� = 90.0f;
bool ��ġ�������������� = 0;		//��ġ������ ���� ����OX
GLfloat ��ġ�����������Ϲڿ�����_x = -0.1, ��ġ�����������Ϲڿ�����_y = 0, ��ġ���������Ϲڿ�����_x = 0.1, ��ġ���������Ϲڿ�����_y = 0;

GLfloat ���׸ӽŰ��� = 0.0f;
GLfloat ���׸ӽŷκ��ٸ����� = 0.0f;
bool ���׸ӽŷκ��ٸ��������� = 0;

GLfloat cp_cw = 0.0f;		//ī�޶��� ����
GLfloat CP_Y = 0.0f;		//ī�޶��� ����
GLfloat cd_cw = 0.0f;		//ī�޶� �ٶ󺸴� ����
GLfloat CD_Y = 270.0f;		//ī�޶� �ٶ󺸴� ����




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

void InitModel(){
	// [obj�б�]
	char filename[30] = "������ü.obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexa_s);

	strcpy(filename, "�簢��(�Ƕ�̵�).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, pyramid_s);

	strcpy(filename, "��(2).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, sphere_s);

	fclose(file);	//���ϴݱ�

	// [�� �ʱ�ȭ]
	//����
	for (int i = 0; i < hexahedronpoints; ++i) {
		if (i < 6) {	//�޸�-���� ȸ��
			����_c[i][0] = 0.9f; ����_c[i][1] = 0.9f; ����_c[i][2] = 0.9f;
		}
		else if (i < 12) {	//���ʸ�-ȸ��
			����_c[i][0] = 0.7f; ����_c[i][1] = 0.7f; ����_c[i][2] = 0.7f;
		}
		else if (i < 18) {	//����-���
			����_c[i][0] = 1.0f; ����_c[i][1] = 1.0f; ����_c[i][2] = 1.0f;
		}
		else if (i < 24) {	//�����ʸ�-ȸ��
			����_c[i][0] = 0.7f; ����_c[i][1] = 0.7f; ����_c[i][2] = 0.7f;
		}
		else if (i < 30) {	//�Ʒ���-���(�ε� ������)
			����_c[i][0] = 1.0f; ����_c[i][1] = 1.0f; ����_c[i][2] = 1.0f;
		}
		else if (i < 36) {	//�ո�-���� ȸ��
			����_c[i][0] = 0.9f; ����_c[i][1] = 0.9f; ����_c[i][2] = 0.9f;
		}
	}

	//�κ�
	for (int i = 0; i < pyramidpoints; ++i) {
		//�κ� ��
		robot_nose_c[i][0] = 0.58; robot_nose_c[i][1] = 0.29; robot_nose_c[i][2] = 0.0;	//����
	}
	for (int i = 0; i < hexahedronpoints; ++i) {
		//�κ� ��ü
		robot_h_c[i][0] = 0.9; robot_h_c[i][1] = 0.8; robot_h_c[i][2] = 0.69;	//�Ӹ�-�챸��
		robot_b_c[i][0] = 1.0; robot_b_c[i][1] = 0.0; robot_b_c[i][2] = 0.0;	//����-������
		robot_Rarm_c[i][0] = 1.0; robot_Rarm_c[i][1] = 0.0; robot_Rarm_c[i][2] = 1.0;	//������-�����
		robot_Larm_c[i][0] = 0.4; robot_Larm_c[i][1] = 0.0; robot_Larm_c[i][2] = 0.6;	//����-���ֻ�
		robot_Rleg_c[i][0] = 0.0; robot_Rleg_c[i][1] = 0.0; robot_Rleg_c[i][2] = 1.0;	//�����ٸ�-�Ķ���
		robot_Lleg_c[i][0] = 0.19; robot_Lleg_c[i][1] = 0.77; robot_Lleg_c[i][2] = 0.65;	//�޴ٸ�-����
	}

	//��ü��
	//������
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_brown_c[i][0] = 0.65; hexa_brown_c[i][1] = 0.4; hexa_brown_c[i][2] = 0.0;	//����-���� �ٱ�
		hexa_red_c[i][0] = 1; hexa_red_c[i][1] = 0; hexa_red_c[i][2] = 0;	//������
		hexa_green_c[i][0] = 0; hexa_green_c[i][1] = 1; hexa_green_c[i][2] = 0;	//�ʷϻ�
		hexa_blue_c[i][0] = 0; hexa_blue_c[i][1] = 0; hexa_blue_c[i][2] = 1;	//�Ķ���
		hexa_white_c[i][0] = 1; hexa_white_c[i][1] = 1; hexa_white_c[i][2] = 1;	//�Ͼ��
	}
	//��
	for (int i = 0; i < spherepoints; ++i) {
		sphere_green_c[i][0] = 0.0; sphere_green_c[i][1] = 1.0; sphere_green_c[i][2] = 0.0;		//�ʷ�-���� ��
		sphere_yellow_c[i][0] = 1; sphere_yellow_c[i][1] = 1.0; sphere_yellow_c[i][2] = 0.0;	//�����-����,ö��
	}



	// [�� ��ġ ���� & InitVao]
	InitVao(����_vao, ����_vbo, hexahedronpoints, hexa_s, ����_c);					//����
	InitVao(floor_vao, floor_vbo, 12, floor_s, floor_c);							//���� �ٴ�
	InitVao(robot_h_vao, robot_h_vbo, hexahedronpoints, hexa_s, robot_h_c);			//�κ�
	InitVao(hexa_brown_vao, hexa_brown_vbo, hexahedronpoints, hexa_s, hexa_brown_c);//���� �簢��
	InitVao(hexa_red_vao, hexa_red_vbo, hexahedronpoints, hexa_s, hexa_red_c);//������ �簢��
	InitVao(hexa_green_vao, hexa_green_vbo, hexahedronpoints, hexa_s, hexa_green_c);//�ʷϻ� �簢��
	InitVao(hexa_blue_vao, hexa_blue_vbo, hexahedronpoints, hexa_s, hexa_blue_c);//�Ķ��� �簢��
	InitVao(hexa_white_vao, hexa_white_vbo, hexahedronpoints, hexa_s, hexa_white_c);//�Ͼ�� �簢��
	InitVao(hexa_black_vao, hexa_black_vbo, hexahedronpoints, hexa_s, 0);//������ �簢��
	InitVao(sphere_green_vao, sphere_green_vbo, spherepoints, sphere_s, sphere_green_c);	//�ʷ� ��
	InitVao(sphere_yellow_vao, sphere_yellow_vbo, spherepoints, sphere_s, sphere_yellow_c);	//��� ��
	InitVao(sphere_black_vao, sphere_black_vbo, spherepoints, sphere_s, 0);	//���� ��

	//ö�� �κ�
	//����
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.25; hexa_s[i][2] -= 0.5;
	}
	InitVao(ö���κ�_����_vao, ö���κ�_����_vbo, hexahedronpoints, hexa_s, hexa_blue_c);
	//������
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][2] += 1; 
	}
	InitVao(ö���κ�_������_vao, ö���κ�_������_vbo, hexahedronpoints, hexa_s, hexa_blue_c);
	//����
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.25; hexa_s[i][2] -= 0.5;
	}
	InitVao(ö���κ�_����_vao, ö���κ�_����_vbo, hexahedronpoints, hexa_s, hexa_blue_c);

	for (int i = 0; i < hexahedronpoints; ++i) {	//����ġ
		hexa_s[i][1] += 0.5;
	}
	//std::cout << hexa_s[0][0] << "," << hexa_s[0][1] << "," << hexa_s[0][2] << std::endl;	//Ȯ��

	//��ġ ������
	//��
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] += 0.25;
	}
	InitVao(��ġ������_��_vao, ��ġ������_��_vbo, hexahedronpoints, hexa_s, hexa_blue_c);

	for (int i = 0; i < hexahedronpoints; ++i) {	//����ġ
		hexa_s[i][1] -= 0.25;
	}

	//��ġ ������
	//�ٸ�
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.25;
	}
	InitVao(���׸ӽ�_�ٸ�_vao, ���׸ӽ�_�ٸ�_vbo, hexahedronpoints, hexa_s, hexa_red_c);

	for (int i = 0; i < hexahedronpoints; ++i) {	//����ġ
		hexa_s[i][1] += 0.25;
	}

	//�����¿�� �����̴� �κ�
	//��
	for (int i = 0; i < pyramidpoints; ++i) {
		pyramid_s[i][1] += 0.25;
	}
	InitVao(robot_nose_vao, robot_nose_vbo, pyramidpoints, pyramid_s, robot_nose_c);

	//����
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.38;
	}
	InitVao(robot_b_vao, robot_b_vbo, hexahedronpoints, hexa_s, robot_b_c);

	//����-���ֻ�
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][0] -= 1.5; hexa_s[i][1] -= 0.21;
	}
	InitVao(robot_Larm_vao, robot_Larm_vbo, hexahedronpoints, hexa_s, robot_Larm_c);

	//������-�����
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][0] += 3.0;
	}
	InitVao(robot_Rarm_vao, robot_Rarm_vbo, hexahedronpoints, hexa_s, robot_Rarm_c);

	//�޴ٸ�-����
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][0] -= 1.5; hexa_s[i][0] += 0.7;
	}
	InitVao(robot_Lleg_vao, robot_Lleg_vbo, hexahedronpoints, hexa_s, robot_Lleg_c);

	//�����ٸ�-�Ķ���
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][0] -= 1.4;
	}
	InitVao(robot_Rleg_vao, robot_Rleg_vbo, hexahedronpoints, hexa_s, robot_Rleg_c);
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


	mTransform = glm::mat4(1.0f);	//�� ��ȯ
	mTransform = glm::scale(mTransform, glm::vec3(3.5, 1.5, 3.5));
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

	//���
	{
		{//����
			glBindVertexArray(����_vao);
			glDrawArrays(GL_TRIANGLES, 0, 24);
			glDrawArrays(GL_TRIANGLES, 30, 6);	//�ٴڸ� �� �׸���
		}
		{//�ٴڸ�
			glFrontFace(GL_CCW);			// �ݽð������ �ո�(����ġ)

			glBindVertexArray(floor_vao);
			glDrawArrays(GL_TRIANGLES, 0, 12);
		}
		{//�ո�
			mTransform = glm::rotate(mTransform, glm::radians(frontscreen_r), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0, frontscreen_y, 0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			if (frontscreen_r > -90) {
				glBindVertexArray(����_vao);
				glDrawArrays(GL_TRIANGLES, 30, 6);
			}
		}
		glDisable(GL_CULL_FACE);		// �޸�(�ð�ݴ�������� winding�� �ո�) ���� ����
	}

	//��ü
	{
		//����
		{
			{//���� ���
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0.6, -0.15, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.4, 1.0, 0.4));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_brown_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//���� ��
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.15, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(tree_leaf_size, tree_leaf_size, tree_leaf_size));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_green_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
		}
		//��� ���
		{
			{//�� �ر��
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.17, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.8, 0.5));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_black_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//1��° ����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.8, 0.35, 0.8));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_red_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//2��° ����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, 0.14, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.6, 0.3, 0.6));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_green_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//3��° ����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0, 0.23, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.4, 0.25, 0.4));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_blue_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
		}
		//ö��
		{
			{//ö�� �κ�
				{//����
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.0, -0.6));
					mTransform = glm::rotate(mTransform, glm::radians(ö������), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(ö���κ�_����_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//������
					glBindVertexArray(ö���κ�_������_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//����
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.0, -0.6));
					mTransform = glm::rotate(mTransform, glm::radians(ö������), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(0.2, 0.4, 0.2));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(ö���κ�_����_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
			}
			{//��(������)
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.0, -0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.1, 0.6));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//������2��
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0.6, -0.2, -0.72));
				mTransform = glm::scale(mTransform, glm::vec3(0.07, 0.8, 0.07));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(0.6, -0.2, -0.48));
				mTransform = glm::scale(mTransform, glm::vec3(0.07, 0.8, 0.07));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
		}
		//��ġ ������
		{
			{
				{//��ġ
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.3, -0.6));
					mTransform = glm::scale(mTransform, glm::vec3(0.6, 0.1, 0.8));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{//�Ӹ�
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.25, -0.76));
					mTransform = glm::scale(mTransform, glm::vec3(0.15, 0.15, 0.15));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_red_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//����
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.25, -0.6));
					mTransform = glm::scale(mTransform, glm::vec3(0.25, 0.1, 0.5));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_green_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{//�������
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.64, -0.25, -0.4));
					mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.1, 0.3));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_blue_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.56, -0.25, -0.4));
					mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.1, 0.3));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_blue_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//��ٸ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.64, -0.3, -0.35));
				mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_blue_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.56, -0.3, -0.35));
				mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_blue_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
			}
			//�����̴� �κ�
			{//�������
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(-0.66, -0.25, -0.66));
			mTransform = glm::rotate(mTransform, glm::radians(��ġ����������), glm::vec3(0.0, 0.0, 1.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(��ġ������_��_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����Ϲ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.66, -0.27, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(��ġ�����������Ϲڿ�����_x, ��ġ�����������Ϲڿ�����_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(��ġ������_��_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�޻��
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.54, -0.25, -0.66));
				mTransform = glm::rotate(mTransform, glm::radians(-��ġ����������), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(��ġ������_��_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//���Ϲ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.54, -0.27, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(��ġ���������Ϲڿ�����_x, ��ġ���������Ϲڿ�����_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(��ġ������_��_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.15, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(0, ��ġ���������Ϲڿ�����_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.9, 0.08, 0.08));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//�����ʿ���
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.83, -0.15, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(0, ��ġ���������Ϲڿ�����_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.15, 0.15));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_yellow_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
			{//���ʿ���
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.37, -0.15, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(0, ��ġ���������Ϲڿ�����_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.15, 0.15));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_yellow_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
		}
		//���׸ӽ�
		{
			{
				{//���׸ӽ� �ٴ�
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.33, 0.6));
					mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(1, 0.22, 0.8));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{//���׸ӽ� ���2��
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.8, -0.19, 0.75));
					mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.8, -0.19, 0.45));
					mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{//���׸ӽ� �����
					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.76, -0.12, 0.6));
					mTransform = glm::rotate(mTransform, glm::radians(-15.0f), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.1, 0.8));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

					mTransform = glm::mat4(1.0f);	//�� ��ȯ
					mTransform = glm::translate(mTransform, glm::vec3(-0.76, -0.12, 0.6));
					mTransform = glm::rotate(mTransform, glm::radians(-15.0f), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(0.2, 0.1, 0.4));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_green_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{

				}
			}
			//���ϵ�
			{
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.312, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŰ���), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.68, -0.318, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŰ���), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.61, -0.324, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŰ���), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.54, -0.33, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŰ���), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.47, -0.336, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŰ���), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.40, -0.342, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŰ���), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
			//�ٴ� �κ�
			{//�ٸ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.65));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŷκ��ٸ�����), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(���׸ӽ�_�ٸ�_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.65));
				mTransform = glm::rotate(mTransform, glm::radians(���׸ӽŷκ��ٸ�����), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.55));
				mTransform = glm::rotate(mTransform, glm::radians(-���׸ӽŷκ��ٸ�����), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(���׸ӽ�_�ٸ�_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.55));
				mTransform = glm::rotate(mTransform, glm::radians(-���׸ӽŷκ��ٸ�����), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//����
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.07, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.15, 0.2, 0.4));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_red_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//�Ӹ�
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, 0.03, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.15, 0.2, 0.4));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_blue_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
		}
	}

	//�κ�
	{
		{	//�Ӹ�
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.1, 0.1));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_h_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
		}
		{	//��
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.1, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_nose_vao);
			glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);
		}
		{	//����
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.25, 0.2, 0.25));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_b_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
		}
		{	//����
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(-walk), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_Larm_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

			//�޴ٸ�
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(0, -0.06, 0));
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(-walk), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_Lleg_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
		}
		{	//������
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(walk), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_Rarm_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

			//�����ٸ�
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			mTransform = glm::translate(mTransform, glm::vec3(0, -0.06, 0));
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(walk), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_Rleg_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
		}
	}



	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Timer_Objmoving(int value);

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
				//����&��ü �����̱� ����
				if (stop_objs == 1) {
					stop_objs = 0;
					glutTimerFunc(0, Timer_Objmoving, 0);
				}
			}
		}
	}
}

void Timer_Move_ahead(int value) {	//�κ� ������ �̵�
	if (stop_animation == 0) {
		if (direction == 0) {//��
			move_z += 0.01;

			if (move_z > 0.82)
				direction = 180;
			if(move_z <= 0 && move_z >= -0.18 && move_x <= 0.18 && move_x >= -0.18)
				direction = 180;
		}
		else if (direction == 180) {	//��
			move_z -= 0.01;

			if (move_z < -0.82)
				direction = 0;
			if (move_z >= 0 && move_z <= 0.18 && move_x <= 0.18 && move_x >= -0.18)
				direction = 0;
		}
		else if (direction == -90) {	//��
			move_x -= 0.01;

			if (move_x < -0.82)
				direction = 90;
			if (move_x >= 0 && move_x <= 0.18 && move_z <= 0.18 && move_z >= -0.18)
				direction = 90;
		}
		else if (direction == 90) {	//��
			move_x += 0.01;

			if (move_x > 0.82)
				direction = -90;
			if (move_x <= 0 && move_x >= -0.18 && move_z <= 0.18 && move_z >= -0.18)
				direction = -90;
		}

		glutPostRedisplay();
		glutTimerFunc(20, Timer_Move_ahead, 1);
	}
}

void Timer_Walk(int value) {	//�κ� �ȴٸ� �ȱ� �ִϸ��̼�
	if (stop_animation == 0) {
		if (value == 1) {
			walk += 1;

			if (walk < 30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk, 1);
			}
			else
				glutTimerFunc(10, Timer_Walk, 0);
		}
		else if (value == 0) {
			walk -= 1;

			if (walk > -30.0f) {
				glutPostRedisplay();
				glutTimerFunc(10, Timer_Walk, 0);
			}
			else
				glutTimerFunc(10, Timer_Walk, 1);
		}
	}
}

void Timer_Jump(int value) {
	if (value == 1) {
		move_y += 0.01;

		if (move_y < -0.07f) {
			glutPostRedisplay();
			glutTimerFunc(30, Timer_Jump, 1);
		}
		else
			glutTimerFunc(30, Timer_Jump, 0);
	}
	else if (value == 0) {
		move_y -= 0.01;

		if (move_y > -0.17f) {
			glutPostRedisplay();
			glutTimerFunc(30, Timer_Jump, 0);
		}
	}
}

void TimerRotate_CAMERA_Y(int value)	//ī�޶� ȭ���� �߽�y ���� ����
{
	if (stop_animation == 0) {

		CP_Y = cp_cw;	//�������� ���� ��ӿ�Ѵ�.

		float temp_x = cameraPos.x, temp_z = cameraPos.z;

		cameraPos.x = temp_x * glm::cos(glm::radians(CP_Y)) + temp_z * glm::sin(glm::radians(CP_Y));	//cw��ŭ ����(ȸ��)���ٴ� ���̹Ƿ�
		cameraPos.z = temp_z * glm::cos(glm::radians(CP_Y)) - temp_x * glm::sin(glm::radians(CP_Y));

		glutPostRedisplay();
	}

}

void TimerRevolution_CAMERA_Y(int value)	//ī�޶�y ���� ����
{
	if (stop_animation == 0) {

		CD_Y += cd_cw;	//�������� ���� ��ӿ�Ѵ�.

		cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y));
		cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + 3.0f;

		glutPostRedisplay();
	}
}

void Timer_Objmoving(int value) {
	if (stop_animation == 0 && stop_objs == 0) {
		ö������ += 2.0f;

		if(tree_leaf_size_up == 1)
			tree_leaf_size += 0.001f;
		else
			tree_leaf_size -= 0.001f;

		if (tree_leaf_size > 0.3)
			tree_leaf_size_up = 0;
		else if (tree_leaf_size < 0.2)
			tree_leaf_size_up = 1;

		if (��ġ�������������� == 1) 
			��ġ���������� += 1;
		else 
			��ġ���������� -= 1;

		if (��ġ���������� <= 0)
			��ġ�������������� = 1;
		else if (��ġ���������� >= 90)
			��ġ�������������� = 0;

		��ġ�����������Ϲڿ�����_x = 0.1 * glm::cos(glm::radians(��ġ����������+90));
		��ġ�����������Ϲڿ�����_y = 0.1 * glm::sin(glm::radians(��ġ����������+90));
		��ġ���������Ϲڿ�����_x = 0.1 * glm::cos(glm::radians(-��ġ���������� + 90));
		��ġ���������Ϲڿ�����_y = 0.1 * glm::sin(glm::radians(-��ġ���������� + 90));

		���׸ӽŰ��� += 1;

		if (���׸ӽŷκ��ٸ��������� == 1)
			���׸ӽŷκ��ٸ����� += 2;
		else
			���׸ӽŷκ��ٸ����� -= 2;

		if (���׸ӽŷκ��ٸ����� <= -36)
			���׸ӽŷκ��ٸ��������� = 1;
		else if (���׸ӽŷκ��ٸ����� >= 36)
			���׸ӽŷκ��ٸ��������� = 0;


		glutPostRedisplay();
		glutTimerFunc(30, Timer_Objmoving, 0);
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

	case 'w'://��
		if (direction == 360 || stop_animation == 1) {
			stop_animation = 0;
			direction = 180.0f;
			glutTimerFunc(0, Timer_Move_ahead, 1);
			glutTimerFunc(0, Timer_Walk, 1);
		}
		else
			direction = 180.0f;
		break;
	case 's'://��
		if (direction == 360 || stop_animation == 1) {
			stop_animation = 0;
			direction = 0.0f;
			glutTimerFunc(0, Timer_Move_ahead, 1);
			glutTimerFunc(0, Timer_Walk, 1);
		}
		else
			direction = 0.0f;
		break;
	case 'a'://��
		if (direction == 360 || stop_animation == 1) {
			stop_animation = 0;
			direction = -90.0f;
			glutTimerFunc(0, Timer_Move_ahead, 1);
			glutTimerFunc(0, Timer_Walk, 1);
		}
		else
			direction = -90.0f;
		break;
	case 'd'://��
		if (direction == 360 || stop_animation == 1) {
			stop_animation = 0;
			direction = 90.0f;
			glutTimerFunc(0, Timer_Move_ahead, 1);
			glutTimerFunc(0, Timer_Walk, 1);
		}
		else
			direction = 90.0f;
		break;
	case 'j':
	case 'J':
		if (move_y == -0.17f) {
			stop_animation = 0;
			glutTimerFunc(0, Timer_Jump, 1);
		}
		break;

	case 'o':
	case 'O':
		stop_animation = 0;
		glutTimerFunc(0, TimerOpen_Screen, 1);
		break;

	case 'z':
		cameraPos.z -= 0.05;
		break;
	case 'Z':
		cameraPos.z += 0.05;
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
	case 'r':
		cp_cw = 1.0f;
		stop_animation = 0;
		glutTimerFunc(0, TimerRotate_CAMERA_Y, 1);
		break;
	case 'R':
		cp_cw = -1.0f;
		stop_animation = 0;
		glutTimerFunc(0, TimerRotate_CAMERA_Y, 1);
		break;

	case 'S':
		if (stop_objs == 0)
			stop_objs = 1;
		else {
			stop_objs = 0;
			glutTimerFunc(0, Timer_Objmoving, 0);
		}
		break;
	case 'i':
	case 'I':	//�ʱ� ����������
		stop_animation = 1;
		stop_objs = 1;
		direction = 360.0f;
		walk = 0;
		frontscreen_y = 0, frontscreen_r = 0;
		move_x = 0; move_y = -0.17f; move_z = 0.5f;
		cameraPos.x = 0.0f; cameraPos.y = 1.0f; cameraPos.z = 2.3f; cameraDirection.x = 0.0f; cameraDirection.z = 0.0f;
		CP_Y = 0.0f; cp_cw = 0; CD_Y = 270.0f; cd_cw = 0;
		tree_leaf_size = 0.2; tree_leaf_size_up = 1;
		ö������ = 0.0; 
		��ġ���������� = 90.0f; ��ġ�������������� = 0;	��ġ�����������Ϲڿ�����_x = -0.1, ��ġ�����������Ϲڿ�����_y = 0, ��ġ���������Ϲڿ�����_x = 0.1, ��ġ���������Ϲڿ�����_y = 0;
		���׸ӽŰ��� = 0.0f; ���׸ӽŷκ��ٸ����� = 0; ���׸ӽŷκ��ٸ��������� = 0;
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
	glutCreateWindow("Example19");

	glewExperimental = GL_TRUE;
	glewInit();

	InitModel();
	InitShader();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	//glutMouseFunc(Mouse);
	glutMainLoop();
}