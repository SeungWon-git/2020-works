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

//주소
#define hexahedronpoints	36	//-> 총 점의 갯수 = 삼각형(폴리곤) 갯수 * 3(꼭짓점)
#define pyramidpoints		18

GLfloat hexahedron_s[hexahedronpoints][3] = {};
GLfloat pyramid_s[pyramidpoints][3] = {};
GLfloat linex_s[2][3] = { {-1.0,0.0,0.0},{1.0,0.0,0.0} };
GLfloat liney_s[2][3] = { {0.0,-1.0,0.0},{0.0,1.0,0.0} };
GLfloat linez_s[2][3] = { {0.0,0.0,-1.0},{0.0,0.0,1.0} };

//색상
GLfloat hexahedron_c[hexahedronpoints][3] = {};
GLfloat pyramid_c[pyramidpoints][3] = {};
GLfloat linex_c[2][3] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };
GLfloat liney_c[2][3] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };
GLfloat linez_c[2][3] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };

//vao,vbo
GLuint 무대_vao, hexahedron_vbo[2];
GLuint pyramid_vao, pyramid_vbo[2];
GLuint linex_vao, linex_vbo[2], liney_vao, liney_vbo[2], linez_vao, linez_vbo[2];

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

GLclampf r_y = 0.0f;	//자전 방향전환
GLfloat RY = 0.0f;		//자전 각도





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

	glShaderSource(vertexshader, 1, (const GLchar * *)& vertexsource, 0);

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

	glShaderSource(fragmentshader, 1, (const GLchar * *)& fragmentsource, 0);

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

void InitShader()
{
	// obj읽기
	char filename[30] = "정육면체.obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, hexahedron_s);

	strcpy(filename,"사각뿔(피라미드).obj");
	file = fopen(filename, "rb");
	if (!file) {
		std::cout << "\"" << filename << " \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else
		ReadObj(file, pyramid_s);

	fclose(file);

	// 색 초기화
	for (int i = 0; i < hexahedronpoints; ++i) {
		if (i < 6) {
			hexahedron_c[i][0] = 1.0f; hexahedron_c[i][1] = 0.0f; hexahedron_c[i][2] = 1.0f;//뒤-보라
			if (i < 3) {
				pyramid_c[i][0] = 1.0f; pyramid_c[i][1] = 0.0f; pyramid_c[i][2] = 1.0f;
			}
			else {
				pyramid_c[i][0] = 0.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 0.0f;
			}
		}
		else if (i < 12) {
			hexahedron_c[i][0] = 0.0f; hexahedron_c[i][1] = 1.0f; hexahedron_c[i][2] = 0.0f;//왼-초록
			if (i < 9) {
				pyramid_c[i][0] = 1.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 0.0f;
			}
			else {
				pyramid_c[i][0] = 0.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 1.0f;
			}
		}
		else if (i < 18) {
			hexahedron_c[i][0] = 0.0f; hexahedron_c[i][1] = 0.0f; hexahedron_c[i][2] = 1.0f;//윗-파랑
			if (i < 15) {
				pyramid_c[i][0] = 0.0f; pyramid_c[i][1] = 1.0f; pyramid_c[i][2] = 1.0f;
			}
			else {
				pyramid_c[i][0] = 1.0f; pyramid_c[i][1] = 0.0f; pyramid_c[i][2] = 0.0f;
			}
		}
		else if (i < 24) {
			hexahedron_c[i][0] = 1.0f; hexahedron_c[i][1] = 1.0f; hexahedron_c[i][2] = 0.0f;//오른-노랑
		}
		else if (i < 30) {
			hexahedron_c[i][0] = 0.0f; hexahedron_c[i][1] = 1.0f; hexahedron_c[i][2] = 1.0f;//아래-하늘
		}
		else if (i < 36) {
			hexahedron_c[i][0] = 1.0f; hexahedron_c[i][1] = 0.0f; hexahedron_c[i][2] = 0.0f;//앞-빨강
		}
	}

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

void InitBuffer()
{
	InitVao(무대_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexahedron_c);

	InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

	InitVao(linex_vao, linex_vbo, 2, linex_s, linex_c);
	InitVao(liney_vao, liney_vbo, 2, liney_s, liney_c);
	InitVao(linez_vao, linez_vbo, 2, linez_s, linez_c);
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

		glDrawArrays(GL_TRIANGLES, 0, 12);	//0번~11번까지	
		glDrawArrays(GL_TRIANGLES, 18, 12);	//18번~29번까지

		{//윗면 회전 애니메이션 (중요- 회전은 무조건 x,y,z를 중심으로밖에 못하므로 로컬값을 변경(원하는 축에 맞춤)해줘서 회전시킨후 다시 원위치를 해줘야한다.)
			for (int i = 12; i < 18; ++i)	//해당 인덱스의 로컬값 수정(y값을 0으로-> x축에 회전할것이기 때문에)
				hexahedron_s[i][1] -= 0.25;

			InitVao(무대_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexahedron_c);	//vao 업데이트

			mTransform = glm::translate(mTransform, glm::vec3(0.0, 0.25, 0.0));	//모델전체 위로 이동(원점도 같이 이동하게됨 - 제자리에 위치하는 역활)
			mTransform = glm::rotate(mTransform, glm::radians(RUP), glm::vec3(1.0, 0.0, 0.0));	//이제 x축에대해 회전
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 12, 6);	//12번~17번까지(윗면)

			for (int i = 12; i < 18; ++i)	//해당 인덱스의 로컬값 원상복귀 - 혹시 후에 이 좌표를 그냥 쓸수도 있기 때문에
				hexahedron_s[i][1] += 0.25;
		}

		{//옆면 회전 애니메이션
			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] += 0.25; hexahedron_s[i][2] -= 0.25;
			}

			InitVao(무대_vao, hexahedron_vbo, hexahedronpoints, hexahedron_s, hexahedron_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.25, 0.25));
			mTransform = glm::rotate(mTransform, glm::radians(RSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 30, 6);	//30번~35번까지(옆면)

			for (int i = 30; i < 36; ++i) {
				hexahedron_s[i][1] -= 0.25; hexahedron_s[i][2] += 0.25;
			}
		}
	}
	else {	//사각뿔(피라미드)
		glBindVertexArray(pyramid_vao);

		glDrawArrays(GL_TRIANGLES, 9, 6);	//9번~14번까지

		{//옆면 회전 애니메이션
			for (int i = 0; i < 3; ++i) {
				pyramid_s[i][1] += 0.15; pyramid_s[i][2] += 0.25;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.15, -0.25));
			mTransform = glm::rotate(mTransform, glm::radians(-PSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 0, 3);	//0번~2번까지

			for (int i = 0; i < 3; ++i) {
				pyramid_s[i][1] -= 0.15; pyramid_s[i][2] -= 0.25;
			}
		}
		{//옆면 회전 애니메이션
			for (int i = 3; i < 6; ++i) {
				pyramid_s[i][0] += 0.25; pyramid_s[i][1] += 0.15;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(-0.25, -0.15, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(PSIDE), glm::vec3(0.0, 0.0, 1.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 3, 3);	//3번~5번까지

			for (int i = 3; i < 6; ++i) {
				pyramid_s[i][0] -= 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//옆면 회전 애니메이션
			for (int i = 6; i < 9; ++i) {
				pyramid_s[i][0] -= 0.25; pyramid_s[i][1] += 0.15;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.25, -0.15, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(-PSIDE), glm::vec3(0.0, 0.0, 1.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 6, 3);	//6번~8번까지

			for (int i = 6; i < 9; ++i) {
				pyramid_s[i][0] += 0.25; pyramid_s[i][1] -= 0.15;
			}
		}
		{//옆면 회전 애니메이션
			for (int i = 15; i < 18; ++i) {
				pyramid_s[i][1] += 0.15; pyramid_s[i][2] -= 0.25;
			}

			InitVao(pyramid_vao, pyramid_vbo, pyramidpoints, pyramid_s, pyramid_c);

			mTransform = glm::mat4(1.0f);
			mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
			mTransform = glm::rotate(mTransform, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(40.0f), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
			mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));

			mTransform = glm::translate(mTransform, glm::vec3(0.0, -0.15, 0.25));
			mTransform = glm::rotate(mTransform, glm::radians(PSIDE), glm::vec3(1.0, 0.0, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glDrawArrays(GL_TRIANGLES, 15, 3);	//15번~17번까지

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
	if(b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
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
	glutCreateWindow("Example16");

	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}