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

//주소(모양)
#define hexahedronpoints	36	//-> 총 점의 갯수 = 삼각형(폴리곤) 갯수 * 3(꼭짓점)
#define pyramidpoints		18
#define spherepoints		240

GLfloat hexa_s[hexahedronpoints][3] = {};
GLfloat floor_s[12][3] = {
	{ -0.25,-0.25,-0.25 }, { 0.0,-0.25,0.0 }, {0.25,-0.25,-0.25},	//북
	{ 0.25,-0.25,-0.25 }, { 0.0,-0.25,0.0 }, {0.25,-0.25,0.25},		//동
	{ 0,-0.25,0.0 }, { -0.25,-0.25,0.25 }, {0.25,-0.25,0.25},		//남
	{-0.25,-0.25,-0.25},{-0.25,-0.25,0.25},{0,-0.25,0}				//서
};
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat sphere_s[spherepoints][3] = {};



//색상
GLfloat 무대_c[hexahedronpoints][3] = {};
GLfloat floor_c[12][3] = {
	{0.5,0.75,0.27},{0.5,0.75,0.27},{0.5,0.75,0.27},	//뒤-연두
	{1,0.5,0},{1,0.5,0},{1,0.5,0},						//오른-주황
	{0.31,0.73,0.87},{0.31,0.73,0.87},{0.31,0.73,0.87},	//앞-하늘
	{1,0.2,0.6},{1,0.2,0.6},{1,0.2,0.6}					//왼-핑크
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
무대_vao, floor_vao, 
robot_h_vao, robot_b_vao, robot_Larm_vao, robot_Rarm_vao, robot_Lleg_vao, robot_Rleg_vao, robot_nose_vao,
hexa_brown_vao, hexa_red_vao, hexa_green_vao, hexa_blue_vao, hexa_white_vao, hexa_black_vao,
sphere_green_vao, sphere_yellow_vao, sphere_black_vao,
철봉로봇_왼팔_vao, 철봉로봇_오른팔_vao, 철봉로봇_몸통_vao,
벤치프레스_팔_vao, 벤치프레스_오상박_vao,
러닝머신_다리_vao,
//vbo
무대_vbo[2], floor_vbo[2], 
robot_h_vbo[2], robot_b_vbo[2], robot_Larm_vbo[2], robot_Rarm_vbo[2], robot_Lleg_vbo[2], robot_Rleg_vbo[2], robot_nose_vbo[2], 
hexa_brown_vbo[2], hexa_red_vbo[2], hexa_green_vbo[2], hexa_blue_vbo[2], hexa_white_vbo[2], hexa_black_vbo[2],
sphere_green_vbo[2], sphere_yellow_vbo[2], sphere_black_vbo[2],
철봉로봇_왼팔_vbo[2], 철봉로봇_오른팔_vbo[2], 철봉로봇_몸통_vbo[2],
벤치프레스_팔_vbo[2], 벤치프레스_오상박_vbo[2],
러닝머신_다리_vbo[2]
;

//전역 변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 2.3f);			//카메라 위치 
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);	//카메라가 바라보는 기준점
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//카메라의 상단이 가리키는 방향

bool perspective = 1;
bool stop_animation = 1;
bool stop_objs = 1;

GLfloat frontscreen_y = 0, frontscreen_r = 0;	//앞 장막 걷기

GLfloat move_x = 0, move_y = -0.17f, move_z = 0.5f;	//모델 이동
GLfloat direction = 360.0f;//모델이 바라보는 방향[각도] ( 0 -> 앞, -90 -> 왼, 90 -> 오, 180 -> 뒤 )
GLfloat walk = 0.0f;	//모델 걷기 각도

GLfloat tree_leaf_size = 0.2f;	//나뭇잎 크기
bool tree_leaf_size_up = 1;		//나뭇잎 크기 증가OX

GLfloat 철봉각도 = 0.0f;

GLfloat 벤치프레스각도 = 90.0f;
bool 벤치프레스각도증가 = 0;		//벤치프레스 각도 증가OX
GLfloat 벤치프레스오른하박움직임_x = -0.1, 벤치프레스오른하박움직임_y = 0, 벤치프레스왼하박움직임_x = 0.1, 벤치프레스왼하박움직임_y = 0;

GLfloat 러닝머신각도 = 0.0f;
GLfloat 러닝머신로봇다리각도 = 0.0f;
bool 러닝머신로봇다리각도증가 = 0;

GLfloat cp_cw = 0.0f;		//카메라의 방향
GLfloat CP_Y = 0.0f;		//카메라의 각도
GLfloat cd_cw = 0.0f;		//카메라가 바라보는 방향
GLfloat CD_Y = 270.0f;		//카메라가 바라보는 각도




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
	rewind(objFile);	// 읽기 위치 다시 맨 앞으로

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
			fscanf(objFile, "%f %f %f", &vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			//확인 -//std::cout << "v["<< vertIndex <<"] - "<<vertex[vertIndex].x << ", " << vertex[vertIndex].y << ", " << vertex[vertIndex].z << std::endl;
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%f%s%f%s%f", &face[faceIndex].x, &count, &face[faceIndex].y, &count, &face[faceIndex].z);
			//확인 -//std::cout  << "f[" << faceIndex << "] - "<<face[faceIndex].x << ", " << face[faceIndex].y << ", " << face[faceIndex].z << std::endl;
			faceIndex++;
		}
	}

	//--- 4. obj 버텍스 주소에 저장
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

	//--- 5.메모리 반환
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

void InitVao(GLuint& vao, GLuint vbo[2], int num, GLfloat Shape[][3], GLfloat colors[][3]) {
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기

	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// 변수 triShape 에서 버텍스 데이터 값을 버퍼에 복사한다.
	// Shape 배열의 사이즈: num * 3 *float
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), Shape, GL_STATIC_DRAW);
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: num * 3 *float
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	// 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void InitModel(){
	// [obj읽기]
	char filename[30] = "정육면체.obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexa_s);

	strcpy(filename, "사각뿔(피라미드).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, pyramid_s);

	strcpy(filename, "구(2).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, sphere_s);

	fclose(file);	//파일닫기

	// [색 초기화]
	//무대
	for (int i = 0; i < hexahedronpoints; ++i) {
		if (i < 6) {	//뒷면-약한 회색
			무대_c[i][0] = 0.9f; 무대_c[i][1] = 0.9f; 무대_c[i][2] = 0.9f;
		}
		else if (i < 12) {	//왼쪽면-회색
			무대_c[i][0] = 0.7f; 무대_c[i][1] = 0.7f; 무대_c[i][2] = 0.7f;
		}
		else if (i < 18) {	//윗면-흰색
			무대_c[i][0] = 1.0f; 무대_c[i][1] = 1.0f; 무대_c[i][2] = 1.0f;
		}
		else if (i < 24) {	//오른쪽면-회색
			무대_c[i][0] = 0.7f; 무대_c[i][1] = 0.7f; 무대_c[i][2] = 0.7f;
		}
		else if (i < 30) {	//아랫면-흰색(인데 사용안함)
			무대_c[i][0] = 1.0f; 무대_c[i][1] = 1.0f; 무대_c[i][2] = 1.0f;
		}
		else if (i < 36) {	//앞면-약한 회색
			무대_c[i][0] = 0.9f; 무대_c[i][1] = 0.9f; 무대_c[i][2] = 0.9f;
		}
	}

	//로봇
	for (int i = 0; i < pyramidpoints; ++i) {
		//로봇 코
		robot_nose_c[i][0] = 0.58; robot_nose_c[i][1] = 0.29; robot_nose_c[i][2] = 0.0;	//갈색
	}
	for (int i = 0; i < hexahedronpoints; ++i) {
		//로봇 몸체
		robot_h_c[i][0] = 0.9; robot_h_c[i][1] = 0.8; robot_h_c[i][2] = 0.69;	//머리-살구색
		robot_b_c[i][0] = 1.0; robot_b_c[i][1] = 0.0; robot_b_c[i][2] = 0.0;	//몸통-빨강색
		robot_Rarm_c[i][0] = 1.0; robot_Rarm_c[i][1] = 0.0; robot_Rarm_c[i][2] = 1.0;	//오른팔-보라색
		robot_Larm_c[i][0] = 0.4; robot_Larm_c[i][1] = 0.0; robot_Larm_c[i][2] = 0.6;	//왼팔-자주색
		robot_Rleg_c[i][0] = 0.0; robot_Rleg_c[i][1] = 0.0; robot_Rleg_c[i][2] = 1.0;	//오른다리-파란색
		robot_Lleg_c[i][0] = 0.19; robot_Lleg_c[i][1] = 0.77; robot_Lleg_c[i][2] = 0.65;	//왼다리-옥색
	}

	//객체들
	//육각형
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_brown_c[i][0] = 0.65; hexa_brown_c[i][1] = 0.4; hexa_brown_c[i][2] = 0.0;	//갈색-나무 줄기
		hexa_red_c[i][0] = 1; hexa_red_c[i][1] = 0; hexa_red_c[i][2] = 0;	//빨강색
		hexa_green_c[i][0] = 0; hexa_green_c[i][1] = 1; hexa_green_c[i][2] = 0;	//초록색
		hexa_blue_c[i][0] = 0; hexa_blue_c[i][1] = 0; hexa_blue_c[i][2] = 1;	//파란색
		hexa_white_c[i][0] = 1; hexa_white_c[i][1] = 1; hexa_white_c[i][2] = 1;	//하얀색
	}
	//구
	for (int i = 0; i < spherepoints; ++i) {
		sphere_green_c[i][0] = 0.0; sphere_green_c[i][1] = 1.0; sphere_green_c[i][2] = 0.0;		//초록-나무 잎
		sphere_yellow_c[i][0] = 1; sphere_yellow_c[i][1] = 1.0; sphere_yellow_c[i][2] = 0.0;	//노란색-역기,철봉
	}



	// [모델 위치 조정 & InitVao]
	InitVao(무대_vao, 무대_vbo, hexahedronpoints, hexa_s, 무대_c);					//무대
	InitVao(floor_vao, floor_vbo, 12, floor_s, floor_c);							//무대 바닥
	InitVao(robot_h_vao, robot_h_vbo, hexahedronpoints, hexa_s, robot_h_c);			//로봇
	InitVao(hexa_brown_vao, hexa_brown_vbo, hexahedronpoints, hexa_s, hexa_brown_c);//갈색 사각형
	InitVao(hexa_red_vao, hexa_red_vbo, hexahedronpoints, hexa_s, hexa_red_c);//빨간색 사각형
	InitVao(hexa_green_vao, hexa_green_vbo, hexahedronpoints, hexa_s, hexa_green_c);//초록색 사각형
	InitVao(hexa_blue_vao, hexa_blue_vbo, hexahedronpoints, hexa_s, hexa_blue_c);//파란색 사각형
	InitVao(hexa_white_vao, hexa_white_vbo, hexahedronpoints, hexa_s, hexa_white_c);//하얀색 사각형
	InitVao(hexa_black_vao, hexa_black_vbo, hexahedronpoints, hexa_s, 0);//검은색 사각형
	InitVao(sphere_green_vao, sphere_green_vbo, spherepoints, sphere_s, sphere_green_c);	//초록 원
	InitVao(sphere_yellow_vao, sphere_yellow_vbo, spherepoints, sphere_s, sphere_yellow_c);	//노랑 원
	InitVao(sphere_black_vao, sphere_black_vbo, spherepoints, sphere_s, 0);	//검은 원

	//철봉 로봇
	//왼팔
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.25; hexa_s[i][2] -= 0.5;
	}
	InitVao(철봉로봇_왼팔_vao, 철봉로봇_왼팔_vbo, hexahedronpoints, hexa_s, hexa_blue_c);
	//오른팔
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][2] += 1; 
	}
	InitVao(철봉로봇_오른팔_vao, 철봉로봇_오른팔_vbo, hexahedronpoints, hexa_s, hexa_blue_c);
	//몸통
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.25; hexa_s[i][2] -= 0.5;
	}
	InitVao(철봉로봇_몸통_vao, 철봉로봇_몸통_vbo, hexahedronpoints, hexa_s, hexa_blue_c);

	for (int i = 0; i < hexahedronpoints; ++i) {	//원위치
		hexa_s[i][1] += 0.5;
	}
	//std::cout << hexa_s[0][0] << "," << hexa_s[0][1] << "," << hexa_s[0][2] << std::endl;	//확인

	//벤치 프레스
	//팔
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] += 0.25;
	}
	InitVao(벤치프레스_팔_vao, 벤치프레스_팔_vbo, hexahedronpoints, hexa_s, hexa_blue_c);

	for (int i = 0; i < hexahedronpoints; ++i) {	//원위치
		hexa_s[i][1] -= 0.25;
	}

	//벤치 프레스
	//다리
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.25;
	}
	InitVao(러닝머신_다리_vao, 러닝머신_다리_vbo, hexahedronpoints, hexa_s, hexa_red_c);

	for (int i = 0; i < hexahedronpoints; ++i) {	//원위치
		hexa_s[i][1] += 0.25;
	}

	//상하좌우로 움직이는 로봇
	//코
	for (int i = 0; i < pyramidpoints; ++i) {
		pyramid_s[i][1] += 0.25;
	}
	InitVao(robot_nose_vao, robot_nose_vbo, pyramidpoints, pyramid_s, robot_nose_c);

	//몸통
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][1] -= 0.38;
	}
	InitVao(robot_b_vao, robot_b_vbo, hexahedronpoints, hexa_s, robot_b_c);

	//왼팔-자주색
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][0] -= 1.5; hexa_s[i][1] -= 0.21;
	}
	InitVao(robot_Larm_vao, robot_Larm_vbo, hexahedronpoints, hexa_s, robot_Larm_c);

	//오른팔-보라색
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][0] += 3.0;
	}
	InitVao(robot_Rarm_vao, robot_Rarm_vbo, hexahedronpoints, hexa_s, robot_Rarm_c);

	//왼다리-옥색
	for (int i = 0; i < hexahedronpoints; ++i) {
		hexa_s[i][0] -= 1.5; hexa_s[i][0] += 0.7;
	}
	InitVao(robot_Lleg_vao, robot_Lleg_vbo, hexahedronpoints, hexa_s, robot_Lleg_c);

	//오른다리-파란색
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

	glEnable(GL_DEPTH_TEST);	// 은면 제거
	glFrontFace(GL_CW);			// 시계방향이 앞면
	glEnable(GL_CULL_FACE);		// 뒷면(시계반대방향으로 winding이 앞면) 제거



	unsigned int modelLoc = glGetUniformLocation(s_program, "model");	//--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	unsigned int viewLoc = glGetUniformLocation(s_program, "view");		//--- 버텍스 세이더에서 뷰잉 변환 행렬 변수값을 받아온다.
	unsigned int projLoc = glGetUniformLocation(s_program, "projection");//--- 버텍스 세이더에서 투영 변환 행렬 변수값을 받아온다.
	glm::mat4 mTransform = glm::mat4(1.0f);		  // 단위행렬로 초기화
	glm::mat4 vTransform = glm::mat4(1.0f);		  // 단위행렬로 초기화
	glm::mat4 pTransform = glm::mat4(1.0f);		  // 단위행렬로 초기화


	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::scale(mTransform, glm::vec3(3.5, 1.5, 3.5));
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

	//배경
	{
		{//무대
			glBindVertexArray(무대_vao);
			glDrawArrays(GL_TRIANGLES, 0, 24);
			glDrawArrays(GL_TRIANGLES, 30, 6);	//바닥면 안 그리기
		}
		{//바닥면
			glFrontFace(GL_CCW);			// 반시계방향이 앞면(원위치)

			glBindVertexArray(floor_vao);
			glDrawArrays(GL_TRIANGLES, 0, 12);
		}
		{//앞면
			mTransform = glm::rotate(mTransform, glm::radians(frontscreen_r), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::translate(mTransform, glm::vec3(0, frontscreen_y, 0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			if (frontscreen_r > -90) {
				glBindVertexArray(무대_vao);
				glDrawArrays(GL_TRIANGLES, 30, 6);
			}
		}
		glDisable(GL_CULL_FACE);		// 뒷면(시계반대방향으로 winding이 앞면) 제거 해제
	}

	//객체
	{
		//나무
		{
			{//나무 기둥
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(0.6, -0.15, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.4, 1.0, 0.4));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_brown_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//나무 잎
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.15, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(tree_leaf_size, tree_leaf_size, tree_leaf_size));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_green_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
		}
		//가운데 기둥
		{
			{//맨 밑기둥
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.17, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.5, 0.8, 0.5));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_black_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//1번째 지붕
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(0, 0, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.8, 0.35, 0.8));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_red_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//2번째 지붕
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(0, 0.14, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.6, 0.3, 0.6));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_green_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//3번째 지붕
				mTransform = glm::mat4(1.0f);	//모델 변환
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
		//철봉
		{
			{//철봉 로봇
				{//왼팔
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.0, -0.6));
					mTransform = glm::rotate(mTransform, glm::radians(철봉각도), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(철봉로봇_왼팔_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//오른팔
					glBindVertexArray(철봉로봇_오른팔_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//몸통
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.0, -0.6));
					mTransform = glm::rotate(mTransform, glm::radians(철봉각도), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(0.2, 0.4, 0.2));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(철봉로봇_몸통_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
			}
			{//봉(손잡이)
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(0.6, 0.0, -0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.1, 0.6));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//지지대2개
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(0.6, -0.2, -0.72));
				mTransform = glm::scale(mTransform, glm::vec3(0.07, 0.8, 0.07));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
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
		//벤치 프레스
		{
			{
				{//벤치
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.3, -0.6));
					mTransform = glm::scale(mTransform, glm::vec3(0.6, 0.1, 0.8));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{//머리
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.25, -0.76));
					mTransform = glm::scale(mTransform, glm::vec3(0.15, 0.15, 0.15));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_red_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//몸통
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.25, -0.6));
					mTransform = glm::scale(mTransform, glm::vec3(0.25, 0.1, 0.5));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_green_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{//양허벅지
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.64, -0.25, -0.4));
					mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.1, 0.3));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_blue_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.56, -0.25, -0.4));
					mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.1, 0.3));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_blue_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
				{//양다리
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.64, -0.3, -0.35));
				mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_blue_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.56, -0.3, -0.35));
				mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_blue_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				}
			}
			//움직이는 부분
			{//오른상박
			mTransform = glm::mat4(1.0f);	//모델 변환
			mTransform = glm::translate(mTransform, glm::vec3(-0.66, -0.25, -0.66));
			mTransform = glm::rotate(mTransform, glm::radians(벤치프레스각도), glm::vec3(0.0, 0.0, 1.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(벤치프레스_팔_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//오른하박
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.66, -0.27, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(벤치프레스오른하박움직임_x, 벤치프레스오른하박움직임_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(벤치프레스_팔_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//왼상박
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.54, -0.25, -0.66));
				mTransform = glm::rotate(mTransform, glm::radians(-벤치프레스각도), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(벤치프레스_팔_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//왼하박
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.54, -0.27, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(벤치프레스왼하박움직임_x, 벤치프레스왼하박움직임_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.2, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(벤치프레스_팔_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//역기봉
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.6, -0.15, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(0, 벤치프레스왼하박움직임_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.9, 0.08, 0.08));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
			{//오른쪽원판
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.83, -0.15, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(0, 벤치프레스왼하박움직임_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.15, 0.15));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_yellow_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
			{//왼쪽원판
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.37, -0.15, -0.66));
				mTransform = glm::translate(mTransform, glm::vec3(0, 벤치프레스왼하박움직임_y, 0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.15, 0.15));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_yellow_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
		}
		//러닝머신
		{
			{
				{//러닝머신 바닥
					mTransform = glm::mat4(1.0f);	//모델 변환
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
				{//러닝머신 기둥2개
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.8, -0.19, 0.75));
					mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.8, -0.19, 0.45));
					mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
				}
				{//러닝머신 계기판
					mTransform = glm::mat4(1.0f);	//모델 변환
					mTransform = glm::translate(mTransform, glm::vec3(-0.76, -0.12, 0.6));
					mTransform = glm::rotate(mTransform, glm::radians(-15.0f), glm::vec3(0.0, 0.0, 1.0));
					mTransform = glm::scale(mTransform, glm::vec3(0.3, 0.1, 0.8));
					modelLoc = glGetUniformLocation(s_program, "model");
					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

					glBindVertexArray(hexa_black_vao);
					glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
					glBindVertexArray(hexa_white_vao);
					glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

					mTransform = glm::mat4(1.0f);	//모델 변환
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
			//레일들
			{
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.75, -0.312, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신각도), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.68, -0.318, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신각도), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.61, -0.324, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신각도), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.54, -0.33, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신각도), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.47, -0.336, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.06, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신각도), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.40, -0.342, 0.6));
				mTransform = glm::rotate(mTransform, glm::radians(-5.0f), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.08, 0.20));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신각도), glm::vec3(0.0, 0.0, 1.0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(sphere_black_vao);
				glDrawArrays(GL_TRIANGLES, 0, spherepoints);
			}
			//뛰는 로봇
			{//다리
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.65));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신로봇다리각도), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(러닝머신_다리_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.65));
				mTransform = glm::rotate(mTransform, glm::radians(러닝머신로봇다리각도), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.55));
				mTransform = glm::rotate(mTransform, glm::radians(-러닝머신로봇다리각도), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(러닝머신_다리_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.11, 0.55));
				mTransform = glm::rotate(mTransform, glm::radians(-러닝머신로봇다리각도), glm::vec3(0.0, 0.0, 1.0));
				mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.3, 0.1));
				mTransform = glm::translate(mTransform, glm::vec3(0, -0.25, 0));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//몸통
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, -0.07, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.15, 0.2, 0.4));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_red_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
				glBindVertexArray(hexa_white_vao);
				glDrawArrays(GL_LINE_STRIP, 0, hexahedronpoints);
			}
			{//머리
				mTransform = glm::mat4(1.0f);	//모델 변환
				mTransform = glm::translate(mTransform, glm::vec3(-0.58, 0.03, 0.6));
				mTransform = glm::scale(mTransform, glm::vec3(0.15, 0.2, 0.4));
				modelLoc = glGetUniformLocation(s_program, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

				glBindVertexArray(hexa_blue_vao);
				glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
			}
		}
	}

	//로봇
	{
		{	//머리
			mTransform = glm::mat4(1.0f);	//모델 변환
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.1, 0.1));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_h_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
		}
		{	//코
			mTransform = glm::mat4(1.0f);	//모델 변환
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.1, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_nose_vao);
			glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);
		}
		{	//몸통
			mTransform = glm::mat4(1.0f);	//모델 변환
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.25, 0.2, 0.25));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_b_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
		}
		{	//왼팔
			mTransform = glm::mat4(1.0f);	//모델 변환
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(-walk), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_Larm_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

			//왼다리
			mTransform = glm::mat4(1.0f);	//모델 변환
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
		{	//오른팔
			mTransform = glm::mat4(1.0f);	//모델 변환
			mTransform = glm::translate(mTransform, glm::vec3(0, 0.02, 0));
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(direction), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(walk), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.18, 0.05));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(robot_Rarm_vao);
			glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

			//오른다리
			mTransform = glm::mat4(1.0f);	//모델 변환
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

void TimerOpen_Screen(int value)	//장막열기
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
				//종료&객체 움직이기 시작
				if (stop_objs == 1) {
					stop_objs = 0;
					glutTimerFunc(0, Timer_Objmoving, 0);
				}
			}
		}
	}
}

void Timer_Move_ahead(int value) {	//로봇 앞으로 이동
	if (stop_animation == 0) {
		if (direction == 0) {//앞
			move_z += 0.01;

			if (move_z > 0.82)
				direction = 180;
			if(move_z <= 0 && move_z >= -0.18 && move_x <= 0.18 && move_x >= -0.18)
				direction = 180;
		}
		else if (direction == 180) {	//뒤
			move_z -= 0.01;

			if (move_z < -0.82)
				direction = 0;
			if (move_z >= 0 && move_z <= 0.18 && move_x <= 0.18 && move_x >= -0.18)
				direction = 0;
		}
		else if (direction == -90) {	//왼
			move_x -= 0.01;

			if (move_x < -0.82)
				direction = 90;
			if (move_x >= 0 && move_x <= 0.18 && move_z <= 0.18 && move_z >= -0.18)
				direction = 90;
		}
		else if (direction == 90) {	//오
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

void Timer_Walk(int value) {	//로봇 팔다리 걷기 애니메이션
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

void TimerRotate_CAMERA_Y(int value)	//카메라가 화면의 중심y 기준 공전
{
	if (stop_animation == 0) {

		CP_Y = cp_cw;	//고정값이 들어가야 등속운동한다.

		float temp_x = cameraPos.x, temp_z = cameraPos.z;

		cameraPos.x = temp_x * glm::cos(glm::radians(CP_Y)) + temp_z * glm::sin(glm::radians(CP_Y));	//cw만큼 움직(회전)였다는 뜻이므로
		cameraPos.z = temp_z * glm::cos(glm::radians(CP_Y)) - temp_x * glm::sin(glm::radians(CP_Y));

		glutPostRedisplay();
	}

}

void TimerRevolution_CAMERA_Y(int value)	//카메라y 기준 자전
{
	if (stop_animation == 0) {

		CD_Y += cd_cw;	//고정값이 들어가야 등속운동한다.

		cameraDirection.x = 3.0f * glm::cos(glm::radians(CD_Y));
		cameraDirection.z = 3.0f * glm::sin(glm::radians(CD_Y)) + 3.0f;

		glutPostRedisplay();
	}
}

void Timer_Objmoving(int value) {
	if (stop_animation == 0 && stop_objs == 0) {
		철봉각도 += 2.0f;

		if(tree_leaf_size_up == 1)
			tree_leaf_size += 0.001f;
		else
			tree_leaf_size -= 0.001f;

		if (tree_leaf_size > 0.3)
			tree_leaf_size_up = 0;
		else if (tree_leaf_size < 0.2)
			tree_leaf_size_up = 1;

		if (벤치프레스각도증가 == 1) 
			벤치프레스각도 += 1;
		else 
			벤치프레스각도 -= 1;

		if (벤치프레스각도 <= 0)
			벤치프레스각도증가 = 1;
		else if (벤치프레스각도 >= 90)
			벤치프레스각도증가 = 0;

		벤치프레스오른하박움직임_x = 0.1 * glm::cos(glm::radians(벤치프레스각도+90));
		벤치프레스오른하박움직임_y = 0.1 * glm::sin(glm::radians(벤치프레스각도+90));
		벤치프레스왼하박움직임_x = 0.1 * glm::cos(glm::radians(-벤치프레스각도 + 90));
		벤치프레스왼하박움직임_y = 0.1 * glm::sin(glm::radians(-벤치프레스각도 + 90));

		러닝머신각도 += 1;

		if (러닝머신로봇다리각도증가 == 1)
			러닝머신로봇다리각도 += 2;
		else
			러닝머신로봇다리각도 -= 2;

		if (러닝머신로봇다리각도 <= -36)
			러닝머신로봇다리각도증가 = 1;
		else if (러닝머신로봇다리각도 >= 36)
			러닝머신로봇다리각도증가 = 0;


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

	case 'w'://뒤
		if (direction == 360 || stop_animation == 1) {
			stop_animation = 0;
			direction = 180.0f;
			glutTimerFunc(0, Timer_Move_ahead, 1);
			glutTimerFunc(0, Timer_Walk, 1);
		}
		else
			direction = 180.0f;
		break;
	case 's'://앞
		if (direction == 360 || stop_animation == 1) {
			stop_animation = 0;
			direction = 0.0f;
			glutTimerFunc(0, Timer_Move_ahead, 1);
			glutTimerFunc(0, Timer_Walk, 1);
		}
		else
			direction = 0.0f;
		break;
	case 'a'://왼
		if (direction == 360 || stop_animation == 1) {
			stop_animation = 0;
			direction = -90.0f;
			glutTimerFunc(0, Timer_Move_ahead, 1);
			glutTimerFunc(0, Timer_Walk, 1);
		}
		else
			direction = -90.0f;
		break;
	case 'd'://오
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
	case 'I':	//초기 설정값으로
		stop_animation = 1;
		stop_objs = 1;
		direction = 360.0f;
		walk = 0;
		frontscreen_y = 0, frontscreen_r = 0;
		move_x = 0; move_y = -0.17f; move_z = 0.5f;
		cameraPos.x = 0.0f; cameraPos.y = 1.0f; cameraPos.z = 2.3f; cameraDirection.x = 0.0f; cameraDirection.z = 0.0f;
		CP_Y = 0.0f; cp_cw = 0; CD_Y = 270.0f; cd_cw = 0;
		tree_leaf_size = 0.2; tree_leaf_size_up = 1;
		철봉각도 = 0.0; 
		벤치프레스각도 = 90.0f; 벤치프레스각도증가 = 0;	벤치프레스오른하박움직임_x = -0.1, 벤치프레스오른하박움직임_y = 0, 벤치프레스왼하박움직임_x = 0.1, 벤치프레스왼하박움직임_y = 0;
		러닝머신각도 = 0.0f; 러닝머신로봇다리각도 = 0; 러닝머신로봇다리각도증가 = 0;
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