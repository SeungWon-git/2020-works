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
GLfloat hexa_vn[hexahedronpoints][3] = {};
GLfloat pyramid_vn[pyramidpoints][3] = {};
GLfloat linex_vn[2][3] = { {0.0,0.0,0.0},{0.0,0.0,1.0} };
GLfloat liney_vn[2][3] = { {0.0,0.0,0.0},{1.0,0.0,1.0} };
GLfloat linez_vn[2][3] = { {0.0,0.0,0.0},{1.0,0.0,0.0} };

//�ؽ�ó ��ǥ
GLfloat hexa_vt[hexahedronpoints][2];
GLfloat pyramid_vt[pyramidpoints][2];
GLfloat line_vt[2][2] = { {0.0,0.0},{1.0,1.0} };

//vao,vbo
GLuint ����_vao, hexahedron_vbo[3];
GLuint pyramid_vao, pyramid_vbo[3];
GLuint linex_vao, linex_vbo[3], liney_vao, liney_vbo[3], linez_vao, linez_vbo[3];

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

GLclampf r_x = 0.0f;	//���� ������ȯ
GLfloat RX = 0.0f;		//���� ����
GLclampf r_y = 0.0f;	//���� ������ȯ
GLfloat RY = 0.0f;		//���� ����

// �̹���
unsigned int texture_a, texture_b, texture_c, texture_d, texture_e, texture_f, texture_line;




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

void InitModel() {
	// obj�б�
	char filename[30] = "������ü.obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, hexahedron_s, hexa_vn, hexa_vt);

	strcpy(filename, "�簢��(�Ƕ�̵�).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"������ ���� ���Ͽ����ϴ�." << std::endl;
		exit(0);	// ���н� ����
	}
	else
		ReadObj(file, pyramid_s, pyramid_vn, pyramid_vt);

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

void InitBuffer()
{
	InitVao(����_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexa_vn,hexa_vt);

	InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

	InitVao(linex_vao, linex_vbo, 2, linex_s, linex_vn, line_vt);
	InitVao(liney_vao, liney_vbo, 2, liney_s, liney_vn, line_vt);
	InitVao(linez_vao, linez_vbo, 2, linez_s, linez_vn, line_vt);
}

void InitTexture() {
	BITMAPINFO* bmp;
	int widthImage, heightImage, numberOfChannel;

	//A
	glGenTextures(1, &texture_a); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_a); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	unsigned char* data = stbi_load("a.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)
	
	//B
	glGenTextures(1, &texture_b); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_b); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("b.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//C
	glGenTextures(1, &texture_c); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_c); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("c.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//D
	glGenTextures(1, &texture_d); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_d); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("d.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//E
	glGenTextures(1, &texture_e); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_e); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("e.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//F
	glGenTextures(1, &texture_f); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_f); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("f.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---�ؽ�ó �̹��� ���� 
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 24�̸� '3'���� �ϰ� 7��° ���ڿ� 'GL_RGB'�� �ش�.)
	//(3��° ���ڰ� - �̹��� ������ ��Ʈ ������ 32�̸� '4'���� �ϰ� 7��° ���ڿ� 'GL_RGBA'�� �ش�.)

	//Line
	glGenTextures(1, &texture_line); //--- �ؽ�ó ����
	glBindTexture(GL_TEXTURE_2D, texture_line); //--- �ؽ�ó ���ε�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- ���� ���ε��� �ؽ�ó�� �Ķ���� �����ϱ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- �̹����� �Ųٷ� �����ٸ� �߰�
	data = stbi_load("black_line.png", &widthImage, &heightImage, &numberOfChannel, 0);
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
	mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_line);

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture_c);
		glDrawArrays(GL_TRIANGLES, 0, 6);	//0��~6������ -��

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_d);
		glDrawArrays(GL_TRIANGLES, 6, 6);	//6��~11������ -����

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_b);
		glDrawArrays(GL_TRIANGLES, 18, 6);	//18��~23������ -������

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_e);
		glDrawArrays(GL_TRIANGLES, 24, 6);	//24��~29������ -�Ʒ�

		{//���� ȸ�� �ִϸ��̼� (�߿�- ȸ���� ������ x,y,z�� �߽����ιۿ� ���ϹǷ� ���ð��� ����(���ϴ� �࿡ ����)���༭ ȸ����Ų�� �ٽ� ����ġ�� ������Ѵ�.)
			for (int i = 12; i < 18; ++i)	//�ش� �ε����� ���ð� ����(y���� 0����-> x�࿡ ȸ���Ұ��̱� ������)
				hexahedron_s[i][1] -= 0.25;

			InitVao(����_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexa_vn, hexa_vt);	//vao ������Ʈ

			mTransform = glm::translate(mTransform, glm::vec3(0.0, 0.25, 0.0));	//����ü ���� �̵�(������ ���� �̵��ϰԵ� - ���ڸ��� ��ġ�ϴ� ��Ȱ)
			mTransform = glm::rotate(mTransform, glm::radians(RUP), glm::vec3(1.0, 0.0, 0.0));	//���� x�࿡���� ȸ��
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_f);
			glDrawArrays(GL_TRIANGLES, 12, 6);	//12��~17������ -��

			for (int i = 12; i < 18; ++i)	//�ش� �ε����� ���ð� ���󺹱� - Ȥ�� �Ŀ� �� ��ǥ�� �׳� ������ �ֱ� ������
				hexahedron_s[i][1] += 0.25;
		}

		{//���� ȸ�� �ִϸ��̼�
			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] += 0.25; hexahedron_s[i][2] -= 0.25;
			}

			InitVao(����_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexa_vn, hexa_vt);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0.25));
			mTransform = glm::rotate(mTransform, glm::radians(RSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_a);
			glDrawArrays(GL_TRIANGLES, 30, 6);	//30��~35������ -��

			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] -= 0.25; hexahedron_s[i][2] += 0.25;
			}
		}
	}
	else {	//�簢��(�Ƕ�̵�)
		glBindVertexArray(pyramid_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_e);
		glDrawArrays(GL_TRIANGLES, 9, 6);	//9��~14������

		{//���� ȸ�� �ִϸ��̼�
			for (int i = 0; i < 3; ++i) {
				pyramid_s[i][1] += 0.15; pyramid_s[i][2] += 0.25;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.15, -0.25));
			mTransform = glm::rotate(mTransform, glm::radians(-PSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_c);
			glDrawArrays(GL_TRIANGLES, 0, 3);	//0��~2������ -��

			for (int i = 0; i < 3; ++i) {
				pyramid_s[i][1] -= 0.15; pyramid_s[i][2] -= 0.25;
			}
		}
		{//���� ȸ�� �ִϸ��̼�
			for (int i = 3; i < 6; ++i) {
				pyramid_s[i][0] += 0.25; pyramid_s[i][1] += 0.15;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(-0.25, -0.15, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(PSIDE), glm::vec3(0.0, 0.0, 1.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_d);
			glDrawArrays(GL_TRIANGLES, 3, 3);	//3��~5������ -��

			for (int i = 3; i < 6; ++i) {
				pyramid_s[i][0] -= 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//���� ȸ�� �ִϸ��̼�
			for (int i = 6; i < 9; ++i) {
				pyramid_s[i][0] -= 0.25; pyramid_s[i][1] += 0.15;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.25, -0.15, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(-PSIDE), glm::vec3(0.0, 0.0, 1.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_b);
			glDrawArrays(GL_TRIANGLES, 6, 3);	//6��~8������ -����

			for (int i = 6; i < 9; ++i) {
				pyramid_s[i][0] += 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//���� ȸ�� �ִϸ��̼�
			for (int i = 15; i < 18; ++i) {
				pyramid_s[i][1] += 0.15; pyramid_s[i][2] -= 0.25;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.15, 0.25));
			mTransform = glm::rotate(mTransform, glm::radians(PSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_a);
			glDrawArrays(GL_TRIANGLES, 15, 3);	//15��~17������ -��

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

void Timer_Rotate_Model_X(int value)
{
	if (stop_animation == 0 && r_x != 0) {

		RX += r_x;

		if (RX == 360.0f)
			RX = 0.0f;
		if (RX == 0.0f)
			RX = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(30, Timer_Rotate_Model_X, 1);
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

	case 'x':
	case 'X':
		stop_animation = 0;
		if (r_x != -1.0f) {
			r_x = -1.0f;
			glutTimerFunc(0, Timer_Rotate_Model_X, 1);
		}
		else
			r_x = 0.0f;
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
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
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
	glutCreateWindow("Example23");

	glewExperimental = GL_TRUE;
	glewInit();

	InitModel();
	InitShader();
	InitBuffer();
	InitTexture();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}