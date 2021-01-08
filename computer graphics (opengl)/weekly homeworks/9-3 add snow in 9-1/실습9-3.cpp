#pragma warning(disable: 4996)
#include <random>
#include <iostream>
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

using namespace std;

std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid{ 0.01,0.05 };

//주소
#define spherepoints_1	960*3	//-> 총 점의 갯수 = 삼각형(폴리곤) 갯수 * 3(꼭짓점)
#define	pyramidpoints	18
GLfloat sphere_s[spherepoints_1][3] = {};
GLfloat orbit_s[360][3];
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat floor_s[6][3] = {
	{1,-1,1} ,{-1,-1,-1}, {-1,-1,1},
	{-1,-1,-1} ,{1,-1,1}, {1,-1,-1}
};

//법선
GLfloat sphere_n[spherepoints_1][3] = {};	//--- 노말값 (Normal)
GLfloat pyramid_n[pyramidpoints][3] = {};
GLfloat floor_n[6][3] = {
	{0,1,0} ,{0,1,0}, {0,1,0},
	{0,1,0} ,{0,1,0}, {0,1,0}
};

//vao,vbo
GLuint solar_vao, solar_vbo[2];
GLuint earth1_vao, earth1_vbo[2], earth2_vao, earth2_vbo[2], earth3_vao, earth3_vbo[2];
GLuint moon1_vao, moon1_vbo[2], moon2_vao, moon2_vbo[2], moon3_vao, moon3_vbo[2];
GLuint orbit_vao, orbit_vbo[2];
GLuint light_vao, light_vbo[2];
GLuint pyramid_vao, pyramid_vbo[2];
GLuint floor_vao, floor_vbo[2];
GLuint snow_vao[20],snow_vbo[20][2];

//전역 변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 1.5f, 5.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool perspective = 1;

GLfloat S_x = 0.0, S_z = 0.0;
GLfloat E1_x = 0.0, E1_z = 0.0, E2_x = 0.0, E2_z = 0.0, E3_x = 0.0, E3_z = 0.0;	//행성의 극좌표
float angle_s = 0.0;
float angle_e1 = 0.0, angle_e2 = 0.0, angle_e3 = 0.0;	//행성의 극좌표 각도
GLfloat revolution_s = uid(dre) * 100;
GLfloat revolution_e1 = uid(dre) * 100; 
GLfloat revolution_e2 = uid(dre); GLfloat revolution_e3 = uid(dre);	//행성의 공전 속도

GLfloat M1_x = 0.0, M1_z = 0.0, M2_x = 0.0, M2_z = 0.0, M3_x = 0.0, M3_z = 0.0;	//달의 극좌표
float angle_m1 = 0.0, angle_m2 = 0.3, angle_m3 = 0.5;	//달의 극좌표 각도
GLfloat revolution_moon = uid(dre) * 100;

bool wire = 0;
GLfloat move_x = 0, move_y = 0.0f, move_z = 0.0f;
GLclampf change_size = 0.05f; //크기배율
GLclampf c_s = 0.01f;	//수축/팽창 전환
GLclampf r_y = 0.0f;	//방향전환
GLfloat RY = 0.0f;		//각도
bool stop_animation = 1;

GLfloat light_r = 1.0f, light_g = 1.0f, light_b = 1.0f;
int light_cnum = 0;
GLfloat light_x = -2.3f, light_y = 2.3f, light_z = 0.0f;
float light_rad = 90.0f;

bool lightmove = 0;

bool start_snow = 0;
GLfloat snow_loc_x[20], snow_loc_y[20], snow_loc_z[20];
bool draw_snow[20] = { 0, };



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

void ReadObj(FILE* objFile, GLfloat(*obj_v)[3], GLfloat(*obj_vn)[3])
{
	using namespace glm;

	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];
	int vertexNum = 0;
	int vertexnormalNum = 0;
	int faceNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'v' && count[1] == 'n' && count[2] == '\0')
			vertexnormalNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // 배열 초기화
	}
	rewind(objFile);	// 읽기 위치 다시 맨 앞으로

	//--- 2. 메모리 할당
	vec4* vertex, * vertexnormal;
	vec4* face_v, * face_vn;
	int vertIndex = 0;
	int vertnormalIndex = 0;
	int faceIndex = 0;
	vertex = (vec4*)malloc(sizeof(vec4) * vertexNum);
	vertexnormal = (vec4*)malloc(sizeof(vec4) * vertexnormalNum);
	face_v = (vec4*)malloc(sizeof(vec4) * faceNum);
	face_vn = (vec4*)malloc(sizeof(vec4) * faceNum);

	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	char bind[5];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", bind);
		if (bind[0] == 'v' && bind[1] == '\0') {
			fscanf(objFile, "%f %f %f", &vertex[vertIndex].x, &vertex[vertIndex].y, &vertex[vertIndex].z);
			//std::cout << "v["<< vertIndex <<"] - "<<vertex[vertIndex].x << ", " << vertex[vertIndex].y << ", " << vertex[vertIndex].z << std::endl;
			vertIndex++;
		}
		else if (bind[0] == 'v' && bind[1] == 'n' && bind[2] == '\0') {
			fscanf(objFile, "%f %f %f", &vertexnormal[vertnormalIndex].x, &vertexnormal[vertnormalIndex].y, &vertexnormal[vertnormalIndex].z);
			//std::cout << "vn[" << vertnormalIndex << "] - " << vertexnormal[vertnormalIndex].x << ", " << vertexnormal[vertnormalIndex].y << ", " << vertexnormal[vertnormalIndex].z << std::endl;
			vertnormalIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%f/%f/%f %f/%f/%f %f/%f/%f", &face_v[faceIndex].x, &count, &face_vn[faceIndex].x, &face_v[faceIndex].y, &count, &face_vn[faceIndex].y, &face_v[faceIndex].z, &count, &face_vn[faceIndex].z);
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

	//--- 5.메모리 반환
	free(vertex);
	free(vertexnormal);
	free(face_v);
	free(face_vn);
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
	char filename[30] = "sphere(1).obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, sphere_s, sphere_n);

	strcpy(filename,"피라미드.obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, pyramid_s, pyramid_n);

	fclose(file);

	// 공전 궤도 초기화
	for (int i = 0; i < 360; ++i) {
		orbit_s[i][0] = 1.5 * glm::cos(i), orbit_s[i][1] = 0.0f, orbit_s[i][2] = 1.5 * glm::sin(i);	//좌표 초기화
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

void InitVao(GLuint& vao, GLuint vbo[2], int num, GLfloat Shape[][3], GLfloat Normal[][3]) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);

	//주소
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), Shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//조명
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, num * 3 * sizeof(GLfloat), Normal, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}

void InitBuffer()
{
	InitVao(solar_vao, solar_vbo, spherepoints_1, sphere_s, sphere_n);

	InitVao(earth1_vao, earth1_vbo, spherepoints_1, sphere_s, sphere_n);
	//InitVao(earth2_vao, earth2_vbo, spherepoints, sphere_s, earth_c);
	//InitVao(earth3_vao, earth3_vbo, spherepoints, sphere_s, earth_c);

	InitVao(moon1_vao, moon1_vbo, spherepoints_1, sphere_s, sphere_n);
	//InitVao(moon2_vao, moon2_vbo, spherepoints, sphere_s, moon_c);
	//InitVao(moon3_vao, moon3_vbo, spherepoints, sphere_s, moon_c);

	//InitVao(orbit_vao, orbit_vbo, 360, orbit_s, orbit_c);

	InitVao(light_vao, light_vbo, spherepoints_1, sphere_s, sphere_n);

	InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_n);

	InitVao(floor_vao, floor_vbo, 6, floor_s, floor_n);

	for (int i = 0; i < 20; ++i) 
		InitVao(snow_vao[i], snow_vbo[i], spherepoints_1, sphere_s, sphere_n);
}




GLvoid drawScene()
{
	glClearColor(0.0, 0.0, 0.0, 0.0f);
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
	mTransform = glm::translate(mTransform, glm::vec3(S_x, 0.0, S_z));	
	mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	vTransform = glm::mat4(1.0f);	// 카메라 변환
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLoc = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);

	pTransform = glm::mat4(1.0f);	// 투영 변환
	if (perspective == 1)	//원근 투영
		pTransform = glm::perspective(glm::radians(60.0f), window_w / window_h, 0.1f, 200.0f);
	else					//직각 투영
		pTransform = glm::ortho(-2.0, 2.0, -2.0, 2.0, 0.1, 50.0);
	projLoc = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);

	//조명
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos");		//--- lightPos 값 전달
	glUniform3f(lightPosLocation, light_x, light_y, light_z);
	int lightColorLocation = glGetUniformLocation(s_program, "lightColor");	//--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, light_r, light_g, light_b);
	int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
	glUniform3f(objColorLocation, 0.0, 0.0, 1.0);

	//태양
	glBindVertexArray(solar_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints_1);
	else
		glDrawArrays(GL_TRIANGLES, 0, spherepoints_1);

	//행성1	
	mTransform = glm::mat4(1.0f);	//초기화
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));	//상하좌우 움직임
	mTransform = glm::translate(mTransform, glm::vec3(E1_x, 0.0, E1_z));	//y축을 중심으로 하는 극좌표계위로 이동(공전)	
	mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.5, change_size * 0.5, change_size * 0.5));	//크기변환
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));	//자전
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
	glUniform3f(objColorLocation, 0.0, 1.0, 0.0);

	glBindVertexArray(earth1_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints_1);
	else
		glDrawArrays(GL_TRIANGLES, 0, spherepoints_1);

	//달1
	mTransform = glm::mat4(1.0f);	//초기화
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));	//상하좌우 움직임	
	mTransform = glm::translate(mTransform, glm::vec3(M1_x, 0.0, M1_z));	
	mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.2, change_size * 0.2, change_size * 0.2));	//크기변환
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));	//자전
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
	glUniform3f(objColorLocation, 1, 0, 0);

	glBindVertexArray(moon1_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints_1);
	else
		glDrawArrays(GL_TRIANGLES, 0, spherepoints_1);

	//피라미드
	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::translate(mTransform, glm::vec3(0, -1, 0));
	mTransform = glm::scale(mTransform, glm::vec3(1.8, 1.8, 1.8));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
	glUniform3f(objColorLocation, 1, 0.83, 0);

	glBindVertexArray(pyramid_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, pyramidpoints);
	else
		glDrawArrays(GL_TRIANGLES, 0, pyramidpoints);

	//바닥
	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::translate(mTransform, glm::vec3(0, 0.3, 0));
	mTransform = glm::scale(mTransform, glm::vec3(1.8, 1.8, 1.8));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
	glUniform3f(objColorLocation, 0.3, 0.3, 0.3);

	glBindVertexArray(floor_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, 6);
	else
		glDrawArrays(GL_TRIANGLES, 0, 6);

	//조명
	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::translate(mTransform, glm::vec3(light_x, light_y, light_z));
	mTransform = glm::scale(mTransform, glm::vec3(0.01f, 0.01f, 0.01f));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
	glUniform3f(objColorLocation, 1, 1, 1);

	glBindVertexArray(light_vao);
	glDrawArrays(GL_TRIANGLES, 0, spherepoints_1);

	// 행성1 공전궤도
	mTransform = glm::mat4(1.0f);
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::scale(mTransform, glm::vec3(1.0, 1.0, 1.0));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	//glDrawArrays(GL_POINTS, 0, 360);

	//달1 공전궤도
	mTransform = glm::mat4(1.0f);
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::translate(mTransform, glm::vec3(E1_x, 0.0, E1_z));
	mTransform = glm::scale(mTransform, glm::vec3(0.333, 0.333, 0.333));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	//glDrawArrays(GL_POINTS, 0, 360);

	//눈20개
	for (int i = 0; i < 20; ++i) {
		if (draw_snow[i] == 1) {
			mTransform = glm::mat4(1.0f);	//초기화
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));	
			mTransform = glm::translate(mTransform, glm::vec3(snow_loc_x[i], snow_loc_y[i], snow_loc_z[i]));
			mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.15, change_size * 0.15, change_size * 0.15));	//크기변환
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
			glUniform3f(objColorLocation, 1, 1, 1);

			glBindVertexArray(snow_vao[i]);
			if (wire == 1)
				glDrawArrays(GL_LINE_STRIP, 0, spherepoints_1);
			else
				glDrawArrays(GL_TRIANGLES, 0, spherepoints_1);
		}
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Timer_Revolution_Planet(int value)	//행성의 공전
{
	angle_s += revolution_s;
	S_x = -2 * glm::cos(glm::radians(angle_s));
	S_z = -2 * glm::sin(glm::radians(angle_s));

	angle_e1 += revolution_e1;
	E1_x = -1.5 * glm::cos(glm::radians(angle_e1));
	E1_z = -1.5 * glm::sin(glm::radians(angle_e1));

	angle_m1 += revolution_moon;
	M1_x = -0.5 * glm::cos(glm::radians(angle_m1));
	M1_z = -0.5 * glm::sin(glm::radians(angle_m1));

	/*angle_e2 += revolution_e2;
	E2_x = 1.5 * glm::cos(angle_e2);
	E2_z = 1.5 * glm::sin(angle_e2);

	angle_e3 += revolution_e3;
	E3_x = -1.5 * glm::cos(angle_e3);
	E3_z = -1.5 * glm::sin(angle_e3);*/

	/*angle_m2 += revolution_moon;
	M2_x = 0.5 * glm::cos(angle_m2);
	M2_z = 0.5 * glm::sin(angle_m2);

	angle_m3 += revolution_moon;
	M3_x = 0.5 * glm::cos(angle_m3);
	M3_z = 0.5 * glm::sin(angle_m3);*/

	glutPostRedisplay();
	glutTimerFunc(50, Timer_Revolution_Planet, 1);
}

void Timer_Rotate_Model_Y(int value)	//자전
{
	if (stop_animation == 0) {

		RY += r_y;

		if (RY == 360.0f)
			RY = 0.0f;
		if (RY == 0.0f)
			RY = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(50, Timer_Rotate_Model_Y, 1);
	}
}

void Timer_Revolution_Light_Z(int value)	//조명 공전
{
	if (stop_animation == 0 && lightmove == 1) {
		light_rad -= 1;
		light_y = 2.3 * glm::sin(glm::radians(light_rad)); 
		light_z = 2.3 * glm::cos(glm::radians(light_rad));

		glutPostRedisplay();
		glutTimerFunc(20, Timer_Revolution_Light_Z, 1);
	}
}

void Timer_Snow(int value) {
	if (stop_animation == 0 && start_snow == 1) {
		int num = value / 5;

		if (value % 5 == 0 && draw_snow[num] == 0) {	//눈 내리기 시작
			draw_snow[num] = 1;

			snow_loc_y[num] = 3.0f;
			snow_loc_x[num] = uid(dre) * 100 - 3;	// -2 ~ 2
			snow_loc_z[num] = uid(dre) * 100 - 3;
		}

		for (int i = 0; i < 20; ++i)
		{
			if (draw_snow[i] == 1) {
				if (snow_loc_y[i] < -3.0f) 	//화면을 벗어나면 그리기 멈추기
					draw_snow[i] = 0;
				else
					snow_loc_y[i] -= 0.1f;	//눈 내리기
			}
		}

		value++;	
		if (value == 100)
			value = 0;

		glutPostRedisplay();
		glutTimerFunc(50, Timer_Snow, value);
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
	case 'm':
		wire = 0;
		break;
	case 'M':
		wire = 1;
		break;

	case '4':
		move_x -= 0.05;
		break;
	case '6':
		move_x += 0.05;
		break;
	case '8':
		move_y += 0.05;
		break;
	case '2':
		move_y -= 0.05;
		break;
	case 'z':
		move_z -= 0.05;
		break;
	case 'Z':
		move_z += 0.05;
		break;

	case 'y':
		stop_animation = 0;
		r_y = -1.0f;
		glutTimerFunc(0, Timer_Rotate_Model_Y, 1);
		break;
	case 'Y':
		stop_animation = 0;
		r_y = 1.0f;
		glutTimerFunc(0, Timer_Rotate_Model_Y, 1);
		break;

	/*case 'S':
		stop_animation = 0;
		glutTimerFunc(50, TimerChangeSize, 1);
		break;*/
	case 'P':
		stop_animation = 1;
		move_x = 0; move_y = 0.0f; move_z = 0.0f;
		RY = 0.0f;
		change_size = 0.05f;
		lightmove = 0;
		break;

	case 'p':
		if (perspective == 0)
			perspective = 1;
		else
			perspective = 0;
		break;

	case 'c':
	case 'C':
		if (light_cnum == 0) {
			light_r = 1.0f, light_g = 0.0f, light_b = 0.0f;
			light_cnum++;
		}
		else if (light_cnum == 1) {
			light_r = 0.0f, light_g = 1.0f, light_b = 0.0f;
			light_cnum++;
		}
		else if (light_cnum == 2) {
			light_r = 0.0f, light_g = 0.0f, light_b = 1.0f;
			light_cnum++;
		}
		else if (light_cnum == 3) {
			light_r = 0.0f, light_g = 0.0f, light_b = 0.0f;
			light_cnum++;
		}
		else if (light_cnum == 4) {
			light_r = 1.0f, light_g = 1.0f, light_b = 0.0f;
			light_cnum++;
		}
		else if (light_cnum == 5) {
			light_r = 1.0f, light_g = 0.0f, light_b = 1.0f;
			light_cnum++;
		}
		else if (light_cnum == 6) {
			light_r = 0.0f, light_g = 1.0f, light_b = 1.0f;
			light_cnum++;
		}
		else if (light_cnum == 7) {
			light_r = 1.0f, light_g = 1.0f, light_b = 1.0f;
			light_cnum = 0;
		}
		break;
	case 'r':
	case 'R':
		if (lightmove != 1) {
			stop_animation = 0;
			lightmove = 1;
			glutTimerFunc(0, Timer_Revolution_Light_Z, 1);
		}
		else
			lightmove = 0;
		break;
	case 's':
	case 'S':
		if (start_snow != 1) {
			stop_animation = 0;
			start_snow = 1;

			glutTimerFunc(0, Timer_Snow, 0);
		}
		else
			start_snow = 0;
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

	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(window_w, window_h);
	glutCreateWindow("Example22");

	glewExperimental = GL_TRUE;
	glewInit();

	InitModel();
	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	glutTimerFunc(0, Timer_Revolution_Planet, 1);
	//glutMouseFunc(Mouse);
	glutMainLoop();
}