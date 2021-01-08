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
#define spherepoints	320		//-> 총 점의 갯수 = 삼각형(폴리곤) 갯수 * 3(꼭짓점)

GLfloat floor_s[hexahedronpoints][3] = {};
GLfloat cranebo_s[hexahedronpoints][3] = {};
GLfloat craneup_s[hexahedronpoints][3] = {};
GLfloat craneleg_s[hexahedronpoints][3] = {};
GLfloat sphere_s[spherepoints][3] = {};


//색상
GLfloat floor_c[hexahedronpoints][3] = {};
GLfloat cranebo_c[hexahedronpoints][3] = {};
GLfloat	craneup_c[hexahedronpoints][3] = {};
GLfloat craneleg_c[hexahedronpoints][3] = {};


//법선
GLfloat floor_n[hexahedronpoints][3] = {};
GLfloat sphere_n[spherepoints][3] = {};

//vao,vbo
GLuint hexahedron_vao[4],	//0-바닥,1-크레인 밑,2-크레인 위,3-크레인 다리
floor_vbo[2], cranebo_vbo[2], craneup_vbo[2], craneleg_vbo[2];
GLuint light_vao, light_vbo[2];

//전역 변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 2.5f);			//카메라 위치 
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);	//카메라가 바라보는 기준점
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);			//카메라의 상단이 가리키는 방향

bool perspective = 1;
bool stop_animation = 1;

GLfloat move_x = 0, move_y = 0.0f, move_z = 0.0f;	//모델 이동

GLuint r_y = 0, r_x = 0;			//뱡향
GLfloat RY = 0.0f, RX = 0.0f;		//각도

GLfloat cw = 0.0f;		//카메라 방향
GLfloat CY = 0.0f;		//카메라 각도
GLfloat CD_Y = 270.0f;	//카메라가 바라보는 각도
GLfloat cd_cw = 0.0f;	//카메라가 바라보는 방향

GLfloat light_r = 1.0f, light_g = 1.0f, light_b = 1.0f;
int light_cnum = 0;
GLfloat light_x = 0.8f, light_y = 0.8f, light_z = 0.0f;
float light_rad = 0.0f, light_rad_inc = 0;

bool light_turn_on = 1;



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
	const char filename[30] = "구(2).obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, sphere_s, sphere_n);

	fclose(file);	//파일닫기

	const char filename1[30] = "정육면체.obj";
	FILE* file1 = fopen(filename1, "rb");
	if (!file) {
		std::cout << "\"" << filename1 << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file1, floor_s,floor_n);

	fclose(file1);	//파일닫기

	//위치 조정 (크레인)
	for (int i = 0; i < hexahedronpoints; ++i) {
		cranebo_s[i][0] = floor_s[i][0]; cranebo_s[i][1] = floor_s[i][1] + 0.5; cranebo_s[i][2] = floor_s[i][2];
	}

	for (int i = 0; i < hexahedronpoints; ++i) {
		craneup_s[i][0] = floor_s[i][0]; craneup_s[i][1] = cranebo_s[i][1] + 1.2; craneup_s[i][2] = floor_s[i][2];
	}

	for (int i = 0; i < hexahedronpoints; ++i) {
		craneleg_s[i][0] = floor_s[i][0]; craneleg_s[i][1] = floor_s[i][1] + 0.1; craneleg_s[i][2] = floor_s[i][2];
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
	InitVao(hexahedron_vao[0], floor_vbo, hexahedronpoints, floor_s, floor_n);

	InitVao(hexahedron_vao[1], cranebo_vbo, hexahedronpoints, cranebo_s, floor_n);
	InitVao(hexahedron_vao[2], craneup_vbo, hexahedronpoints, craneup_s, floor_n);
	InitVao(hexahedron_vao[3], craneleg_vbo, hexahedronpoints, craneleg_s, floor_n);

	InitVao(light_vao, light_vbo, spherepoints, sphere_s, sphere_n);
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

	//조명
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos");		//--- lightPos 값 전달
	glUniform3f(lightPosLocation, light_x, light_y, light_z);
	int lightColorLocation = glGetUniformLocation(s_program, "lightColor");	//--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, light_r, light_g, light_b);

	{//바닥
		mTransform = glm::mat4(1.0f);	//모델 변환
		mTransform = glm::scale(mTransform, glm::vec3(4.0, 1.0, 4.0));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
		glUniform3f(objColorLocation, 0.59, 0.3, 0.0);

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

		glBindVertexArray(hexahedron_vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
	}

	{//크레인 밑둥
		mTransform = glm::mat4(1.0f);
		mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
		mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::scale(mTransform, glm::vec3(0.8, 0.5, 0.8));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
		glUniform3f(objColorLocation, 0.9, 0.0, 0.0);

		glBindVertexArray(hexahedron_vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
	}
	{//크레인 몸통
		mTransform = glm::mat4(1.0f);
		mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
		mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::scale(mTransform, glm::vec3(0.4, 0.25, 0.4));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
		glUniform3f(objColorLocation, 1.0, 0.8, 0.0);

		glBindVertexArray(hexahedron_vao[2]);
		glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);
	}
	{//크레인 다리
		for (int i = 0; i < hexahedronpoints; ++i) {
			craneleg_s[i][0] += 0.5;
		}
		InitVao(hexahedron_vao[3], craneleg_vbo, hexahedronpoints, craneleg_s, craneleg_c);

		mTransform = glm::mat4(1.0f);
		mTransform = glm::translate(mTransform, glm::vec3(0.0, 0.5, 0));
		mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
		mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::rotate(mTransform, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
		mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.25, 0.1));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
		glUniform3f(objColorLocation, 0.3, 0.74, 1.0);

		glBindVertexArray(hexahedron_vao[3]);
		glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

		for (int i = 0; i < hexahedronpoints; ++i) {
			craneleg_s[i][0] -= 0.5;
		}
		InitVao(hexahedron_vao[3], craneleg_vbo, hexahedronpoints, craneleg_s, craneleg_c);
	}
	{//크레인 다리
		for (int i = 0; i < hexahedronpoints; ++i) {
			craneleg_s[i][0] -= 0.5;
		}
		InitVao(hexahedron_vao[3], craneleg_vbo, hexahedronpoints, craneleg_s, craneleg_c);

		mTransform = glm::mat4(1.0f);
		mTransform = glm::translate(mTransform, glm::vec3(0.0, 0.5, 0));
		mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
		mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
		mTransform = glm::rotate(mTransform, glm::radians(-RX), glm::vec3(1.0, 0.0, 0.0));
		mTransform = glm::scale(mTransform, glm::vec3(0.1, 0.25, 0.1));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		glBindVertexArray(hexahedron_vao[3]);
		glDrawArrays(GL_TRIANGLES, 0, hexahedronpoints);

		for (int i = 0; i < hexahedronpoints; ++i) {
			craneleg_s[i][0] += 0.5;
		}
		InitVao(hexahedron_vao[3], craneleg_vbo, hexahedronpoints, craneleg_s, craneleg_c);
	}
	{//조명
		mTransform = glm::mat4(1.0f);
		mTransform = glm::translate(mTransform, glm::vec3(light_x, light_y, light_z));
		mTransform = glm::scale(mTransform, glm::vec3(0.05, 0.05, 0.05));
		modelLoc = glGetUniformLocation(s_program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

		int objColorLocation = glGetUniformLocation(s_program, "objectColor");	//--- object Color값 전달
		glUniform3f(objColorLocation, 1, 1, 1);

		glBindVertexArray(light_vao);
		glDrawArrays(GL_POLYGON, 0, spherepoints);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerRotate_RY(int value)
{
	if (stop_animation == 0) {
		if (r_y == 1) {
			RY += 1.0f;

			if (RY >= 90.0f)
				r_y = -1;

			glutPostRedisplay();
			glutTimerFunc(30, TimerRotate_RY, 1);
		}
		else if (r_y == -1) {
			RY -= 1.0f;

			if (RY <= -90.0f)
				r_y = 1;

			glutPostRedisplay();
			glutTimerFunc(30, TimerRotate_RY, 1);
		}
	}
}

void TimerRotate_RX(int value)
{
	if (stop_animation == 0) {
		if (r_x == 1) {
			RX += 1.0f;

			if (RX >= 90.0f)
				r_x = -1;

			glutPostRedisplay();
			glutTimerFunc(30, TimerRotate_RX, 1);
		}
		else if (r_x == -1) {
			RX -= 1.0f;

			if (RX <= -90.0f)
				r_x = 1;

			glutPostRedisplay();
			glutTimerFunc(30, TimerRotate_RX, 1);
		}
	}
}

void TimerRotate_CAMERA_Y(int value)
{
	if (stop_animation == 0) {
		if (cw != 0.0f) {
			CY = cw;	//고정값이 들어가야 등속운동한다.

			float temp_x = cameraPos.x, temp_z = cameraPos.z;

			cameraPos.x = temp_x * glm::cos(CY) + temp_z * glm::sin(CY);	//cw만큼 움직(회전)였다는 뜻이므로
			cameraPos.z = temp_z * glm::cos(CY) - temp_x * glm::sin(CY);

			glutPostRedisplay();
			glutTimerFunc(30, TimerRotate_CAMERA_Y, 1);
		}
	}
}

void TimerRevolution_CAMERA_Y(int value)	//카메라y 기준 자전
{
	if (stop_animation == 0) {

		CD_Y += cd_cw;	//고정값이 들어가야 등속운동한다.

		cameraDirection.x = 2.5f * glm::cos(glm::radians(CD_Y));
		cameraDirection.z = 2.5f * glm::sin(glm::radians(CD_Y)) + 2.5f;

		glutPostRedisplay();
	}

}

void Timer_Light_Rotate_CenterY(int value) {
	if (stop_animation == 0) {
		light_rad += light_rad_inc;

		light_x = 0.8*glm::cos(glm::radians(light_rad)); light_z = 0.8*glm::sin(glm::radians(light_rad));

		glutPostRedisplay();
		glutTimerFunc(30, Timer_Light_Rotate_CenterY, 1);
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

	case 'b':
		move_x -= 0.05;
		break;
	case 'B':
		move_x += 0.05;
		break;

	/*case 'm':
		if (r_y == 0)
		{
			r_y = 1;
			stop_animation = 0;
			glutTimerFunc(0, TimerRotate_RY, 1);
		}
		else if (r_y == -1)
			r_y = 1;
		break;
	case 'M':
		if (r_y == 0)
		{
			r_y = -1;
			stop_animation = 0;
			glutTimerFunc(0, TimerRotate_RY, 1);
		}
		else if (r_y == 1) {
			stop_animation = 0;
			r_y = -1;
		}
		break;*/
	case 't':
		if (r_x == 0)
		{
			r_x = 1;
			stop_animation = 0;
			glutTimerFunc(0, TimerRotate_RX, 1);
		}
		else if (r_x == -1)
			r_x = 1;
		break;
	case 'T':
		if (r_x == 0)
		{
			r_x = -1;
			stop_animation = 0;
			glutTimerFunc(0, TimerRotate_RX, 1);
		}
		else if (r_x == 1)
			r_x = -1;
		break;

	case 'z':
		cameraPos.z -= 0.05;
		break;
	case 'Z':
		cameraPos.z += 0.05;
		break;
	case 'x':
		cameraPos.x += 0.05;
		cameraDirection.x += 0.05; //카메라 시점도 같이 움직여줘야 시점도 오른쪽으로 간다
		break;
	case 'X':
		cameraPos.x -= 0.05;
		cameraDirection.x -= 0.05;
		break;

	case 'l':
		if (cw == 0.0f)
		{
			cw = 0.03f;
			stop_animation = 0;
			glutTimerFunc(0, TimerRotate_CAMERA_Y, 1);
		}
		else if (cw == -0.03f)
			cw = 0.03f;
		break;
	case 'L':
		if (cw == 0.0f)
		{
			cw = -0.03f;
			stop_animation = 0;
			glutTimerFunc(0, TimerRotate_CAMERA_Y, 1);
		}
		else if (cw == 0.03f)
			cw = -0.03f;
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

	case 's':
	case 'S':
		r_y = 0; r_x = 0; cw = 0;
		stop_animation = 1;
		break;
	case 'o':
	case 'O':	//초기 설정값으로
		stop_animation = 1;
		move_x = 0; move_y = 0.0f; move_z = 0.0f;
		cameraPos.x = 0.0f; cameraDirection.x = 0.0f; cameraPos.z = 2.4f;
		RY = 0.0f; r_y = 0;	RX = 0.0f; r_x = 0; CY = 0; cw = 0;
		break;

	case 'm':
	case 'M':
		if (light_turn_on == 0)
		{
			light_turn_on = 1;
			light_r = 1; light_g = 1; light_b = 1;
		}
		else
		{
			light_turn_on = 0;
			light_r = 0; light_g = 0; light_b = 0;
		}
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
		if (light_rad_inc == 0) {
			stop_animation = 0;
			light_rad_inc = 1;
			glutTimerFunc(0, Timer_Light_Rotate_CenterY, 1);
		}
		else light_rad_inc = 1;
		if (stop_animation == 1) {
			stop_animation = 0;
			light_rad_inc = 1;
			glutTimerFunc(0, Timer_Light_Rotate_CenterY, 1);
		}
		break;
	case 'R':	
		if (light_rad_inc == 0) {
			stop_animation = 0;
			light_rad_inc = -1;
			glutTimerFunc(0, Timer_Light_Rotate_CenterY, 1);
		}
		else light_rad_inc = -1;
		if (stop_animation == 1){
			stop_animation = 0;
			light_rad_inc = -1;
			glutTimerFunc(0, Timer_Light_Rotate_CenterY, 1);
		}
		break;

	case 'q':
	case 'Q':
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
	glutCreateWindow("Example21");

	glewExperimental = GL_TRUE;
	glewInit();

	InitModel();
	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	//glutMouseFunc(Mouse);
	glutMainLoop();
}