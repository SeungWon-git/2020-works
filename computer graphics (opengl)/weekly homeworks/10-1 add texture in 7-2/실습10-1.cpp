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

//주소
#define hexahedronpoints	36	//-> 총 점의 갯수 = 삼각형(폴리곤) 갯수 * 3(꼭짓점)
#define pyramidpoints		18

GLfloat hexahedron_s[hexahedronpoints][3] = {};
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat linex_s[2][3] = { {-1.0,0.0,0.0},{1.0,0.0,0.0} };
GLfloat liney_s[2][3] = { {0.0,-1.0,0.0},{0.0,1.0,0.0} };
GLfloat linez_s[2][3] = { {0.0,0.0,-1.0},{0.0,0.0,1.0} };

//법선
GLfloat hexa_vn[hexahedronpoints][3] = {};
GLfloat pyramid_vn[pyramidpoints][3] = {};
GLfloat linex_vn[2][3] = { {0.0,0.0,0.0},{0.0,0.0,1.0} };
GLfloat liney_vn[2][3] = { {0.0,0.0,0.0},{1.0,0.0,1.0} };
GLfloat linez_vn[2][3] = { {0.0,0.0,0.0},{1.0,0.0,0.0} };

//텍스처 좌표
GLfloat hexa_vt[hexahedronpoints][2];
GLfloat pyramid_vt[pyramidpoints][2];
GLfloat line_vt[2][2] = { {0.0,0.0},{1.0,1.0} };

//vao,vbo
GLuint 무대_vao, hexahedron_vbo[3];
GLuint pyramid_vao, pyramid_vbo[3];
GLuint linex_vao, linex_vbo[3], liney_vao, liney_vbo[3], linez_vao, linez_vbo[3];

//전역 변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);			//카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);	//카메라가 바라보는 기준점
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//카메라의 상단이 가리키는 방향

bool perspective = 1;
bool pyramid = 0;
bool stop_rup = 1, stop_rside = 1, stop_pside = 1;
bool stop_animation = 1;

GLfloat move_x = 0, move_y = -0.1f, move_z = -0.5f;	//이동

GLclampf change_size = 2.3f; //크기배율

GLclampf c_s = 0.01f;	//수축/팽창 전환

GLfloat RUP = 0.0f;		//육면체 윗면 각도
GLclampf rside = 0.0f;	//육면체 방향전환
GLfloat RSIDE = 0.0f;	//육면체 옆면 각도
GLclampf pside = 0.0f;	//사각뿔 방향전환
GLfloat PSIDE = 0.0f;	//사각뿔 옆면 각도

GLclampf r_x = 0.0f;	//자전 방향전환
GLfloat RX = 0.0f;		//자전 각도
GLclampf r_y = 0.0f;	//자전 방향전환
GLfloat RY = 0.0f;		//자전 각도

// 이미지
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

void InitModel() {
	// obj읽기
	char filename[30] = "정육면체.obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexahedron_s, hexa_vn, hexa_vt);

	strcpy(filename, "사각뿔(피라미드).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
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

// 노말값을 주긴하지만 프래그먼트에서 사용X (조명 사용시에 사용)
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
	InitVao(무대_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexa_vn,hexa_vt);

	InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_vn, pyramid_vt);

	InitVao(linex_vao, linex_vbo, 2, linex_s, linex_vn, line_vt);
	InitVao(liney_vao, liney_vbo, 2, liney_s, liney_vn, line_vt);
	InitVao(linez_vao, linez_vbo, 2, linez_s, linez_vn, line_vt);
}

void InitTexture() {
	BITMAPINFO* bmp;
	int widthImage, heightImage, numberOfChannel;

	//A
	glGenTextures(1, &texture_a); //--- 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, texture_a); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	unsigned char* data = stbi_load("a.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
	//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
	//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)
	
	//B
	glGenTextures(1, &texture_b); //--- 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, texture_b); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	data = stbi_load("b.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
	//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
	//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)

	//C
	glGenTextures(1, &texture_c); //--- 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, texture_c); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	data = stbi_load("c.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
	//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
	//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)

	//D
	glGenTextures(1, &texture_d); //--- 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, texture_d); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	data = stbi_load("d.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
	//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
	//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)

	//E
	glGenTextures(1, &texture_e); //--- 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, texture_e); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	data = stbi_load("e.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
	//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
	//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)

	//F
	glGenTextures(1, &texture_f); //--- 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, texture_f); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	data = stbi_load("f.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
	//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
	//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)

	//Line
	glGenTextures(1, &texture_line); //--- 텍스처 생성
	glBindTexture(GL_TEXTURE_2D, texture_line); //--- 텍스처 바인딩
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //--- 현재 바인딩된 텍스처의 파라미터 설정하기
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); //--- 이미지가 거꾸로 읽힌다면 추가
	data = stbi_load("black_line.png", &widthImage, &heightImage, &numberOfChannel, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, widthImage, heightImage, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //---텍스처 이미지 정의 
	//(3번째 인자값 - 이미지 파일의 비트 수준이 24이면 '3'으로 하고 7번째 인자에 'GL_RGB'를 준다.)
	//(3번째 인자값 - 이미지 파일의 비트 수준이 32이면 '4'으로 하고 7번째 인자에 'GL_RGBA'를 준다.)

	glUseProgram(s_program);
	int tLocation = glGetUniformLocation(s_program, "outTexture"); //--- outTexture 유니폼 샘플러의 위치를 가져옴
	glUniform1i(tLocation, 0); //--- 샘플러를 0번 유닛으로 설정

	stbi_image_free(data);	//---메모리 반환
}



GLvoid drawScene()
{
	glClearColor(1.0, 1.0, 1.0, 0.0f);
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

	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
	mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	vTransform = glm::mat4(1.0f);	// 카메라 변환
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLoc = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	pTransform = glm::mat4(1.0f);	// 투영 변환
	if (perspective == 1)	//원근 투영
		pTransform = glm::perspective(glm::radians(45.0f), window_w / window_h, 0.1f, 50.0f);
	else					//직각 투영
		pTransform = glm::ortho(-2.0, 2.0, -2.0, 2.0, 0.1, 50.0);
	projLoc = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_line);

	//x,y,z축
	glBindVertexArray(linex_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(liney_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	glBindVertexArray(linez_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	//도형
	if (pyramid == 0) {	//육면체
		glBindVertexArray(무대_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture_c);
		glDrawArrays(GL_TRIANGLES, 0, 6);	//0번~6번까지 -뒤

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_d);
		glDrawArrays(GL_TRIANGLES, 6, 6);	//6번~11번까지 -왼쪽

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_b);
		glDrawArrays(GL_TRIANGLES, 18, 6);	//18번~23번까지 -오른쪽

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_e);
		glDrawArrays(GL_TRIANGLES, 24, 6);	//24번~29번까지 -아래

		{//윗면 회전 애니메이션 (중요- 회전은 무조건 x,y,z를 중심으로밖에 못하므로 로컬값을 변경(원하는 축에 맞춤)해줘서 회전시킨후 다시 원위치를 해줘야한다.)
			for (int i = 12; i < 18; ++i)	//해당 인덱스의 로컬값 수정(y값을 0으로-> x축에 회전할것이기 때문에)
				hexahedron_s[i][1] -= 0.25;

			InitVao(무대_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexa_vn, hexa_vt);	//vao 업데이트

			mTransform = glm::translate(mTransform, glm::vec3(0.0, 0.25, 0.0));	//모델전체 위로 이동(원점도 같이 이동하게됨 - 제자리에 위치하는 역활)
			mTransform = glm::rotate(mTransform, glm::radians(RUP), glm::vec3(1.0, 0.0, 0.0));	//이제 x축에대해 회전
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_f);
			glDrawArrays(GL_TRIANGLES, 12, 6);	//12번~17번까지 -위

			for (int i = 12; i < 18; ++i)	//해당 인덱스의 로컬값 원상복귀 - 혹시 후에 이 좌표를 그냥 쓸수도 있기 때문에
				hexahedron_s[i][1] += 0.25;
		}

		{//옆면 회전 애니메이션
			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] += 0.25; hexahedron_s[i][2] -= 0.25;
			}

			InitVao(무대_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexa_vn, hexa_vt);

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
			glDrawArrays(GL_TRIANGLES, 30, 6);	//30번~35번까지 -앞

			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] -= 0.25; hexahedron_s[i][2] += 0.25;
			}
		}
	}
	else {	//사각뿔(피라미드)
		glBindVertexArray(pyramid_vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_e);
		glDrawArrays(GL_TRIANGLES, 9, 6);	//9번~14번까지

		{//옆면 회전 애니메이션
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
			glDrawArrays(GL_TRIANGLES, 0, 3);	//0번~2번까지 -뒤

			for (int i = 0; i < 3; ++i) {
				pyramid_s[i][1] -= 0.15; pyramid_s[i][2] -= 0.25;
			}
		}
		{//옆면 회전 애니메이션
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
			glDrawArrays(GL_TRIANGLES, 3, 3);	//3번~5번까지 -왼

			for (int i = 3; i < 6; ++i) {
				pyramid_s[i][0] -= 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//옆면 회전 애니메이션
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
			glDrawArrays(GL_TRIANGLES, 6, 3);	//6번~8번까지 -오른

			for (int i = 6; i < 9; ++i) {
				pyramid_s[i][0] += 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//옆면 회전 애니메이션
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
			glDrawArrays(GL_TRIANGLES, 15, 3);	//15번~17번까지 -앞

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
	case 'C':	//초기 설정값으로
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