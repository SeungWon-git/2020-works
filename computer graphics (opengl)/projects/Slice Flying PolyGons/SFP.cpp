//made by Jeung Seung Won -20.11.15



#pragma warning(disable: 4996)
#include <random>
#include <iostream>

//��������
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

//�Ҹ����
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace std;


std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid_s{ 0.05,0.25 };	//���
std::uniform_real_distribution<> uid_c{ 0.0,1.0 };		//��
std::uniform_real_distribution<> uid_sy{ -0.25,0.4 };	//������ y��ġ
std::uniform_real_distribution<> uid_dx{ 0.05,0.065 };	//������ �ӵ�
std::uniform_real_distribution<> uid_dy{ 0.035,0.05 };
std::uniform_real_distribution<> uid_dfx{ -0.02,0.02 };	//��������
std::uniform_real_distribution<> uid_dfy{ 0.01,0.02 };

#define Block_num	500

typedef struct Block {
	GLuint vao, vbo[2];		//vao,vbo
	GLfloat s[6][3];		//�ּ�(���)
	GLfloat c[6][3];		//����
	size_t	status{ 0 };			//����(0-���, 1-�����̽� ���� ����, 2-ĭ�� ��, 5- ���������� �� �̻� �߸��� ����, 6- �������� �ﰢ���� ���� �ٿ�� �ڽ�üũ�� �� �ϱ� ����), 7- ���ӿ��� ����(������ ������ ��ȭ ����)
	GLfloat m_x{ 0 }, m_y{ 0 };		//ȭ��� ��ġ
	GLfloat m_dx, m_dy;		//�����̴� �ӵ�
	GLfloat l,r,b,u;		//�ٿ���ڽ��� ���� �����¿찪
	GLfloat size_x, size_y;	//�� �� ������ ������ ���� ��
	int hexa_tri_index{ Block_num };		//�������� �ﰢ���� �簢���� ���󰡱� ���� �ﰢ���� �ε��� ���� ����

	void get_bb(){
		GLfloat max_l = 1, max_r = -1, max_b = 1, max_u = -1;
		GLfloat bl, br, bb, bu;

		for (int i = 0; i < 6; ++i) {
			max_l = max_l < s[i][0] ? max_l : s[i][0];
			max_r = max_r > s[i][0] ? max_r : s[i][0];
			max_b = max_b < s[i][1] ? max_b : s[i][1];
			max_u = max_u > s[i][1] ? max_u : s[i][1];
		}

		if (max_l > -0.05)
			bl = -0.005;
		else bl = max_l / 2;
		if (max_r < 0.05)
			br = 0.005;
		else br = max_r / 2;
		if (max_b > -0.05)
			bb = -0.005;
		else bb = max_b / 2;
		if (max_u < 0.05)
			bu = 0.005;
		else bu = max_u / 2;	//�ٿ�� �ڽ� �ڸ� �� ��� ���� ����

		l = (max_l + max_r) / 2 + bl; r = (max_l + max_r) / 2 + br; b = (max_b + max_u) / 2 + bb; u = (max_b + max_u) / 2 + bu;	
	}

}Block;

//���
//#define Block_num	500
Block block[Block_num];

//�ٿ�� �ڽ�
GLuint bb_vao[Block_num], bb_vbo[Block_num][2];

//���콺 �����̽�
GLfloat mouse_s[2][3];
GLfloat mouse_c[2][3];
GLuint mouse_vao, mouse_vbo[2];

// 3x8 ��
typedef struct Cell{
	GLfloat s[4][3];	//��ǥ
	GLuint cell_vao, cell_vbo[2];
	bool fill_cell{ 0 };	//���� ��ġ(���� �Ʒ� �𼭸�����) & ä������ �� ����
	int index;	//����ִ� ������ �ε��� �ѹ�
	GLfloat gg_x, gg_y;	//���ӿ����� �����̱� ����
	GLfloat gg_dx, gg_dy;
	
}Cell;

Cell cell[24];

//��� 
#define line_l	250
GLfloat trace_s[line_l][3];
GLfloat trace_c[line_l][3];
GLuint trace_vao[Block_num], trace_vbo[Block_num][2];

//���� ����
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 800.0f;

GLenum mode = GL_TRIANGLES;	//Line/ Triangles

bool click = 0;	//���콺 Ŭ��

bool pause = 0;
int speed = 50;
bool trace{0};	//��� on/off
bool show_bb = 0;	//�ٿ���ڽ� ����

int fill_num{ 0 };	//���� ü���� ��
bool game_over{ 0 };	
GLfloat gg_R{ 0 };

bool cell_click = 0;	//�� ĭ Ŭ��
int cell_click_index = 0;	//Ŭ���� ���� �ε���
int cell_click_follow = 0;	//�������� �ﰢ�� �簢�� ���󰡱�
GLfloat move_x = 0.0f, move_y = 0.0f, move_z = 0.0f;	//�� �̵�




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

void InitModel() {
	// [�� ��ġ ���� & InitVao]
	// cell_vao[0]�� ���� ���� ��Ҹ����� ����
	GLfloat s[4][3];
	GLfloat cell_c[4][3] = {
		{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} // �ϴû�
	};

	s[0][0] = -1.0f; s[0][1] = -1.0f; s[0][2] = 0.0f;
	s[1][0] = -0.75f; s[1][1] = -1.0f; s[1][2] = 0.0f;
	s[2][0] = -0.75f; s[2][1] = -0.8f; s[2][2] = 0.0f;
	s[3][0] = -1.0f; s[3][1] = -0.8f; s[3][2] = 0.0f;

	for (int j = 0; j < 3; ++j) {
		s[0][0] = -1.0f; s[1][0] = -0.75f; s[2][0] = -0.75f; s[3][0] = -1.0f;
		for (int i = 0; i < 8; ++i) {

			cell[i + j * 8].s[0][0] = s[0][0]; cell[i + j * 8].s[0][1] = s[0][1]; cell[i + j * 8].s[0][2] = s[0][2];
			cell[i + j * 8].s[1][0] = s[1][0]; cell[i + j * 8].s[1][1] = s[1][1]; cell[i + j * 8].s[1][2] = s[1][2];
			cell[i + j * 8].s[2][0] = s[2][0]; cell[i + j * 8].s[2][1] = s[2][1]; cell[i + j * 8].s[2][2] = s[2][2];
			cell[i + j * 8].s[3][0] = s[3][0]; cell[i + j * 8].s[3][1] = s[3][1]; cell[i + j * 8].s[3][2] = s[3][2];

			InitVao(cell[i + j * 8].cell_vao, cell[i + j * 8].cell_vbo, 4, cell[i + j * 8].s, cell_c);

			s[0][0] += 0.25f; s[1][0] += 0.25f; s[2][0] += 0.25f; s[3][0] += 0.25f;  	
		}
		s[0][1] += 0.2f; s[1][1] += 0.2f; s[2][1] += 0.2f; s[3][1] += 0.2f;
	}

	//��� ����
	for (int i = 0; i < line_l; ++i) {
		trace_c[i][0] = 0; trace_c[i][1] = 1; trace_c[i][2] = 0;	//�ʷϻ�����
	}
}

void CreateBlock() {
	for (int j = 0; j < Block_num; ++j) {
		if (block[j].status == 0) {

			// [�� �ʱ�ȭ]
			GLfloat r = uid_c(dre), g = uid_c(dre), b = uid_c(dre);
			for (int i = 0; i < 6; ++i) {
				block[j].c[i][0] = r; block[j].c[i][1] = g; block[j].c[i][2] = b;
			}

			// [�� ��ġ ����]
			if (uid_s(dre) <= 0.1)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] < 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] < 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1�� ���� �׻� 3��и鿡 �ֵ���
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] > 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] < 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2�� ���� �׻� 4��и鿡 �ֵ���
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] > 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] > 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3�� ���� �׻� 1��и鿡 �ֵ���
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//�簢���� �ȴ�.0.5
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] < 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] > 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6�� ���� �׻� 2��и鿡 �ֵ���
				}
				else {	//�ﰢ���� �ȴ�.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}
			else if (uid_s(dre) <= 0.15)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] > 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] < 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1�� ���� �׻� 4��и鿡 �ֵ���
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] > 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] > 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2�� ���� �׻� 1��и鿡 �ֵ���
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] < 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] > 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3�� ���� �׻� 2��и鿡 �ֵ���
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//�簢���� �ȴ�.
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] < 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] < 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6�� ���� �׻� 3��и鿡 �ֵ���
				}
				else {	//�ﰢ���� �ȴ�.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}
			else if (uid_s(dre) <= 0.2)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] > 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] > 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1�� ���� �׻� 1��и鿡 �ֵ���
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] < 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] > 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2�� ���� �׻� 2��и鿡 �ֵ���
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] < 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] < 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3�� ���� �׻� 3��и鿡 �ֵ���
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//�簢���� �ȴ�.
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] > 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] < 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6�� ���� �׻� 4��и鿡 �ֵ���
				}
				else {	//�ﰢ���� �ȴ�.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}
			else if (uid_s(dre) <= 0.25)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] < 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] > 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1�� ���� �׻� 2��и鿡 �ֵ���
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] < 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] < 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2�� ���� �׻� 3��и鿡 �ֵ���
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] > 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] < 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3�� ���� �׻� 4��и鿡 �ֵ���
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//�簢���� �ȴ�.
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] > 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] > 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6�� ���� �׻� 1��и鿡 �ֵ���
				}
				else {	//�ﰢ���� �ȴ�.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}

			block[j].get_bb();

			// [ȭ����ǥ�� ��Ÿ�� ��ġ]
			if (uid_c(dre) < 0.5) {	//������
				block[j].m_x = -1.1; block[j].m_y = uid_sy(dre);
				// [�ӵ� ����]
				block[j].m_dx = uid_dx(dre); block[j].m_dy = uid_dy(dre);
			}
			else {	//������
				block[j].m_x = 1.1; block[j].m_y = uid_sy(dre);
				// [�ӵ� ����]
				block[j].m_dx = uid_dx(dre) * (-1); block[j].m_dy = uid_dy(dre);
			}

			// [�ٿ���� �ڽ� �ּ� & ����]
			GLfloat bb_s[4][3] = {
				{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
			};
			GLfloat bb_c[4][3] = {
				{1,1,1},{0,1,0},{1,0,0},{0,0,1}	
			};

			// [InitVao]
			InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
			InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�

			// status ����
			block[j].status = 1;

			//��� vao
			trace_s[0][0] = block[j].m_x; trace_s[0][1] = block[j].m_y; trace_s[0][2] = 0;

			GLfloat tdx = block[j].m_dx;
			GLfloat tdy = block[j].m_dy;

			for (int i = 1; i < line_l; ++i) {
				trace_s[i][0] = trace_s[i - 1][0] + tdx / 5;
				trace_s[i][1] = trace_s[i - 1][1] + tdy / 5;
				tdy -= 0.0004f;//�߷�

				trace_s[i][2] = 0;
			}

			InitVao(trace_vao[j], trace_vbo[j], line_l, trace_s, trace_c);

			break;
		}
	}
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

// �� ���� ���� Ȯ�� �Լ�
bool GetIntersectPoint(const GLfloat& AP1_x, const GLfloat& AP1_y, const GLfloat& AP2_x, const GLfloat& AP2_y, const GLfloat& BP1_x, const GLfloat& BP1_y, const GLfloat& BP2_x, const GLfloat& BP2_y, GLfloat IP[3])	
{
	GLfloat t;
	GLfloat s;
	GLfloat under = (BP2_y - BP1_y) * (AP2_x - AP1_x) - (BP2_x - BP1_x) * (AP2_y - AP1_y);
	if (under == 0) return false;

	GLfloat _t = (BP2_x - BP1_x) * (AP1_y - BP1_y) - (BP2_y - BP1_y) * (AP1_x - BP1_x);
	GLfloat _s = (AP2_x - AP1_x) * (AP1_y - BP1_y) - (AP2_y - AP1_y) * (AP1_x - BP1_x);

	t = _t / under;
	s = _s / under;

	if (t < 0.0 || t>1.0 || s < 0.0 || s>1.0) return false;
	if (_t == 0 && _s == 0) return false;

	IP[0] = AP1_x + t * (AP2_x - AP1_x);
	IP[1] = AP1_y + t * (AP2_y - AP1_y);
	IP[2] = 0;

	return true;
}





GLvoid drawScene()
{
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);	// ���� ����


	unsigned int modelLoc = glGetUniformLocation(s_program, "model");	//--- ���ؽ� ���̴����� �𵨸� ��ȯ ��� �������� �޾ƿ´�.
	glm::mat4 mTransform = glm::mat4(1.0f);		  // ������ķ� �ʱ�ȭ

	//���ִ�(����) ��
	for (int i = 0; i < 24; ++i) {
		if (cell[i].fill_cell == 1) {

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			if (game_over == 1)
				mTransform = glm::translate(mTransform, glm::vec3(cell[i].gg_x, cell[i].gg_y, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(cell[i].cell_vao);
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	}

	//��(Ǫ��) ��
	for (int i = 0; i < 24; ++i) {
		if (cell[i].fill_cell == 0) {

			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			if (game_over == 1)
				mTransform = glm::translate(mTransform, glm::vec3(cell[i].gg_x, cell[i].gg_y, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(cell[i].cell_vao);
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	}

	//���콺 �����̽�
	glBindVertexArray(mouse_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	//�ٿ�� �ڽ� ǥ��
	if (show_bb == 1) {
		for (int i = 0; i < Block_num; ++i) {
			if (block[i].status != 0 && block[i].status != 6) {
				mTransform = glm::mat4(1.0f);	//�� ��ȯ
				mTransform = glm::translate(mTransform, glm::vec3(block[i].m_x, block[i].m_y, 0.0));
				//mTransform = glm::rotate(mTransform, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
				//mTransform = glm::scale(mTransform, glm::vec3(1.0, 1.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(bb_vao[i]);
				glDrawArrays(GL_LINE_LOOP, 0, 4);
			}
		}
	}

	//���
	for (int i = 0; i < Block_num; ++i) {
		if (block[i].status != 0) {
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			if (cell_click == 1 && i == cell[cell_click_index].index || cell_click == 1 && i == cell_click_follow && block[i].hexa_tri_index == cell_click_follow)
				mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, 0.0));
			else
				mTransform = glm::translate(mTransform, glm::vec3(block[i].m_x, block[i].m_y, 0.0));
			if (game_over == 1)
				mTransform = glm::rotate(mTransform, glm::radians(gg_R), glm::vec3(0.0, 0.0, 1.0));
			if(block[i].status == 2 || block[i].status == 7)
				mTransform = glm::scale(mTransform, glm::vec3(block[i].size_x, block[i].size_y, 1.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
		
			glBindVertexArray(block[i].vao);
			glDrawArrays(mode, 0, 6);
		}
		//���
		if (trace == 1 && block[i].status == 1 && game_over == 0) {
			mTransform = glm::mat4(1.0f);	//�� ��ȯ
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(trace_vao[i]);
			glDrawArrays(GL_POINTS, 0, line_l);
		}

	}

	
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Timer_Game_Over(int value) {
	for (int i = 0; i < Block_num; ++i) {
		if (block[i].status != 0) {
			block[i].m_x += block[i].m_dx; block[i].m_y += block[i].m_dy;
			block[i].m_dy -= 0.002;	//�߷�
		}
	}

	for (int i = 0; i < 24; ++i) {
		cell[i].gg_x += cell[i].gg_dx;
		cell[i].gg_y += cell[i].gg_dy;
		cell[i].gg_dy -= 0.002;//�߷�
	}

	++value;

	gg_R += 5;

	cout << (80 - value) + 1 << "ms left"<< endl;
	
	if (value == 80)	//4�� �� ����
		exit(0);

	glutPostRedisplay();
	glutTimerFunc(50, Timer_Game_Over, value);
}

void Timer_Block(int value) {
	if (pause == 0 && game_over != 1) {
		if (value == 1) {
			++value;
			PlaySound(L"�������.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);	//��� �뷡

			glutPostRedisplay();
			glutTimerFunc(8000, Timer_Block, value);
		}
		else {
			++value;
			if (value == MAXINT)
				value = 2;

			//���ο� ģ���� ����
			if (value % 25 == 0)
				CreateBlock();

			//�ߺ�üũ�ؼ� ũ��0,0�� ģ�� ���� && ũ�� �ʹ� ���� ģ�� ����
			for (int i = 0; i < Block_num; ++i) {
				if (block[i].status != 2) {	//���� ���� �ƴ϶��
					if (abs(block[i].s[0][0] - block[i].s[1][0]) <= 0.005 && abs(block[i].s[0][0] - block[i].s[2][0]) <= 0.005 && abs(block[i].s[1][0] - block[i].s[2][0]) <= 0.005)
						block[i].status = 0;
					else if (abs(block[i].s[1][0] - block[i].s[2][0]) <= 0.005 && abs(block[i].s[1][1] - block[i].s[2][1]) <= 0.005)
						block[i].status = 0;
					
				}
			}

			// ������
			for (int i = 0; i < Block_num; ++i) {
				if (block[i].status == 1 || block[i].status == 5 || block[i].status == 6) {	//���ư��� �� && ������
					block[i].m_x += block[i].m_dx; block[i].m_y += block[i].m_dy;
					block[i].m_dy -= 0.002;	//�߷�

					if (block[i].m_x >= 1.2 || block[i].m_x <= -1.2 || block[i].m_y <= -1.5)	//ȭ�鿡�� ����
						block[i].status = 0;
				}
			}

			//�� �浹 �˻�
			for (int i = 0; i < Block_num; ++i) {
				if ((block[i].status == 1 || block[i].status == 5) && (block[i].b + block[i].m_y) <= -0.4f) {
					for (int j = 0; j < 24; ++j) {
						//���� �浹��
						if (cell[j].fill_cell == 0 && abs(block[i].s[0][0]- block[i].s[1][0]) >= 0.005 && abs(block[i].s[0][0] - block[i].s[2][0]) >= 0.005 && abs(block[i].s[1][0] - block[i].s[2][0]) >= 0.005
							&& block[i].b + block[i].m_y >= cell[j].s[0][1] && block[i].b + block[i].m_y <= cell[j].s[0][1] +0.1
							&& (block[i].l + block[i].m_x + block[i].r + block[i].m_x) / 2.0f >= cell[j].s[0][0] && (block[i].l + block[i].m_x + block[i].r + block[i].m_x) / 2.0f <= cell[j].s[1][0])
						{
							++fill_num; --speed;
							if (speed <= 6)
								speed = 5;

							cout << 24 - fill_num << "- left" << endl;

							//�������̿��� ��� �ﰢ�� ���� ��ȯ
							if (block[i].status == 5) {
								int n = block[i].hexa_tri_index;
								
								//��ġ ����
								float tmp1 = block[i].l + block[i].r;
								if (tmp1 < 0)
									block[n].m_x = cell[j].s[0][0] + 0.125 + abs(tmp1) / 2.0f;
								else
									block[n].m_x = cell[j].s[0][0] + 0.125 - abs(tmp1) / 2.0f;

								//������ ����
								GLfloat w = abs(block[i].r - block[i].l);
								GLfloat h = abs(block[i].u - block[i].b);
								if (w >= 0.18)
									block[n].size_x = 0.25f / w * 0.3;
								else if (w <= 0.07)
									block[n].size_x = 0.25f / w * 0.5;
								else block[n].size_x = 1.0f;
								if (h >= 0.15)
									block[n].size_y = 0.2f / h * 0.3;
								else if (h <= 0.05)
									block[n].size_y = 0.2f / h * 0.6;
								else block[n].size_y = 1.0f;

								block[n].status = 2;
							}

							//��ġ ����
							float tmp1 = block[i].l + block[i].r;
							if (tmp1 < 0)
								block[i].m_x = cell[j].s[0][0] + 0.125 + abs(tmp1) / 2.0f;
							else
								block[i].m_x = cell[j].s[0][0] + 0.125 - abs(tmp1) / 2.0f;

							//������ ����
							GLfloat w = abs(block[i].r - block[i].l);
							GLfloat h = abs(block[i].u - block[i].b);
							if (w >= 0.18)
								block[i].size_x = 0.25f / w * 0.3;
							else if (w <= 0.07)
								block[i].size_x = 0.25f / w * 0.5;
							else block[i].size_x = 1.0f;
							if (h >= 0.15)
								block[i].size_y = 0.2f / h * 0.3;
							else if (h <= 0.05)
								block[i].size_y = 0.2f / h * 0.6;
							else block[i].size_y = 1.0f;
							
							GLfloat cell_c[4][3] = {
							{0.31,0.73,0.87},{0.31,0.73,0.87} ,{1,0,0},{1,0,0} // ������ ������
							};
							InitVao(cell[j].cell_vao, cell[j].cell_vbo, 4, cell[j].s, cell_c);

							cell[j].index = i;
							cell[j].fill_cell = 1;
							block[i].status = 2;
							
							break;
						}
					}
				}
			}

			//����
			if (fill_num == 24) {
				game_over = 1;

				for (int i = 0; i < Block_num; ++i) {
					if (block[i].status == 2) {
						block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre) + 0.04;
						for (int j = 0; j < 24; ++j) {
							if (cell[j].index == i) {
								cell[j].gg_dx = block[i].m_dx;
								cell[j].gg_dy = block[i].m_dy;

								break;
							}
						}
						block[i].status = 7;
					}
				}

				glutTimerFunc(0, Timer_Game_Over, 0);
			}

			glutPostRedisplay();
			glutTimerFunc(speed, Timer_Block, value);
		}
	}
}

void Mouse(int b, int s, int x, int y)
{
	if (/*pause == 0 &&*/ b == GLUT_LEFT_BUTTON && s == GLUT_DOWN && y <= 560) {	// �����̽� ����
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
		Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

		mouse_s[0][0] = X; mouse_s[0][1] = Y; mouse_s[0][2] = 0;
		mouse_s[1][0] = X; mouse_s[1][1] = Y; mouse_s[1][2] = 0;

		mouse_c[0][0] = 1; mouse_c[0][1] = 1; mouse_c[0][2] = 1;	//���
		mouse_c[1][0] = 1; mouse_c[1][1] = 1; mouse_c[1][2] = 1;
		InitVao(mouse_vao, mouse_vbo, 2, mouse_s, mouse_c);
		click = 1;
	}
	if (click == 1 && b == GLUT_LEFT_BUTTON && s == GLUT_UP && y <= 560) {	// �����̽� ��
		click = 0;
		int t{ 0 }; //������ ���鶧 �簢���� �ε��� ������ �ʿ�

		for (int i = 0; i < Block_num; ++i) {	// �����̽� Ȯ��		
			if (block[i].status == 1) {

				GLfloat o1[2] = { block[i].s[0][0] + block[i].m_x,block[i].s[0][1] + block[i].m_y };
				GLfloat o2[2] = { block[i].s[1][0] + block[i].m_x,block[i].s[1][1] + block[i].m_y };
				GLfloat o3[2] = { block[i].s[2][0] + block[i].m_x,block[i].s[2][1] + block[i].m_y };
				GLfloat o6[2] = { block[i].s[5][0] + block[i].m_x,block[i].s[5][1] + block[i].m_y };

				GLfloat IP[3];
				GLfloat SliceP1[3], SliceP2[3];
				int num = 0;
				bool line1to2{ false }, line1to3{ false }, line1to6{ false }, line2to3{ false }, line3to6{ false };

				//�ﰢ�� �浹�˻�
				if (block[i].s[5][0] == block[i].s[0][0])
				{
					if (GetIntersectPoint(mouse_s[0][0], mouse_s[0][1], mouse_s[1][0], mouse_s[1][1], o1[0], o1[1], o2[0], o2[1], IP)) {
						num++; line1to2 = 1;
						SliceP1[0] = IP[0]; SliceP1[1] = IP[1]; SliceP1[2] = 0;
						/*cout << "1-2" << endl;*/
					}
					if (GetIntersectPoint(mouse_s[0][0], mouse_s[0][1], mouse_s[1][0], mouse_s[1][1], o1[0], o1[1], o3[0], o3[1], IP)) {
						num++; line1to3 = 1;
						if (num == 1) {
							SliceP1[0] = IP[0]; SliceP1[1] = IP[1]; SliceP1[2] = 0;
						}
						else if (num == 2) {
							SliceP2[0] = IP[0]; SliceP2[1] = IP[1]; SliceP2[2] = 0;
						}
						/*cout << "1-3" << endl;*/
					}
					if (GetIntersectPoint(mouse_s[0][0], mouse_s[0][1], mouse_s[1][0], mouse_s[1][1], o2[0], o2[1], o3[0], o3[1], IP)) {
						num++; line2to3 = 1;
						if (num == 1) {
							SliceP1[0] = IP[0]; SliceP1[1] = IP[1]; SliceP1[2] = 0;
						}
						else if (num == 2) {
							SliceP2[0] = IP[0]; SliceP2[1] = IP[1]; SliceP2[2] = 0;
						}
						/*cout << "2-3" << endl;*/
					}
				}
				//�簢�� �浹�˻�
				else {
					if (GetIntersectPoint(mouse_s[0][0], mouse_s[0][1], mouse_s[1][0], mouse_s[1][1], o1[0], o1[1], o2[0], o2[1], IP)) {
						num++; line1to2 = 1;
						SliceP1[0] = IP[0]; SliceP1[1] = IP[1]; SliceP1[2] = 0;
						/*cout << "1-2" << endl;*/
					}
					if (GetIntersectPoint(mouse_s[0][0], mouse_s[0][1], mouse_s[1][0], mouse_s[1][1], o1[0], o1[1], o6[0], o6[1], IP)) {
						num++; line1to6 = 1;
						if (num == 1) {
							SliceP1[0] = IP[0]; SliceP1[1] = IP[1]; SliceP1[2] = 0;
						}
						else if (num == 2) {
							SliceP2[0] = IP[0]; SliceP2[1] = IP[1]; SliceP2[2] = 0;
						}
						/*cout << "1-6" << endl;*/
					}
					if (GetIntersectPoint(mouse_s[0][0], mouse_s[0][1], mouse_s[1][0], mouse_s[1][1], o2[0], o2[1], o3[0], o3[1], IP)) {
						num++; line2to3 = 1;
						if (num == 1) {
							SliceP1[0] = IP[0]; SliceP1[1] = IP[1]; SliceP1[2] = 0;
						}
						else if (num == 2) {
							SliceP2[0] = IP[0]; SliceP2[1] = IP[1]; SliceP2[2] = 0;
						}
						/*cout << "2-3" << endl;*/
					}
					if (GetIntersectPoint(mouse_s[0][0], mouse_s[0][1], mouse_s[1][0], mouse_s[1][1], o3[0], o3[1], o6[0], o6[1], IP)) {
						num++; line3to6 = 1;
						if (num == 1) {
							SliceP1[0] = IP[0]; SliceP1[1] = IP[1]; SliceP1[2] = 0;
						}
						else if (num == 2) {
							SliceP2[0] = IP[0]; SliceP2[1] = IP[1]; SliceP2[2] = 0;
						}
						/*cout << "3-6" << endl;*/
					}
				}

				if (num == 2) {	// ������
					if (block[i].s[5][0] == block[i].s[0][0])	//�ﰢ��
					{
						if (line1to2 == 1 && line1to3 == 1)	//1-2,1-3�� �ڸ���
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//�ڽ� �簢�� �����
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}
							//�ﰢ�� ������ (�θ� �ﰢ��->�ڽ�)
							block[i].s[1][0] = SliceP1[0] - block[i].m_x; block[i].s[1][1] = SliceP1[1] - block[i].m_y;
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
						else if (line1to3 == 1 && line2to3 == 1)	//1-3, 2-3�� �ڸ���
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//�ڽ� �簢�� �����
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[0][0]; block[j].s[1][1] = block[i].s[0][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[1][0]; block[j].s[2][1] = block[i].s[1][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}
							//�ﰢ�� ������ (�θ� �ﰢ��->�ڽ�)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[1][0] = SliceP2[0] - block[i].m_x; block[i].s[1][1] = SliceP2[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
						else if (line1to2 == 1 && line2to3 == 1)	//1-2,2-3�� �ڸ���
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//�ڽ� �簢�� �����
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[0][0]; block[j].s[1][1] = block[i].s[0][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}
							//�ﰢ�� ������ (�θ� �ﰢ��->�ڽ�)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
					}
					else			//�簢��
					{
						//�簢�� �ΰ��� �߸���
						if (line1to2 == 1 && line3to6 == 1)	//1-2,3-6�� �ڸ���
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//�ڽ� �簢�� �����
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP2[0] - block[i].m_x; block[j].s[0][1] = SliceP2[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[5][0]; block[j].s[1][1] = block[i].s[5][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[0][0]; block[j].s[2][1] = block[i].s[0][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP1[0] - block[i].m_x; block[j].s[5][1] = SliceP1[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}
							//�簢�� ������ (�θ� �簢��->�ڽ�)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[5][0] = SliceP2[0] - block[i].m_x; block[i].s[5][1] = SliceP2[1] - block[i].m_y;

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
						else if (line1to6 == 1 && line2to3 == 1)	//1-6,2-3�� �ڸ���
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//�ڽ� �簢�� �����
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[0][0]; block[j].s[1][1] = block[i].s[0][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[1][0]; block[j].s[2][1] = block[i].s[1][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}
							//�簢�� ������ (�θ� �簢��->�ڽ�)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[1][0] = SliceP2[0] - block[i].m_x; block[i].s[1][1] = SliceP2[1] - block[i].m_y;

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
						//�簢�� �� �ﰢ������ �߸���
						else if (line1to6 == 1 && line3to6 == 1)	//1-6,3-6�� �ڸ���
						{	//�ڽ� ������ �����
							for (int j = Block_num - 1; j >= 0; --j) {
								//�簢��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//�������̹Ƿ� �ٿ�� �ڽ� ���Ƿ� �ø�
									t = j;

									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//�ﰢ��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP1[0] - block[i].m_x; block[j].s[2][1] = SliceP1[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//�ӵ� �簢���� ����

									//�ٿ�� �ڽ�
									//block[j].l = block[t].l; block[j].r = block[t].r; block[j].b = block[t].b; block[j].u = block[t].u;	//�� �簢���� �ٿ���� �ڽ��� ����

									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[t].hexa_tri_index = j;	//�ﰢ�� �簢���� ���ӽ�Ŵ
									block[j].status = 6;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									//InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}
							//�ﰢ�� ������ (�θ� �簢��->�ڽ� �ﰢ��)
							block[i].s[0][0] = block[i].s[5][0]; block[i].s[0][1] = block[i].s[5][1];
							block[i].s[1][0] = SliceP1[0] - block[i].m_x; block[i].s[1][1] = SliceP1[1] - block[i].m_y;
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
						else if (line2to3 == 1 && line3to6 == 1)	//2-3,3-6�� �ڸ���
						{	//�ڽ� ������ �����
							for (int j = Block_num - 1; j >= 0; --j) {
								//�簢��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = SliceP1[0] - block[i].m_x; block[j].s[1][1] = SliceP1[1] - block[i].m_y; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP2[0] - block[i].m_x; block[j].s[2][1] = SliceP2[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[i].s[5][0]; block[j].s[5][1] = block[i].s[5][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//�������̹Ƿ� �ٿ�� �ڽ� ���Ƿ� �ø�
									t = j;

									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//�ﰢ��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP1[0] - block[i].m_x; block[j].s[2][1] = SliceP1[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//�ӵ� �簢���� ����

									block[t].hexa_tri_index = j;	//�ﰢ�� �簢���� ���ӽ�Ŵ
									block[j].status = 6;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									break;
								}
							}
							//�ﰢ�� ������ (�θ� �簢��->�ڽ� �ﰢ��)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[1][0] = SliceP2[0] - block[i].m_x; block[i].s[1][1] = SliceP2[1] - block[i].m_y;
							block[i].s[2][0]; block[i].s[2][1];
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
						else if (line1to2 == 1 && line2to3 == 1)	//1-2,2-3�� �ڸ���
						{	//�ڽ� ������ �����
							for (int j = Block_num - 1; j >= 0; --j) {
								//�簢��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = SliceP2[0] - block[i].m_x; block[j].s[1][1] = SliceP2[1] - block[i].m_y; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[i].s[5][0]; block[j].s[5][1] = block[i].s[5][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//�������̹Ƿ� �ٿ�� �ڽ� ���Ƿ� �ø�
									t = j;

									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//�ﰢ��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = SliceP1[0] - block[i].m_x; block[j].s[1][1] = SliceP1[1] - block[i].m_y; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[5][0]; block[j].s[2][1] = block[i].s[5][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//�ӵ� �簢���� ����

									block[t].hexa_tri_index = j;	//�ﰢ�� �簢���� ���ӽ�Ŵ
									block[j].status = 6;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									break;
								}
							}
							//�ﰢ�� ������ (�θ� �簢��->�ڽ� �ﰢ��)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;	//1
							block[i].s[1][0]; block[i].s[1][1]; 	//2
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;	//3
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
						else if (line1to2 == 1 && line1to6 == 1)	//1-2,1-6�� �ڸ���
						{	//�ڽ� ������ �����
							for (int j = Block_num - 1; j >= 0; --j) {
								//�簢��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP2[0] - block[i].m_x; block[j].s[0][1] = SliceP2[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[i].s[5][0]; block[j].s[5][1] = block[i].s[5][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

									//�ٿ�� �ڽ�
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//�������̹Ƿ� �ٿ�� �ڽ� ���Ƿ� �ø�
									t = j;

									// [�ٿ���� �ڽ� �ּ� & ����]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//�ﰢ��
								if (block[j].status == 0) {
									// �� ��ġ ����
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP2[0] - block[i].m_x; block[j].s[2][1] = SliceP2[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//ȭ��� ��ġ �θ�� ��������

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//���� �θ�κ��� �����޴´�.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//�ӵ� �簢���� ����

									block[t].hexa_tri_index = j;	//�ﰢ�� �簢���� ���ӽ�Ŵ
									block[j].status = 6;//������ Ȱ��ȭ

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									break;
								}
							}
							//�ﰢ�� ������ (�θ� �簢��->�ڽ� �ﰢ��)
							block[i].s[0][0]; block[i].s[0][1];	//1
							block[i].s[1][0] = SliceP1[0] - block[i].m_x; block[i].s[1][1] = SliceP1[1] - block[i].m_y; 	//2
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;	//3
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//�ӵ� �ٽ� ����

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao������Ʈ

							block[i].get_bb();	//�ٿ�� �ڽ� �ٽ� ����
							// [�ٿ���� �ڽ� �ּ� & ����]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//�ٿ�� �ڽ�
						}
					}



					mouse_c[0][0] = 1; mouse_c[0][1] = 0; mouse_c[0][2] = 0;	//������ ������
					mouse_c[1][0] = 1; mouse_c[1][1] = 0; mouse_c[1][2] = 0;
					InitVao(mouse_vao, mouse_vbo, 2, mouse_s, mouse_c);

					//break; //=> �ߺ�üũ ���� ( break�ϸ� �ѹ��� ������ ��Ⱑ �Ұ���, ��� �ߺ�üũ�� ����)
				}

			}
		}
	}
	else if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN && y > 560) {	// ä���� �� Ŭ��
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
		Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

		for (int i = 0; i < 24; ++i) {
			if (cell[i].fill_cell == 1 && X >= cell[i].s[0][0] && X <= cell[i].s[1][0] && Y >= cell[i].s[0][1] && Y <= cell[i].s[2][1]) {
				move_x = X; move_y = Y;

				cell_click_index = i;

				if (block[cell[i].index].hexa_tri_index != Block_num) {//������ - �ﰢ���� ����
					cell_click_follow = block[cell[i].index].hexa_tri_index;
					block[cell_click_follow].hexa_tri_index = cell_click_follow;	//�ű�⿡�� �ﰢ�� ������ ��ġ�� ����(��ο� �Լ����� �˻�)
				}
				else
					cell_click_follow = cell[i].index;

				cell_click = 1;
				break;
			}
		}
	}
	else if (b == GLUT_LEFT_BUTTON && s == GLUT_UP && y > 560 && cell_click == 1) {	//�� �� ���� �����̱� ����
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
		Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

		for (int i = 0; i < 24; ++i) {
			if (cell[i].fill_cell == 0 && X >= cell[i].s[0][0] && X <= cell[i].s[1][0] && Y >= cell[i].s[0][1] && Y <= cell[i].s[2][1]) {
				cell[i].index = cell[cell_click_index].index;
				int num = cell[i].index;

				//��ġ ����
				float tmp1 = block[num].l + block[num].r;
				if (tmp1 < 0)
					block[num].m_x = cell[i].s[0][0] + 0.125 + abs(tmp1) / 2.0f;
				else
					block[num].m_x = cell[i].s[0][0] + 0.125 - abs(tmp1) / 2.0f;

				if (cell[cell_click_index].s[0][1] > cell[i].s[0][1] + 0.3f)
					block[num].m_y -= 0.4f;
				else if(cell[cell_click_index].s[0][1] > cell[i].s[0][1] + 0.1f)
					block[num].m_y -= 0.2f;
				if (cell[cell_click_index].s[0][1] < cell[i].s[0][1] - 0.3f)
					block[num].m_y += 0.4f;
				else if (cell[cell_click_index].s[0][1] < cell[i].s[0][1] - 0.1f)
					block[num].m_y += 0.2f; 

				if (block[num].hexa_tri_index != Block_num) {//�������� �ﰢ�� ��ġ ��ȯ
					int tri_n = block[num].hexa_tri_index;

					block[tri_n].m_x = block[num].m_x;
					block[tri_n].m_y = block[num].m_y;
				}

				cell[cell_click_index].fill_cell = 0;
				cell[i].fill_cell = 1;

				GLfloat cell_empty[4][3] = {
				{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} // �ϴû�
				};
				GLfloat cell_fill[4][3] = {
				{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{1,0,0}  ,{1,0,0} // ���� ������
				};

				InitVao(cell[cell_click_index].cell_vao, cell[cell_click_index].cell_vbo, 4, cell[cell_click_index].s, cell_empty);
				InitVao(cell[i].cell_vao, cell[i].cell_vbo, 4, cell[i].s, cell_fill);
				break;
			}
		}

		cell_click = 0;
	}

	glutPostRedisplay();
}

void Motion(int x, int y)
{
	if (/*pause == 0 &&*/ y <= 560) {
		if (click == true)
		{
			GLfloat X, Y;

			X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
			Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

			float tx = (X - mouse_s[0][0]) * (X - mouse_s[0][0]);
			float ty = (Y - mouse_s[0][1]) * (Y - mouse_s[0][1]);

			if (sqrt(tx + ty) <= 0.6) {					//���� 0.6 ����
				mouse_s[1][0] = X; mouse_s[1][1] = Y;
			}

			InitVao(mouse_vao, mouse_vbo, 2, mouse_s, mouse_c);
		}
		cell_click = 0;

		glutPostRedisplay();
	}
	else if (y > 560 && cell_click == 1)	//�� �� ���� �����̱�
	{
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
		Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

		move_x = X; move_y = Y;

		glutPostRedisplay();
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		//������ ������
	case 'E':
		fill_num = 24;
		break;
		//�Ͻ�����
	case 'p':
	case 'P':
		if (pause == 0)
			pause = 1;
		else {
			pause = 0;
			glutTimerFunc(0, Timer_Block, 2);
		}
		break;
		//���ǵ�
	case '+':	//������!
		--speed;
		if (speed <= 6)
			speed = 5;
		break;
	case '-':	//������!
		++speed;
		if (speed >= 61)
			speed = 60;
		break;
		// ���� �Ǵ� �ﰢ��
	case 'l':
	case 'L':
		if (mode == GL_TRIANGLES)
			mode = GL_LINE_LOOP;
		else mode = GL_TRIANGLES;
		break;
		//��� ǥ��
	case 't':
	case 'T':
		if (trace == 0)
			trace = 1;
		else trace = 0;
		break;
		//�ٿ�� �ڽ� ����
	case 'b':
	case 'B':
		if (show_bb == 0)
			show_bb = 1;
		else show_bb = 0;
		break;
		//�ʱ� ����������
	case 'i':
	case 'I':
		break;
		//����
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

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 0);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Slice Flying Polygons");

	glewExperimental = GL_TRUE;
	glewInit();

	InitModel();
	InitShader();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(0, Timer_Block, 1);
	glutMainLoop();
}