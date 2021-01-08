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

//소리재생
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace std;


std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid{ 0.01,0.05 };

//모양(주소)
#define hexahedronpoints	36	//-> 총 점의 갯수 = 삼각형(폴리곤) 갯수 * 3(꼭짓점)
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

GLfloat hexa가슴_s[hexahedronpoints][3] = {};
GLfloat hexa허리_s[hexahedronpoints][3] = {};
GLfloat hexa골반_s[hexahedronpoints][3] = {};
GLfloat hexa어깨_s[hexahedronpoints][3] = {};
GLfloat hexa상박_s[hexahedronpoints][3] = {};
GLfloat hexa하박_s[hexahedronpoints][3] = {};
GLfloat hexa주먹1_s[hexahedronpoints][3] = {};
GLfloat hexa주먹2l_s[hexahedronpoints][3] = {}; GLfloat hexa주먹2r_s[hexahedronpoints][3] = {};
GLfloat hexa주먹3_s[hexahedronpoints][3] = {};
GLfloat hexa허벅지_s[hexahedronpoints][3] = {};
GLfloat hexa종아리_s[hexahedronpoints][3] = {};
GLfloat hexa발_s[hexahedronpoints][3] = {};

GLfloat 마왕관_s[hexahedronpoints][3] = {}; 
GLfloat 마가슴V_s[6][3] = {};

//법선
GLfloat hexa_vn[hexahedronpoints][3] = {};
GLfloat pyramid_vn[pyramidpoints][3] = {};
GLfloat sphere_vn[spherepoints][3] = {};
GLfloat ui_vn[6][3] = {
	{0,0,1} ,{0,0,1}, {0,0,1},
	{0,0,1} ,{0,0,1}, {0,0,1} };

GLfloat hexa가슴_vn[hexahedronpoints][3] = {};
GLfloat hexa허리_vn[hexahedronpoints][3] = {};
GLfloat hexa골반_vn[hexahedronpoints][3] = {};
GLfloat hexa어깨_vn[hexahedronpoints][3] = {};
GLfloat hexa상박_vn[hexahedronpoints][3] = {};
GLfloat hexa하박_vn[hexahedronpoints][3] = {};
GLfloat hexa주먹1_vn[hexahedronpoints][3] = {};
GLfloat hexa주먹2l_vn[hexahedronpoints][3] = {};
GLfloat hexa주먹3_vn[hexahedronpoints][3] = {};
GLfloat hexa허벅지_vn[hexahedronpoints][3] = {};
GLfloat hexa종아리_vn[hexahedronpoints][3] = {};
GLfloat hexa발_vn[hexahedronpoints][3] = {};

GLfloat 마왕관_vn[hexahedronpoints][3] = {};
GLfloat 마가슴V_vn[6][3] = {};


//텍스처 좌표
GLfloat hexa_vt[hexahedronpoints][2];
GLfloat pyramid_vt[pyramidpoints][2];
GLfloat sphere_vt[spherepoints][2] = {};
GLfloat ui_vt[6][2] = {
	{1,1} ,{0,0}, {0,1},
	{0,0} ,{1,1}, {1,0} };

GLfloat hexa가슴_vt[hexahedronpoints][2] = {};
GLfloat hexa허리_vt[hexahedronpoints][2] = {};
GLfloat hexa골반_vt[hexahedronpoints][2] = {};
GLfloat hexa어깨_vt[hexahedronpoints][2] = {};
GLfloat hexa상박_vt[hexahedronpoints][2] = {};
GLfloat hexa하박_vt[hexahedronpoints][2] = {};
GLfloat hexa주먹1_vt[hexahedronpoints][2] = {};
GLfloat hexa주먹2l_vt[hexahedronpoints][2] = {};
GLfloat hexa주먹3_vt[hexahedronpoints][2] = {};
GLfloat hexa허벅지_vt[hexahedronpoints][2] = {};
GLfloat hexa종아리_vt[hexahedronpoints][2] = {};
GLfloat hexa발_vt[hexahedronpoints][2] = {};

GLfloat 마왕관_vt[hexahedronpoints][2] = {};
GLfloat 마가슴V_vt[6][2] = {};


//vao,vbo
GLuint hexa_vao, hexa_vbo[3];
GLuint pyramid_vao, pyramid_vbo[3];
GLuint sphere_vao, sphere_vbo[3];
GLuint ui1_vao, ui1_vbo[3];
GLuint ui2_vao, ui2_vbo[3];

GLuint hexa가슴_vao, hexa가슴_vbo[3];
GLuint hexa허리_vao, hexa허리_vbo[3];
GLuint hexa골반_vao, hexa골반_vbo[3];
GLuint hexa어깨_vao, hexa어깨_vbo[3];
GLuint hexa상박_vao, hexa상박_vbo[3];
GLuint hexa하박_vao, hexa하박_vbo[3];
GLuint hexa주먹1_vao, hexa주먹1_vbo[3];
GLuint hexa주먹2l_vao, hexa주먹2l_vbo[3]; GLuint hexa주먹2r_vao, hexa주먹2r_vbo[3];
GLuint hexa주먹3_vao, hexa주먹3_vbo[3];
GLuint hexa허벅지_vao, hexa허벅지_vbo[3];
GLuint hexa종아리_vao, hexa종아리_vbo[3];
GLuint hexa발_vao, hexa발_vbo[3];

GLuint 마왕관_vao, 마왕관_vbo[3];
GLuint 마가슴V_vao, 마가슴V_vbo[3];


//전역 변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 1000.0f;
GLfloat window_h = 800.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, -1.5f, 6.0f);			//카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, -1.5f, 0.0f);	//카메라가 바라보는 기준점
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//카메라의 상단이 가리키는 방향

//일반 변수
//GLfloat move_x = 0, move_y = 0.0f, move_z = 0.0f;	//이동

bool start = 0;
float startlogo_size = 1.0;
bool click = 0;
float c_size = 0;
float x0_d = 0;
int character = 0;
bool projection = 1;
bool animation = 0;


//---관절 각도
GLfloat 
	//상반신
	목_x = 0, 목_y = 0,
	가슴_y = 0, 
	왼상박_x = 0, 왼상박_z = 0, 오른상박_x = 0, 오른상박_z = 0,
	왼하박_x = 0, 왼하박_z = 0, 오른하박_x = 0, 오른하박_z = 0,
	왼주먹_x = 0, 왼주먹_y = 0, 왼주먹_z = 0, 오른주먹_x = 0, 오른주먹_y = 0, 오른주먹_z = 0,
	//하반신
	골반_y = 0,
	왼허벅지_x = 0, 왼허벅지_z = 0, 오른허벅지_x = 0, 오른허벅지_z = 0,
	왼종아리_x = 0, 왼종아리_z = 0, 오른종아리_x = 0, 오른종아리_z = 0,
	왼발_x = 0, 오른발_x = 0;



//조명
GLfloat light_r = 1.0f, light_g = 1.0f, light_b = 1.0f;	//백색조명
GLfloat light_x = 0.0, light_y = 5.0, light_z = 10.0;

// 이미지
unsigned int
검은색, 노랑색, 빨간색, 아톰피부색, 옵티_파랑색, 회색, 흰색, 남색, 연두색, 좀검은흰색, 파란색,
태_f, 태_r, 태_l, 태_bo, 태_u, 태_ba, 태_가V,
마_f, 마_r, 마_l, 마_bo, 마_u, 마_ba, 마_가V,
아_f, 아_r, 아_l, 아_bo, 아_u, 아_ba,
터_f, 터_r, 터_l, 터_bo, 터_u, 터_ba,
옵_f, 옵_r, 옵_l, 옵_bo, 옵_u, 옵_ba,
시작화면, 로고, 엔터,
ui_오화, ui_왼화, ui_시계, ui_반시계, ui_나가기, ui_초기화, 
ui오테_빨, ui오테_파, ui오테_초,
ui_캐릭, ui_컨트롤, ui_사진,
ui_애니메이션, ui_복싱,
ui_상체,	 ui_목x, ui_목y, ui_가슴y, ui_왼상x, ui_왼상z, ui_왼하x, ui_왼하z, ui_왼주x, ui_왼주y, ui_왼주z, ui_오상x, ui_오상z, ui_오하x, ui_오하z, ui_오주x, ui_오주y, ui_오주z,
ui_하체,	 ui_골y, ui_왼허x, ui_왼허z, ui_왼종x, ui_왼종z, ui_왼발x, ui_오허x, ui_오허z, ui_오종x, ui_오종z, ui_오발x,
ui_찍기, ui_불러오기;

struct UI_coord {
	int x, y;
	float a;
};

UI_coord ui = { 0,1,0 };

class Body {
	//---모델 종류
	int model;



	//---관절 각도
	GLfloat
		//상반신
		목_x = 0, 목_y = 0,
		가슴_y = 0,
		왼상박_x = 0, 왼상박_z = 0, 오른상박_x = 0, 오른상박_z = 0,
		왼하박_x = 0, 왼하박_z = 0, 오른하박_x = 0, 오른하박_z = 0,
		왼주먹_x = 0, 왼주먹_y = 0, 왼주먹_z = 0, 오른주먹_x = 0, 오른주먹_y = 0, 오른주먹_z = 0,
		//하반신
		골반_y = 0,
		왼허벅지_x = 0, 왼허벅지_z = 0, 오른허벅지_x = 0, 오른허벅지_z = 0,
		왼종아리_x = 0, 왼종아리_z = 0, 오른종아리_x = 0, 오른종아리_z = 0,
		왼발_x = 0, 오른발_x = 0;
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

	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
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
		memset(count, '\0', sizeof(count)); // 배열 초기화
	}
	rewind(objFile);	// 읽기 위치 다시 맨 앞으로

	//--- 2. 메모리 할당
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

	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	char bind[100];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f", &vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			//확인 -//std::cout << "v["<< vertIndex <<"] - "<<vertex[vertIndex].x << ", " << vertex[vertIndex].y << ", " << vertex[vertIndex].z << std::endl;
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
	
	//--- 4. obj 버텍스 주소에 저장
	//주소값
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
	//법선값
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
	//텍스처 값
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

	//--- 5.메모리 반환
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
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
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
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void AssignObj_Variable() {
	// obj읽기
	//------------------------------------기본--------------------------------------------
	char filename[30] = "obj/정육면체.obj";
	FILE* file = fopen(filename, "rb");

	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa_s, hexa_vn, hexa_vt);

	strcpy(filename, "obj/사각뿔(피라미드).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, pyramid_s, pyramid_vn, pyramid_vt);

	//------------------------------------모델--------------------------------------------

	strcpy(filename, "obj/정육면체-가슴.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa가슴_s, hexa가슴_vn, hexa가슴_vt);

	strcpy(filename, "obj/정육면체-허리.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa허리_s, hexa허리_vn, hexa허리_vt);
	
	strcpy(filename, "obj/정육면체-골반.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa골반_s, hexa골반_vn, hexa골반_vt);

	strcpy(filename, "obj/정육면체-어깨.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

	strcpy(filename, "obj/정육면체-상박.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa상박_s, hexa상박_vn, hexa상박_vt);

	strcpy(filename, "obj/정육면체-하박.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa하박_s, hexa하박_vn, hexa하박_vt);

	strcpy(filename, "obj/정육면체-주먹1.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);

	strcpy(filename, "obj/정육면체-주먹2l.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);

	strcpy(filename, "obj/정육면체-주먹2r.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);

	strcpy(filename, "obj/정육면체-주먹3.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

	strcpy(filename, "obj/정육면체-허벅지.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

	strcpy(filename, "obj/정육면체-종아리.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

	strcpy(filename, "obj/정육면체-발.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa발_s, hexa발_vn, hexa발_vt);

	//------------------------------------특수 모양--------------------------------------------
	
	//---마징가
	strcpy(filename, "obj/정육면체-마징가왕관.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, 마왕관_s, 마왕관_vn, 마왕관_vt);

	strcpy(filename, "obj/정육면체-마가슴V.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, 마가슴V_s, 마가슴V_vn, 마가슴V_vt);


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
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기

	glGenBuffers(3, vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// 변수 triShape 에서 버텍스 데이터 값을 버퍼에 복사한다.
	// Shape 배열의 사이즈: num * 3 *float
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), Shape, GL_STATIC_DRAW);
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (법선)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), Normal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//--- 3번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (텍스처)을 저장
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

	InitVao(hexa가슴_vao, hexa가슴_vbo, hexahedronpoints, hexa가슴_s, hexa가슴_vn, hexa가슴_vt);
	InitVao(hexa허리_vao, hexa허리_vbo, hexahedronpoints, hexa허리_s, hexa허리_vn, hexa허리_vt);
	InitVao(hexa골반_vao, hexa골반_vbo, hexahedronpoints, hexa골반_s, hexa골반_vn, hexa골반_vt);
	InitVao(hexa어깨_vao, hexa어깨_vbo, hexahedronpoints, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
	InitVao(hexa상박_vao, hexa상박_vbo, hexahedronpoints, hexa상박_s, hexa상박_vn, hexa상박_vt);
	InitVao(hexa하박_vao, hexa하박_vbo, hexahedronpoints, hexa하박_s, hexa하박_vn, hexa하박_vt);
	InitVao(hexa주먹1_vao, hexa주먹1_vbo, hexahedronpoints, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
	InitVao(hexa주먹2l_vao, hexa주먹2l_vbo, hexahedronpoints, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt); InitVao(hexa주먹2r_vao, hexa주먹2r_vbo, hexahedronpoints, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
	InitVao(hexa주먹3_vao, hexa주먹3_vbo, hexahedronpoints, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
	InitVao(hexa허벅지_vao, hexa허벅지_vbo, hexahedronpoints, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
	InitVao(hexa종아리_vao, hexa종아리_vbo, hexahedronpoints, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
	InitVao(hexa발_vao, hexa발_vbo, hexahedronpoints, hexa발_s, hexa발_vn, hexa발_vt);

	InitVao(마왕관_vao, 마왕관_vbo, hexahedronpoints, 마왕관_s, 마왕관_vn, 마왕관_vt);
	InitVao(마가슴V_vao, 마왕관_vbo, 6, 마가슴V_s, 마가슴V_vn, 마가슴V_vt);
}

void InitTexture() {
	BITMAPINFO* bmp;
	int widthImage, heightImage, numberOfChannel;

	unsigned char* data;

	// -------------------------------------- [단색]---------------------------------------------------------
	{
		//검은색
		glGenTextures(1, &검은색); //--- 텍스처 생성
		glBindTexture(GL_TEXTURE_2D, 검은색); //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
		data = stbi_load("res/검은색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
		//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
		//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)
		//노란색
		glGenTextures(1, &노랑색);
		glBindTexture(GL_TEXTURE_2D, 노랑색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/노랑색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//빨간색
		glGenTextures(1, &빨간색);
		glBindTexture(GL_TEXTURE_2D, 빨간색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/빨간색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//아톰피부색
		glGenTextures(1, &아톰피부색);
		glBindTexture(GL_TEXTURE_2D, 아톰피부색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/아톰피부색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//옵티-파랑색
		glGenTextures(1, &옵티_파랑색);
		glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/옵티-파랑색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//회색
		glGenTextures(1, &회색);
		glBindTexture(GL_TEXTURE_2D, 회색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/회색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//흰색
		glGenTextures(1, &흰색);
		glBindTexture(GL_TEXTURE_2D, 흰색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/흰색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//남색
		glGenTextures(1, &남색);
		glBindTexture(GL_TEXTURE_2D, 남색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/남색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//연두색
		glGenTextures(1, &연두색);
		glBindTexture(GL_TEXTURE_2D, 연두색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/연두색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//좀검은흰색
		glGenTextures(1, &좀검은흰색);
		glBindTexture(GL_TEXTURE_2D, 좀검은흰색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/좀검은흰색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//파랑색
		glGenTextures(1, &파란색);
		glBindTexture(GL_TEXTURE_2D, 파란색);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/파란색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	// -------------------------------------- [메뉴]---------------------------------------------------------
	{
		//시작화면
		glGenTextures(1, &시작화면);
		glBindTexture(GL_TEXTURE_2D, 시작화면);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/Realsteel.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//로고
		glGenTextures(1, &로고);
		glBindTexture(GL_TEXTURE_2D, 로고);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/Realsteel-logo.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//엔터
		glGenTextures(1, &엔터);
		glBindTexture(GL_TEXTURE_2D, 엔터);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/엔터.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	// -------------------------------------- [ui]---------------------------------------------------------
	{
		glGenTextures(1, &ui_오화);
		glBindTexture(GL_TEXTURE_2D, ui_오화);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼화);
		glBindTexture(GL_TEXTURE_2D, ui_왼화);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_시계);
		glBindTexture(GL_TEXTURE_2D, ui_시계);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_시계방향.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_반시계);
		glBindTexture(GL_TEXTURE_2D, ui_반시계);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_반시계방향.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_나가기);
		glBindTexture(GL_TEXTURE_2D, ui_나가기);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_나가기.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_초기화);
		glBindTexture(GL_TEXTURE_2D, ui_초기화);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_초기화.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui오테_빨);
		glBindTexture(GL_TEXTURE_2D, ui오테_빨);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui오테_빨.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui오테_파);
		glBindTexture(GL_TEXTURE_2D, ui오테_파);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui오테_파.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui오테_초);
		glBindTexture(GL_TEXTURE_2D, ui오테_초);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui오테_초.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_캐릭);
		glBindTexture(GL_TEXTURE_2D, ui_캐릭);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_캐릭선택.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_컨트롤);
		glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_컨트롤.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//-------[컨트롤]
		glGenTextures(1, &ui_상체);
		glBindTexture(GL_TEXTURE_2D, ui_상체);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_상체.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_하체);
		glBindTexture(GL_TEXTURE_2D, ui_하체);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_하체.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//---[상체]
		glGenTextures(1, &ui_목x);
		glBindTexture(GL_TEXTURE_2D, ui_목x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_목x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_목y);
		glBindTexture(GL_TEXTURE_2D, ui_목y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_목y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_가슴y);
		glBindTexture(GL_TEXTURE_2D, ui_가슴y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_가슴y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼상x);
		glBindTexture(GL_TEXTURE_2D, ui_왼상x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼상박x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼상z);
		glBindTexture(GL_TEXTURE_2D, ui_왼상z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼상박z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼하x);
		glBindTexture(GL_TEXTURE_2D, ui_왼하x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼하박x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼하z);
		glBindTexture(GL_TEXTURE_2D, ui_왼하z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼하박z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼주x);
		glBindTexture(GL_TEXTURE_2D, ui_왼주x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼주먹x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼주y);
		glBindTexture(GL_TEXTURE_2D, ui_왼주y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼주먹y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼주z);
		glBindTexture(GL_TEXTURE_2D, ui_왼주z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼주먹z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오상x);
		glBindTexture(GL_TEXTURE_2D, ui_오상x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른상박x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오상z);
		glBindTexture(GL_TEXTURE_2D, ui_오상z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른상박z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오하x);
		glBindTexture(GL_TEXTURE_2D, ui_오하x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른하박x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오하z);
		glBindTexture(GL_TEXTURE_2D, ui_오하z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른하박z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오주x);
		glBindTexture(GL_TEXTURE_2D, ui_오주x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른주먹x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오주y);
		glBindTexture(GL_TEXTURE_2D, ui_오주y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른주먹y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오주z);
		glBindTexture(GL_TEXTURE_2D, ui_오주z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른주먹z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//---[하체]
		glGenTextures(1, &ui_골y);
		glBindTexture(GL_TEXTURE_2D, ui_골y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_골반y.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼허x);
		glBindTexture(GL_TEXTURE_2D, ui_왼허x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼허벅지x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼허z);
		glBindTexture(GL_TEXTURE_2D, ui_왼허z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼허벅지z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼종x);
		glBindTexture(GL_TEXTURE_2D, ui_왼종x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼종아리x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼종z);
		glBindTexture(GL_TEXTURE_2D, ui_왼종z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼종아리z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_왼발x);
		glBindTexture(GL_TEXTURE_2D, ui_왼발x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_왼발x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오허x);
		glBindTexture(GL_TEXTURE_2D, ui_오허x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른허벅지x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오허z);
		glBindTexture(GL_TEXTURE_2D, ui_오허z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른허벅지z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오종x);
		glBindTexture(GL_TEXTURE_2D, ui_오종x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른종아리x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오종z);
		glBindTexture(GL_TEXTURE_2D, ui_오종z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른종아리z.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_오발x);
		glBindTexture(GL_TEXTURE_2D, ui_오발x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_오른발x.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		//---사진
		glGenTextures(1, &ui_사진);
		glBindTexture(GL_TEXTURE_2D, ui_사진);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_사진.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_찍기);
		glBindTexture(GL_TEXTURE_2D, ui_찍기);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_찍기.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_불러오기);
		glBindTexture(GL_TEXTURE_2D, ui_불러오기);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_불러오기.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_애니메이션);
		glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_애니메이션.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenTextures(1, &ui_복싱);
		glBindTexture(GL_TEXTURE_2D, ui_복싱);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/ui/ui_복싱.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	// -------------------------------------- [모델]---------------------------------------------------------
	//-----------태권V
	{
		//u
		glGenTextures(1, &태_u);
		glBindTexture(GL_TEXTURE_2D, 태_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/검은색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &태_bo);
		glBindTexture(GL_TEXTURE_2D, 태_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/빨간색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &태_f);
		glBindTexture(GL_TEXTURE_2D, 태_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/태권Vf.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &태_ba);
		glBindTexture(GL_TEXTURE_2D, 태_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/태권Vb.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &태_r);
		glBindTexture(GL_TEXTURE_2D, 태_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/태권Vr.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &태_l);
		glBindTexture(GL_TEXTURE_2D, 태_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/태권Vl.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//태권V 가슴V
		glGenTextures(1, &태_가V);
		glBindTexture(GL_TEXTURE_2D, 태_가V);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/태권V-가슴V.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------마징가Z
	{
		//u
		glGenTextures(1, &마_u);
		glBindTexture(GL_TEXTURE_2D, 마_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/회색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &마_bo);
		glBindTexture(GL_TEXTURE_2D, 마_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/회색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &마_f);
		glBindTexture(GL_TEXTURE_2D, 마_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/마징가Zf.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &마_ba);
		glBindTexture(GL_TEXTURE_2D, 마_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/마징가Zb.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &마_r);
		glBindTexture(GL_TEXTURE_2D, 마_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/마징가Zr.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &마_l);
		glBindTexture(GL_TEXTURE_2D, 마_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/마징가Zl.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); 
		//가슴V
		glGenTextures(1, &마_가V);
		glBindTexture(GL_TEXTURE_2D, 마_가V);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/마징가-가슴V.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------아톰
	{
		//u
		glGenTextures(1, &아_u);
		glBindTexture(GL_TEXTURE_2D, 아_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/검은색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &아_bo);
		glBindTexture(GL_TEXTURE_2D, 아_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/아톰피부색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &아_f);
		glBindTexture(GL_TEXTURE_2D, 아_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/아톰f.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &아_ba);
		glBindTexture(GL_TEXTURE_2D, 아_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/아톰b.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &아_r);
		glBindTexture(GL_TEXTURE_2D, 아_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/아톰r.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &아_l);
		glBindTexture(GL_TEXTURE_2D, 아_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/아톰l.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------터미네이터
	{
		//u
		glGenTextures(1, &터_u);
		glBindTexture(GL_TEXTURE_2D, 터_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/회색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &터_bo);
		glBindTexture(GL_TEXTURE_2D, 터_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/회색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &터_f);
		glBindTexture(GL_TEXTURE_2D, 터_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/터미네이터f.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &터_ba);
		glBindTexture(GL_TEXTURE_2D, 터_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/터미네이터b.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &터_r);
		glBindTexture(GL_TEXTURE_2D, 터_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/터미네이터r.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &터_l);
		glBindTexture(GL_TEXTURE_2D, 터_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/터미네이터l.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }
	//-----------옵티머스
	{
		//u
		glGenTextures(1, &옵_u);
		glBindTexture(GL_TEXTURE_2D, 옵_u);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/옵티-파랑색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//bo
		glGenTextures(1, &옵_bo);
		glBindTexture(GL_TEXTURE_2D, 옵_bo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/옵티-파랑색.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//f
		glGenTextures(1, &옵_f);
		glBindTexture(GL_TEXTURE_2D, 옵_f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/옵티머스f.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//ba
		glGenTextures(1, &옵_ba);
		glBindTexture(GL_TEXTURE_2D, 옵_ba);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/옵티머스b.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//r
		glGenTextures(1, &옵_r);
		glBindTexture(GL_TEXTURE_2D, 옵_r);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/옵티머스r.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//l
		glGenTextures(1, &옵_l);
		glBindTexture(GL_TEXTURE_2D, 옵_l);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("res/옵티머스l.png", &widthImage, &heightImage, &numberOfChannel, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); }

	glUseProgram(s_program);
	int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정

	stbi_image_free(data);	//---메모리 반환
}

void MoveDown(GLuint* vao, GLuint *vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][1] -= d;
		s[i][1] = round(s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveUp(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][1] += d;
		s[i][1] = round(s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveLeft(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][0] -= d;
		s[i][0] = round(s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveRight(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][0] += d;
		s[i][0] = round(s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveBack(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][2] -= d;
		s[i][2] = round(s[i][2] * 1000) / 1000;	//소수점3자리까지 반올림
	}
	InitVao(*vao, vbo, hexahedronpoints, s, vn, vt);
}

void MoveForward(GLuint* vao, GLuint* vbo, GLfloat d, GLfloat s[36][3], GLfloat vn[36][3], GLfloat vt[36][2]) {
	for (int i = 0; i < hexahedronpoints; ++i) {
		s[i][2] += d;
		s[i][2] = round(s[i][2] * 1000) / 1000;	//소수점3자리까지 반올림
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

	glEnable(GL_DEPTH_TEST);	// 은면 제거
	//glEnable(GL_CULL_FACE);		// 뒷면제거

	unsigned int modelLoc = glGetUniformLocation(s_program, "model");	//--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	unsigned int viewLoc = glGetUniformLocation(s_program, "view");		//--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	unsigned int projLoc = glGetUniformLocation(s_program, "projection");//--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.
	glm::mat4 mTransform = glm::mat4(1.0f);		  // 단위행렬로 초기화
	glm::mat4 vTransform = glm::mat4(1.0f);		  // 단위행렬로 초기화
	glm::mat4 pTransform = glm::mat4(1.0f);		  // 단위행렬로 초기화

	vTransform = glm::mat4(1.0f);	// 카메라 변환
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLoc = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	pTransform = glm::mat4(1.0f);	// 투영 변환
	if (projection == 1)	//원근 투영
		pTransform = glm::perspective(glm::radians(45.0f), window_w / window_h, 0.1f, 50.0f);
	else					//직각 투영
		pTransform = glm::ortho(-3.0, 3.0, -3.0, 3.0, 0.1, 50.0);
	projLoc = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	//조명
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos");		//--- lightPos 값 전달
	glUniform3f(lightPosLocation, light_x, light_y, light_z);
	int lightColorLocation = glGetUniformLocation(s_program, "lightColor");	//--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, light_r, light_g, light_b);
	int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
	glUniform3f(objColorLocation, 1, 1, 1);	// --- 백색 조명

	//텍스쳐 활성화
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);	//투명
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (start == 0)
	{
		//배경
		glBindVertexArray(hexa_vao);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::translate(mTransform, glm::vec3(0, -1.5, 4.8));
		mTransform = glm::scale(mTransform, glm::vec3(2.5, 1.1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, 시작화면);
		glDrawArrays(GL_TRIANGLES, 30, 6);	///-앞

		//로고
		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::translate(mTransform, glm::vec3(0, -1.15, 4.801));
		mTransform = glm::scale(mTransform, glm::vec3(1.3, 0.5, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, 로고);
		glDrawArrays(GL_TRIANGLES, 30, 6);	///-앞

		//엔터버튼
		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::translate(mTransform, glm::vec3(0.0, -1.7, 5.25));
		mTransform = glm::scale(mTransform, glm::vec3(0.75 * startlogo_size, 0.25 * startlogo_size, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, 엔터);
		glDrawArrays(GL_TRIANGLES, 30, 6);	///-앞
	}
	else
	{
		//경기장
		glBindVertexArray(hexa_vao);
		{
			//링-바닥
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -3.55, 0));
			mTransform = glm::scale(mTransform, glm::vec3(50, 1.0, 50));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, 좀검은흰색);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		{
			//링-봉4개
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.75, -1.3, 11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, 검은색);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.75, -1.3, 11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, 검은색);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.75, -1.3, -11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, 검은색);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.75, -1.3, -11.75));
			mTransform = glm::scale(mTransform, glm::vec3(1, 8, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glBindTexture(GL_TEXTURE_2D, 검은색);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		{
			//링-맨윗줄-빨간색
			glBindTexture(GL_TEXTURE_2D, 빨간색);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.1, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.1, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, 0.1, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, 0.1, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//링-중간줄1-흰색
			glBindTexture(GL_TEXTURE_2D, 흰색);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -0.8, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -0.8, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, -0.8, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, -0.8, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//링-중간줄2-흰색
			glBindTexture(GL_TEXTURE_2D, 흰색);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -1.6, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -1.6, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, -1.6, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, -1.6, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//링-맨아래줄-파란색
			glBindTexture(GL_TEXTURE_2D, 파란색);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -2.5, 11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(0, -2.5, -11.8));
			mTransform = glm::scale(mTransform, glm::vec3(49, 0.3, 0.3));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(11.8, -2.5, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::translate(mTransform, glm::vec3(-11.8, -2.5, 0));
			mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.3, 49));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//UI

		//오른편 인터페이스
		//x=0
		if (ui.x == 0 and click == 0) {
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_캐릭);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_사진);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			{
				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76 - (ui.y - 1) * 0.2, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			{
				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=0,y=1
		else if (ui.x == 1 and (ui.y/10) == 1 || ui.x == 0 and ui.y == 1 and click == 1) {
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_캐릭);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_사진);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, 태_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, 마_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, 옵_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, 아_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, 터_f);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			if (click == 1 and (ui.y/10) == 1)
			{
				if (c_size < 1) {
					glBindVertexArray(ui1_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_빨);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75 - (ui.y - 11) * 0.2, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_빨);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
			else {
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75 - (ui.y - 11) * 0.2, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.75, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=0,y=2
		else if (ui.x == 1 and (ui.y / 10) == 2  and c_size <= 1 || ui.x == 0 and ui.y == 2 and click == 1) {
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_캐릭);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_사진);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_상체);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_하체);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			if (click == 1 and ui.y == 21)
			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_빨);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else {
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95 - (ui.y - 21) * 0.2, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=1,y=21
		else if (ui.x == 2 and (ui.y / 100) == 21 || ui.x == 1 and ui.y == 21 and click == 1) {
			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_캐릭);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_사진);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.55 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_상체);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_하체);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				//------관절
				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_목x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.35, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_목y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_가슴y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼상x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼상z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼하x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.8, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼하z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼주x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼주y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼주z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오상x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오상z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오하x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.25, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오하z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.4, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오주x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.4, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오주y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -2.4, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오주z);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (click == 1 and ui.y == 2101)
			{
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_빨);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if(click == 0) {
				if (ui.y <= 2102) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2101) * 0.15, -1.35, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2103) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2105) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2104) * 0.15, -1.65, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2107) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2106) * 0.15, -1.8, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2110) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2108) * 0.15, -1.95, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2112) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2111) * 0.15, -2.1, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2114) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2113) * 0.15, -2.25, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2117) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2115) * 0.15, -2.4, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.56 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				//------관절
				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.8, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.25, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.4, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.4, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.95, -2.4, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=1,y=22
		else if (ui.x == 2 and (ui.y / 100) == 22 || ui.x == 1 and ui.y == 22 and click == 1) {
			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_캐릭);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_사진);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.55 + x0_d, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_상체);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_하체);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				//------관절
				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_골y);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼허x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.5, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼허z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼종x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼종z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_왼발x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오허x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오허z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오종x);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오종z);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.7));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui_오발x);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			if (click == 1 and ui.y == 2201)
			{
				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_빨);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			else if (click == 0){
				if (ui.y <= 2201) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 , -1.35, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2203) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2202) * 0.15, -1.5, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2205) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2204) * 0.15, -1.65, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2206) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2208) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2207) * 0.15, -1.95, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2210) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65 + (ui.y - 2209) * 0.15, -2.1, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				else if (ui.y <= 2211) {
					glBindVertexArray(ui2_vao);

					mTransform = glm::mat4(1.0f);	//모델 변환
					modelLoc = glGetUniformLocation(s_program, "model");
					mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
					mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.71));
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindTexture(GL_TEXTURE_2D, ui오테_파);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

			{
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.56 + x0_d, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);


				glBindVertexArray(ui2_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.95, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_파);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				//------관절
				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.35, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.5, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.5, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.65, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.8, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -1.95, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.8, -2.1, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -2.25, 3.71));
				mTransform = glm::scale(mTransform, glm::vec3(1, c_size - 1, 1));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_초);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//x=0,y=3
		else if (ui.x == 1 and (ui.y / 10) == 3 || ui.x == 0 and ui.y == 3 and click == 1) {
		glBindVertexArray(ui1_vao);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_캐릭);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_사진);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35 + x0_d, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindVertexArray(ui2_vao);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_찍기);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_불러오기);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		if (click == 1 and (ui.y / 10) == 3)
		{
			if (c_size != 1) {
				glBindVertexArray(ui1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, ui오테_빨);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		else {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15 - (ui.y - 31) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_빨);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		}
		else {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15 - (ui.y - 31) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_파);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		{
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_파);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36 + x0_d, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.15, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		}
		//x=0,y=4
		else if (ui.x == 1 and (ui.y / 10) == 4 || ui.x == 0 and ui.y == 4 and click == 1) {
		glBindVertexArray(ui1_vao);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.75, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_캐릭);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.95, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_컨트롤);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.15, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_사진);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.35, 3.7));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_애니메이션);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glBindVertexArray(ui2_vao);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, ui_복싱);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		mTransform = glm::mat4(1.0f);	//모델 변환
		modelLoc = glGetUniformLocation(s_program, "model");
		mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.7));
		mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindTexture(GL_TEXTURE_2D, 연두색);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		if (animation == 1) {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35 - (ui.y - 41) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_빨);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (click == 1 and (ui.y / 10) == 4)
		{
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_빨);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else {
			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35 - (ui.y - 41) * 0.2, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_파);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		{
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.76, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -0.96, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.16, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.55, -1.36, 3.71));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_파);
			glDrawArrays(GL_TRIANGLES, 0, 6);


			glBindVertexArray(ui2_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.35, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.75, -1.55, 3.71));
			mTransform = glm::scale(mTransform, glm::vec3(1, c_size, 1));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui오테_초);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		}

		//-----------태권V
		if (character == 1) {
			{//머리
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 태_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-뒤

				glBindTexture(GL_TEXTURE_2D, 태_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-왼

				glBindTexture(GL_TEXTURE_2D, 태_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-위

				glBindTexture(GL_TEXTURE_2D, 태_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-오

				glBindTexture(GL_TEXTURE_2D, 태_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-아래

				glBindTexture(GL_TEXTURE_2D, 태_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-앞

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//뿔1
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += (0.1 +0.03);
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] += (0.8 - 0.2);
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.7, 0.7, 0.7));
				mTransform = glm::translate(mTransform, glm::vec3(0.03, 0.09, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.7, 0.7, 0.7));
				mTransform = glm::translate(mTransform, glm::vec3(-0.03, 0.09, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= (0.1 + 0.03)*2;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += (0.1 + 0.03);
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] -= (0.8 -0.2);
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//뿔2
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 노랑색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 노랑색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//가슴
				glBindVertexArray(hexa가슴_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, 30);

				glBindTexture(GL_TEXTURE_2D, 태_가V);
				glDrawArrays(GL_TRIANGLES, 30, 6);
			}
			{//어깨
				glBindVertexArray(hexa어깨_vao);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
			}
			{//상박
				glBindVertexArray(hexa상박_vao);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveDown(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveUp(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);
			}
			{//하박
				glBindVertexArray(hexa하박_vao);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveDown(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 파란색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 파란색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveUp(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);

				//---가시
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][1] += 0.15;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.4, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.85, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.4, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.85, 1.5, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][1] -= 0.15;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//주먹
				MoveDown(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveDown(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveDown(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 파란색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2l_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveRight(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveDown(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveRight(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 파란색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2r_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveUp(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
			}
			{//허리
				glBindVertexArray(hexa허리_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//골반
				glBindVertexArray(hexa골반_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//허벅지
				glBindVertexArray(hexa허벅지_vao);

				MoveDown(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
			}
			{//종아리
				glBindVertexArray(hexa종아리_vao);

				MoveDown(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
			}
			{//발
				glBindVertexArray(hexa발_vao);

				MoveDown(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveForward(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveBack(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
			}
		}
		//-----------마징가Z
		else if (character == 2) {
			{//머리
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 마_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-뒤

				glBindTexture(GL_TEXTURE_2D, 마_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-왼

				glBindTexture(GL_TEXTURE_2D, 마_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-위

				glBindTexture(GL_TEXTURE_2D, 마_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-오

				glBindTexture(GL_TEXTURE_2D, 마_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-아래

				glBindTexture(GL_TEXTURE_2D, 마_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-앞

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//왕관
				glBindVertexArray(마왕관_vao);

				MoveUp(&마왕관_vao, 마왕관_vbo, 0.15 + 0.25 + 0.075, 마왕관_s, 마왕관_vn, 마왕관_vt);
				MoveLeft(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);
				MoveRight(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveDown(&마왕관_vao, 마왕관_vbo, 0.15 + 0.25 + 0.075, 마왕관_s, 마왕관_vn, 마왕관_vt);
				MoveLeft(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);
			}
			{//뿔
				glBindVertexArray(pyramid_vao);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 노랑색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] += 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] += 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 노랑색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				for (int i = 0; i < pyramidpoints; ++i) {
					pyramid_s[i][0] -= 0.25;
					pyramid_s[i][0] = round(pyramid_s[i][0] * 1000) / 1000;	//소수점3자리까지 반올림
					pyramid_s[i][1] -= 0.11;
					pyramid_s[i][1] = round(pyramid_s[i][1] * 1000) / 1000;	//소수점3자리까지 반올림
				}
				InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);
			}
			{//가슴
				glBindVertexArray(hexa가슴_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//어깨
				glBindVertexArray(hexa어깨_vao);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
			}
			{//상박
				glBindVertexArray(hexa상박_vao);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveDown(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveUp(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);
			}
			{//하박
				glBindVertexArray(hexa하박_vao);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveDown(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveUp(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);
			}
			{//주먹
				MoveDown(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveDown(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveDown(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2l_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveRight(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveDown(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveRight(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2r_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveUp(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
			}
			{//허리
				glBindVertexArray(hexa허리_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//골반
				glBindVertexArray(hexa골반_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//허벅지
				glBindVertexArray(hexa허벅지_vao);

				MoveDown(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
			}
			{//종아리
				glBindVertexArray(hexa종아리_vao);

				MoveDown(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
			}
			{//발
				glBindVertexArray(hexa발_vao);

				MoveDown(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveForward(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 남색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveBack(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
			}
			{//마징가 가슴V
				glBindVertexArray(마가슴V_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.45, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 마_가V);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		//-----------옵티머스 프라임
		else if (character == 3) {
			{//머리
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-뒤

				glBindTexture(GL_TEXTURE_2D, 옵_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-왼

				glBindTexture(GL_TEXTURE_2D, 옵_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-위

				glBindTexture(GL_TEXTURE_2D, 옵_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-오

				glBindTexture(GL_TEXTURE_2D, 옵_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-아래

				glBindTexture(GL_TEXTURE_2D, 옵_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-앞

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//더듬이
				glBindVertexArray(마왕관_vao);

				MoveUp(&마왕관_vao, 마왕관_vbo, 0.15 + 0.25 + 0.075, 마왕관_s, 마왕관_vn, 마왕관_vt);
				MoveLeft(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 1, 0.3));
				mTransform = glm::translate(mTransform, glm::vec3(-0.25, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);
				MoveRight(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 1, 0.3));
				mTransform = glm::translate(mTransform, glm::vec3(0.25, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveDown(&마왕관_vao, 마왕관_vbo, 0.15 + 0.25 + 0.075, 마왕관_s, 마왕관_vn, 마왕관_vt);
				MoveLeft(&마왕관_vao, 마왕관_vbo, 0.2, 마왕관_s, 마왕관_vn, 마왕관_vt);
			}
			{//가슴
				glBindVertexArray(hexa가슴_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//어깨
				glBindVertexArray(hexa어깨_vao);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
			}
			{//상박
				glBindVertexArray(hexa상박_vao);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveDown(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveUp(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);
			}
			{//하박
				glBindVertexArray(hexa하박_vao);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveDown(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveUp(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);
			}
			{//주먹
				MoveDown(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveDown(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveDown(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2l_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveRight(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveDown(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveRight(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2r_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveUp(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
			}
			{//허리
				glBindVertexArray(hexa허리_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 흰색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//골반
				glBindVertexArray(hexa골반_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//허벅지
				glBindVertexArray(hexa허벅지_vao);

				MoveDown(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
			}
			{//종아리
				glBindVertexArray(hexa종아리_vao);

				MoveDown(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
			}
			{//발
				glBindVertexArray(hexa발_vao);

				MoveDown(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveForward(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 옵티_파랑색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveBack(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
			}
		}
		//-----------아톰
		else if (character == 4) {
			{//머리
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-뒤

				glBindTexture(GL_TEXTURE_2D, 아_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-왼

				glBindTexture(GL_TEXTURE_2D, 아_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-위

				glBindTexture(GL_TEXTURE_2D, 아_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-오

				glBindTexture(GL_TEXTURE_2D, 아_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-아래

				glBindTexture(GL_TEXTURE_2D, 아_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-앞

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{// 머리카락
				glBindVertexArray(pyramid_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 1, 0.5));
				mTransform = glm::rotate(mTransform, glm::radians(-20.0f), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.4, 0.37, 0.3));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-30.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.5, 1));
				mTransform = glm::rotate(mTransform, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(-120.0f), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.43, 0.1, 0.51));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

			}
			{//가슴
				glBindVertexArray(hexa가슴_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//어깨
				glBindVertexArray(hexa어깨_vao);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
			}
			{//상박
				glBindVertexArray(hexa상박_vao);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveDown(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveUp(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);
			}
			{//하박
				glBindVertexArray(hexa하박_vao);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveDown(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveUp(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);
			}
			{//주먹
				MoveDown(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveDown(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveDown(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2l_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveRight(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveDown(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveRight(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2r_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveUp(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
			}
			{//허리
				glBindVertexArray(hexa허리_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 연두색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//골반
				glBindVertexArray(hexa골반_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 검은색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//허벅지
				glBindVertexArray(hexa허벅지_vao);

				MoveDown(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 아톰피부색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
			}
			{//종아리
				glBindVertexArray(hexa종아리_vao);

				MoveDown(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
			}
			{//발
				glBindVertexArray(hexa발_vao);

				MoveDown(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveForward(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 빨간색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveBack(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
			}
		}
		//-----------터미네이터 t-850
		else if (character == 5) {
			{//머리
				glBindVertexArray(hexa_vao);

				MoveUp(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				mTransform = glm::rotate(mTransform, glm::radians(목_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(목_y), glm::vec3(0.0, 1.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 터_ba);
				glDrawArrays(GL_TRIANGLES, 0, 6);	//-뒤

				glBindTexture(GL_TEXTURE_2D, 터_l);
				glDrawArrays(GL_TRIANGLES, 6, 6);	//-왼

				glBindTexture(GL_TEXTURE_2D, 터_u);
				glDrawArrays(GL_TRIANGLES, 12, 6);	//-위

				glBindTexture(GL_TEXTURE_2D, 터_r);
				glDrawArrays(GL_TRIANGLES, 18, 6);	//-오

				glBindTexture(GL_TEXTURE_2D, 터_bo);
				glDrawArrays(GL_TRIANGLES, 24, 6);	//-아래

				glBindTexture(GL_TEXTURE_2D, 터_f);
				glDrawArrays(GL_TRIANGLES, 30, 6);	//-앞

				MoveDown(&hexa_vao, hexa_vbo, 0.25, hexa_s, hexa_vn, hexa_vt);
			}
			{//가슴
				glBindVertexArray(hexa가슴_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//어깨
				glBindVertexArray(hexa어깨_vao);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
				MoveRight(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.4, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa어깨_vao, hexa어깨_vbo, 0.65, hexa어깨_s, hexa어깨_vn, hexa어깨_vt);
			}
			{//상박
				glBindVertexArray(hexa상박_vao);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveDown(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveRight(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa상박_vao, hexa상박_vbo, 0.65, hexa상박_s, hexa상박_vn, hexa상박_vt);
				MoveUp(&hexa상박_vao, hexa상박_vbo, 0.28, hexa상박_s, hexa상박_vn, hexa상박_vt);
			}
			{//하박
				glBindVertexArray(hexa하박_vao);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveDown(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveRight(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveLeft(&hexa하박_vao, hexa하박_vbo, 0.65, hexa하박_s, hexa하박_vn, hexa하박_vt);
				MoveUp(&hexa하박_vao, hexa하박_vbo, 0.3, hexa하박_s, hexa하박_vn, hexa하박_vt);
			}
			{//주먹
				MoveDown(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveDown(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveDown(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2l_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveRight(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.65, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹2l_vao, hexa주먹2l_vbo, 0.05, hexa주먹2l_s, hexa주먹2l_vn, hexa주먹2l_vt);
				//---------------------------------------------------------------------
				MoveRight(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveRight(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);

				MoveDown(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveRight(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);

				glBindVertexArray(hexa주먹1_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.65, 0, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹2r_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.2, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				glBindVertexArray(hexa주먹3_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(가슴_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0.65, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른상박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른하박_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.55, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른주먹_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.1, 0.05));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa주먹1_vao, hexa주먹1_vbo, 0.15, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveLeft(&hexa주먹1_vao, hexa주먹1_vbo, 0.65, hexa주먹1_s, hexa주먹1_vn, hexa주먹1_vt);
				MoveUp(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.05, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveLeft(&hexa주먹2r_vao, hexa주먹2r_vbo, 0.65, hexa주먹2r_s, hexa주먹2l_vn, hexa주먹2l_vt);
				MoveUp(&hexa주먹3_vao, hexa주먹3_vbo, 0.05, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
				MoveLeft(&hexa주먹3_vao, hexa주먹3_vbo, 0.65, hexa주먹3_s, hexa주먹3_vn, hexa주먹3_vt);
			}
			{//허리
				glBindVertexArray(hexa허리_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.18, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//골반
				glBindVertexArray(hexa골반_vao);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.71, 0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//허벅지
				glBindVertexArray(hexa허벅지_vao);

				MoveDown(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveRight(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa허벅지_vao, hexa허벅지_vbo, 0.3, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
				MoveLeft(&hexa허벅지_vao, hexa허벅지_vbo, 0.2, hexa허벅지_s, hexa허벅지_vn, hexa허벅지_vt);
			}
			{//종아리
				glBindVertexArray(hexa종아리_vao);

				MoveDown(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveRight(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa종아리_vao, hexa종아리_vbo, 0.3, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
				MoveLeft(&hexa종아리_vao, hexa종아리_vbo, 0.2, hexa종아리_s, hexa종아리_vn, hexa종아리_vt);
			}
			{//발
				glBindVertexArray(hexa발_vao);

				MoveDown(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveForward(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(왼종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(왼발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveRight(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);

				mTransform = glm::mat4(1.0f);	//모델 변환
				modelLoc = glGetUniformLocation(s_program, "model");
				mTransform = glm::rotate(mTransform, glm::radians(골반_y), glm::vec3(0.0, 1.0, 0.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -1.91, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른허벅지_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_x), glm::vec3(1.0, 0.0, 0.0));
				mTransform = glm::rotate(mTransform, glm::radians(오른종아리_z), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.6, 0));
				mTransform = glm::rotate(mTransform, glm::radians(오른발_x), glm::vec3(1.0, 0.0, 0.0));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindTexture(GL_TEXTURE_2D, 회색);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				MoveUp(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
				MoveLeft(&hexa발_vao, hexa발_vbo, 0.2, hexa발_s, hexa발_vn, hexa발_vt);
				MoveBack(&hexa발_vao, hexa발_vbo, 0.1, hexa발_s, hexa발_vn, hexa발_vt);
			}
		}

		{
			//투명객체 맨 나중에 그려야함
			//UI
			//화살표 
			glBindVertexArray(ui1_vao);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(-0.1, -1.85, 4.9));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_왼화);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.1, -1.85, 4.9));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_오화);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(-0.45, -1.85, 4.9));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_초기화);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(-0.55, -1.1, 4.75));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_나가기);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0.89, -0.63, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_반시계);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			mTransform = glm::mat4(1.0f);	//모델 변환
			modelLoc = glGetUniformLocation(s_program, "model");
			mTransform = glm::rotate(mTransform, glm::radians(ui.a), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(1.06, -0.63, 3.7));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindTexture(GL_TEXTURE_2D, ui_시계);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}


	//glDisable(GL_BLEND);	//투명비활성화

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

//value == 1: 창 내려가기(클릭한 칸은 확장), 0: 원상보구
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
		if (가슴_y > (-3) * 6)					가슴_y -= 3;							 
		if (왼상박_x > (-3) * 4)			왼상박_x -= 3;
		if (왼상박_z < 3 * 5)			왼상박_z += 3;
		if (왼하박_x > (-3) * 42)		왼하박_x -= 3;
		if (왼하박_z < 3 * 3)		왼하박_z += 3;
		if (오른상박_x > (-3) * 10)		오른상박_x -= 3;							 
		if (오른상박_z < 3 * 3)		오른상박_z += 3;							 
		if (오른하박_x > (-3) * 40)		오른하박_x -= 3;						 
		if (오른하박_z > (-3) * 3)		오른하박_z -= 3;	
		if (오른허벅지_z < 3 * (3))	오른허벅지_z += 3;
		if (오른종아리_x < 3 * 3)		오른종아리_x += 3;
		if (오른종아리_z > -3 * 2)		오른종아리_z -= 3;
		if (왼허벅지_x < 3 * 3)		왼허벅지_x += 3;
		if (왼허벅지_z > -3 * 2)		왼허벅지_z -= 3;
		if (왼종아리_x < 3 * 12)	왼종아리_x += 3;
		if (왼종아리_z < 3 * 2)	왼종아리_z += 3;
			
		value++;
		glutTimerFunc(30, TimerAnimation_1, value);
	}
	else if (value < 62) {
		if (가슴_y > (-3) * 3 -18)			가슴_y -= 3;
		if (오른상박_x > (-3) * 20-30)		오른상박_x -= 3;
		if (오른하박_x < 3 * 40-120)		오른하박_x += 6;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else if (value < 82) {
		if (가슴_y < 3 * 3-18-9)			가슴_y += 3;
		if (오른상박_x < 3 * 20-30-3*20)		오른상박_x += 3;
		if (오른하박_x > (-3) * 40)		오른하박_x -= 6;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else if (value < 105) {
		if (가슴_y < (3) * 10 - 18)			가슴_y += 3;
		if (왼상박_x > (-3) * 23 - 12)		왼상박_x -= 3;
		if (왼상박_z > (-3) * 5 + 15)		왼상박_z -= 3;
		if (왼하박_x < 3 * 40 - 126)		왼하박_x += 6;
		if (골반_y < (3) * 5 )			골반_y += 3;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else if (value < 128) {
		if (가슴_y > (-3) * 10 - 18 +3*10)			가슴_y -= 3;
		if (왼상박_x < (3) * 23 - 12 -3*23)		왼상박_x += 3;
		if (왼상박_z < (3) * 5 )			왼상박_z += 3;
		if (왼하박_x > (-3) * 40 - 6)		왼하박_x -= 6;
		if (골반_y > (-3) * 5)			골반_y -= 3;

		value++;
		glutTimerFunc(20, TimerAnimation_1, value);
	}
	else
		animation = 0;
	
	glutPostRedisplay();
	
}

//카메라가 화면의 중심y 기준 공전
void TimerRotate_CAMERA_Y(int value)
{
	float cp;

	//고정값이 들어가야 등속운동한다.
	if (value == 1)
		cp = 3;
	else
		cp = -3;	

	float temp_cx = cameraPos.x, temp_cz = cameraPos.z;
	float temp_lx = light_x, temp_lz = light_z;

	//카메라 회전
	cameraPos.x = temp_cx * glm::cos(glm::radians(cp)) + temp_cz * glm::sin(glm::radians(cp));	//cp만큼 움직(회전)였다는 뜻이므로
	cameraPos.z = temp_cz * glm::cos(glm::radians(cp)) - temp_cx * glm::sin(glm::radians(cp));
	//조명 회전
	light_x = temp_lx * glm::cos(glm::radians(cp)) + temp_lz * glm::sin(glm::radians(cp));	//cp만큼 움직(회전)였다는 뜻이므로
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

//right - 1: 오른쪽클릭, 0: 왼쪽클릭
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

//up - 1: 위쪽클릭, 0: 아래쪽클릭
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
			PlaySound(L"wav/태권V.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);	
		else if (character == 2)
			PlaySound(L"wav/마징가Z.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		else if (character == 3)
			PlaySound(L"wav/트랜스포머.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		else if (character == 4)
			PlaySound(L"wav/아톰.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		else if (character == 5)
			PlaySound(L"wav/터미네이터.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		glutTimerFunc(0, TimerRed, 7);
	}
	else if (*x == 1 and (*y / 10) == 3) {
		if (*y == 31){
			ofstream out{ "Pose.txt" };
			out<< character << ' ';
			out << 목_x << ' '; out << 목_y << ' ';
			out << 가슴_y << ' ';
			out << 왼상박_x << ' '; out << 왼상박_z << ' ';
			out << 왼하박_x << ' '; out << 왼하박_z << ' ';
			out << 왼주먹_x << ' '; out << 왼주먹_y << ' '; out << 왼주먹_z << ' ';
			out << 오른상박_x << ' '; out << 오른상박_z << ' ';
			out << 오른하박_x << ' '; out << 오른하박_z << ' ';
			out << 오른주먹_x << ' '; out << 오른주먹_y << ' '; out << 오른주먹_z << ' ';
			out << 골반_y << ' ';
			out << 왼허벅지_x << ' '; out << 왼허벅지_z << ' ';
			out << 왼종아리_x << ' '; out << 왼종아리_z << ' ';
			out << 왼발_x << ' ';
			out << 오른허벅지_x << ' '; out << 오른허벅지_z << ' ';
			out << 오른종아리_x << ' '; out << 오른종아리_z << ' ';
			out << 오른발_x << ' ';
		}
		else{
			ifstream in{ "Pose.txt" };
			in >> character;
			in >> 목_x; in >> 목_y;
			in >> 가슴_y;
			in >> 왼상박_x ; in >> 왼상박_z;
			in >> 왼하박_x ; in >> 왼하박_z;
			in >> 왼주먹_x ; in >> 왼주먹_y; in >> 왼주먹_z;
			in >> 오른상박_x ;in >> 오른상박_z;
			in >> 오른하박_x; in >> 오른하박_z;
			in >> 오른주먹_x; in >> 오른주먹_y; in >> 오른주먹_z;
			in >> 골반_y ;
			in >> 왼허벅지_x; in >> 왼허벅지_z;
			in >> 왼종아리_x; in >> 왼종아리_z;
			in >> 왼발_x;
			in >> 오른허벅지_x; in >> 오른허벅지_z;
			in >> 오른종아리_x; in >> 오른종아리_z;
			in >> 오른발_x;

			if (character == 1)
				PlaySound(L"wav/태권V.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 2)
				PlaySound(L"wav/마징가Z.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 3)
				PlaySound(L"wav/트랜스포머.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 4)
				PlaySound(L"wav/아톰.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
			else if (character == 5)
				PlaySound(L"wav/터미네이터.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}
		
		click = 1;
		glutTimerFunc(0, TimerRed, 7);
	}
	else if (*x == 1 and (*y / 10) == 4) {
		animation = 1;

		//상반신
		목_x = 0, 목_y = 0,
		가슴_y = 0,
		왼상박_x = 0, 왼상박_z = 0, 오른상박_x = 0, 오른상박_z = 0,
		왼하박_x = 0, 왼하박_z = 0, 오른하박_x = 0, 오른하박_z = 0,
		왼주먹_x = 0, 왼주먹_y = 0, 왼주먹_z = 0, 오른주먹_x = 0, 오른주먹_y = 0, 오른주먹_z = 0,
		//하반신
		골반_y = 0,
		왼허벅지_x = 0, 왼허벅지_z = 0, 오른허벅지_x = 0, 오른허벅지_z = 0,
		왼종아리_x = 0, 왼종아리_z = 0, 오른종아리_x = 0, 오른종아리_z = 0,
		왼발_x = 0, 오른발_x = 0;
		
		
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
				{//상반신 관절 꺽기
					if (ui.y == 2101) 목_x -= 3;
					else if (ui.y == 2102) 목_y -= 3;
					else if (ui.y == 2103) 가슴_y -= 3;
					else if (ui.y == 2104) 왼상박_x -= 3;
					else if (ui.y == 2105) 왼상박_z -= 3;
					else if (ui.y == 2106) 왼하박_x -= 3;
					else if (ui.y == 2107) 왼하박_z -= 3;
					else if (ui.y == 2108) 왼주먹_x -= 3;
					else if (ui.y == 2109) 왼주먹_y -= 3;
					else if (ui.y == 2110) 왼주먹_z -= 3;
					else if (ui.y == 2111) 오른상박_x -= 3;
					else if (ui.y == 2112) 오른상박_z -= 3;
					else if (ui.y == 2113) 오른하박_x -= 3;
					else if (ui.y == 2114) 오른하박_z -= 3;
					else if (ui.y == 2115) 오른주먹_x -= 3;
					else if (ui.y == 2116) 오른주먹_y -= 3;
					else if (ui.y == 2117) 오른주먹_z -= 3;
				}
				{//하반신 관절 꺽기
					if (ui.y == 2201) 골반_y -= 3;
					else if (ui.y == 2202) 왼허벅지_x -= 3;
					else if (ui.y == 2203) 왼허벅지_z -= 3;
					else if (ui.y == 2204) 왼종아리_x -= 3;
					else if (ui.y == 2205) 왼종아리_z -= 3;
					else if (ui.y == 2206) 왼발_x -= 3;
					else if (ui.y == 2207) 오른허벅지_x -= 3;
					else if (ui.y == 2208) 오른허벅지_z -= 3;
					else if (ui.y == 2209) 오른종아리_x -= 3;
					else if (ui.y == 2210) 오른종아리_z -= 3;
					else if (ui.y == 2211) 오른발_x -= 3;

				}
			}
			break;
		case '-':
			if (character != 0) {
				{//상반신 관절 꺽기
					if (ui.y == 2101) 목_x += 3;
					else if (ui.y == 2102) 목_y += 3;
					else if (ui.y == 2103) 가슴_y += 3;
					else if (ui.y == 2104) 왼상박_x += 3;
					else if (ui.y == 2105) 왼상박_z += 3;
					else if (ui.y == 2106) 왼하박_x += 3;
					else if (ui.y == 2107) 왼하박_z += 3;
					else if (ui.y == 2108) 왼주먹_x += 3;
					else if (ui.y == 2109) 왼주먹_y += 3;
					else if (ui.y == 2110) 왼주먹_z += 3;
					else if (ui.y == 2111) 오른상박_x += 3;
					else if (ui.y == 2112) 오른상박_z += 3;
					else if (ui.y == 2113) 오른하박_x += 3;
					else if (ui.y == 2114) 오른하박_z += 3;
					else if (ui.y == 2115) 오른주먹_x += 3;
					else if (ui.y == 2116) 오른주먹_y += 3;
					else if (ui.y == 2117) 오른주먹_z += 3;
				}
				{//하반신 관절 꺽기
					if (ui.y == 2201) 골반_y += 3;
					else if (ui.y == 2202) 왼허벅지_x += 3;
					else if (ui.y == 2203) 왼허벅지_z += 3;
					else if (ui.y == 2204) 왼종아리_x += 3;
					else if (ui.y == 2205) 왼종아리_z += 3;
					else if (ui.y == 2206) 왼발_x += 3;
					else if (ui.y == 2207) 오른허벅지_x += 3;
					else if (ui.y == 2208) 오른허벅지_z += 3;
					else if (ui.y == 2209) 오른종아리_x += 3;
					else if (ui.y == 2210) 오른종아리_z += 3;
					else if (ui.y == 2211) 오른발_x += 3;

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
			//상반신
			목_x = 0, 목_y = 0,
			가슴_y = 0,
			왼상박_x = 0, 왼상박_z = 0, 오른상박_x = 0, 오른상박_z = 0,
			왼하박_x = 0, 왼하박_z = 0, 오른하박_x = 0, 오른하박_z = 0,
			왼주먹_x = 0, 왼주먹_y = 0, 왼주먹_z = 0, 오른주먹_x = 0, 오른주먹_y = 0, 오른주먹_z = 0,
			//하반신
			골반_y = 0,
			왼허벅지_x = 0, 왼허벅지_z = 0, 오른허벅지_x = 0, 오른허벅지_z = 0,
			왼종아리_x = 0, 왼종아리_z = 0, 오른종아리_x = 0, 오른종아리_z = 0,
			왼발_x = 0, 오른발_x = 0;
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