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
#define spherepoints	1452	//-> 총 점의 갯수 = 삼각형(폴리곤) 갯수 * 3(꼭짓점)
GLfloat sphere_s[spherepoints][3] = {};
GLfloat orbit_s[360][3];

//색상
GLfloat solar_c[spherepoints][3] = {};
GLfloat earth_c[spherepoints][3] = {};
GLfloat moon_c[spherepoints][3] = {};
GLfloat orbit_c[360][3];

//vao,vbo
GLuint solar_vao, solar_vbo[2];
GLuint earth1_vao, earth1_vbo[2], earth2_vao, earth2_vbo[2], earth3_vao, earth3_vbo[2];
GLuint moon1_vao, moon1_vbo[2], moon2_vao, moon2_vbo[2], moon3_vao, moon3_vbo[2];
GLuint orbit_vao, orbit_vbo[2];

//전역 변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 700.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


GLfloat E1_x = 0.0, E1_z = 0.0, E2_x = 0.0, E2_z = 0.0, E3_x = 0.0, E3_z = 0.0;	//행성의 극좌표
float angle_e1 = 0.0, angle_e2 = 0.0, angle_e3 = 0.0;	//행성의 극좌표 각도
GLfloat revolution_e1 = uid(dre); GLfloat revolution_e2 = uid(dre); GLfloat revolution_e3 = uid(dre);	//행성의 공전 속도

GLfloat M1_x = 0.0, M1_z = 0.0, M2_x = 0.0, M2_z = 0.0, M3_x = 0.0, M3_z = 0.0;	//달의 극좌표
float angle_m1 = 0.0, angle_m2 = 0.3, angle_m3 = 0.5;	//달의 극좌표 각도
GLfloat revolution_moon = 0.1f;	//달의 공전 속도

bool wire = 0;
GLfloat move_x = 0, move_y = -0.1f, move_z = -0.1f;
GLclampf change_size = 2.0f; //크기배율
GLclampf c_s = 0.01f;	//수축/팽창 전환
GLclampf r_y = 0.0f;	//방향전환
GLfloat RY = 0.0f;		//각도
bool stop_animation = 1;



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

void ReadObj(FILE* objFile,GLfloat (*obj)[3])	
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
			fscanf(objFile, "%f %f %f",&vertex[vertIndex].x, &vertex[vertIndex].y,&vertex[vertIndex].z);
			//확인 -//std::cout << "v["<< vertIndex <<"] - "<<vertex[vertIndex].x << ", " << vertex[vertIndex].y << ", " << vertex[vertIndex].z << std::endl;
			vertIndex++;
		}
		else if (bind[0] == 'f' && bind[1] == '\0') {
			fscanf(objFile, "%f%s%f%s%f",&face[faceIndex].x, &count,&face[faceIndex].y, &count,&face[faceIndex].z);
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
	const char filename[30] = "구(1).obj";
	FILE* file = fopen(filename, "rb");
	if (!file) {	
		std::cout << "\"" << filename <<" \"파일을 읽지 못하였습니다." << std::endl;
		exit(0);	// 실패시 종료
	}
	else 
		ReadObj(file, sphere_s);

	fclose(file);
	

	// 색 초기화
	for (int i = 0; i < spherepoints; ++i) {
		solar_c[i][0] = 1.0f; solar_c[i][1] = 0.0f; solar_c[i][2] = 0.0f;

		earth_c[i][0] = 0.0f; earth_c[i][1] = 0.0f; earth_c[i][2] = 1.0f;

		moon_c[i][0] = 0.3f; moon_c[i][1] = 0.3f; moon_c[i][2] = 0.3f;
	}

	// 공전 궤도 초기화
	for (int i = 0; i < 360; ++i) {
		orbit_s[i][0] = 1.5 * glm::cos(i), orbit_s[i][1] = 0.0f, orbit_s[i][2] = 1.5 * glm::sin(i);	//좌표 초기화

		orbit_c[i][0] = 1.0f, orbit_c[i][1] = 1.0f, orbit_c[i][2] = 1.0f;	//색 초기화
	}

	//쉐이더 처리

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
	glBufferData(GL_ARRAY_BUFFER, num * 3 *sizeof(GLfloat), Shape, GL_STATIC_DRAW);
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
	InitVao(solar_vao, solar_vbo, spherepoints,sphere_s,solar_c);

	InitVao(earth1_vao, earth1_vbo, spherepoints, sphere_s, earth_c);
	InitVao(earth2_vao, earth2_vbo, spherepoints, sphere_s, earth_c);
	InitVao(earth3_vao, earth3_vbo, spherepoints, sphere_s, earth_c);

	InitVao(moon1_vao, moon1_vbo, spherepoints, sphere_s, moon_c);
	InitVao(moon2_vao, moon2_vbo, spherepoints, sphere_s, moon_c);
	InitVao(moon3_vao, moon3_vbo, spherepoints, sphere_s, moon_c);

	InitVao(orbit_vao, orbit_vbo, 360, orbit_s, orbit_c);
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

	//태양	
	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::scale(mTransform, glm::vec3(change_size, change_size, change_size));
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);
	vTransform = glm::mat4(1.0f);	// 카메라 변환
	vTransform = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	viewLoc = glGetUniformLocation(s_program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &vTransform[0][0]);
	pTransform = glm::mat4(1.0f);	// 투영 변환
	pTransform = glm::perspective(glm::radians(60.0f), window_w / window_h, 0.1f, 200.0f);
	projLoc = glGetUniformLocation(s_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);
		
	glBindVertexArray(solar_vao);
	if(wire==1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints);
	else
		glDrawArrays(GL_POLYGON, 0, spherepoints);

	// 행성1 공전궤도
	mTransform = glm::mat4(1.0f);
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::scale(mTransform, glm::vec3(1.0, 1.0, 1.0));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	glDrawArrays(GL_POINTS, 0, 360);

	//달1 공전궤도
	mTransform = glm::mat4(1.0f);	
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::translate(mTransform, glm::vec3(E1_x, 0.0, E1_z));	
	mTransform = glm::scale(mTransform, glm::vec3(0.333, 0.333, 0.333));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	glDrawArrays(GL_POINTS, 0, 360);

	// 행성2 공전궤도
	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));	
	mTransform = glm::scale(mTransform, glm::vec3(1.0, 1.0, 1.0));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	glDrawArrays(GL_POINTS, 0, 360);

	//달2 공전궤도
	mTransform = glm::mat4(1.0f);
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));	
	mTransform = glm::translate(mTransform, glm::vec3(E2_x, 0.0, E2_z));	
	mTransform = glm::scale(mTransform, glm::vec3(0.333, 0.333, 0.333));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	glDrawArrays(GL_POINTS, 0, 360);

	// 행성3 공전궤도
	mTransform = glm::mat4(1.0f);	//모델 변환
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));	//xz평면을 z방향으로 -45도 기울임
	mTransform = glm::scale(mTransform, glm::vec3(1.0, 1.0, 1.0));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	glDrawArrays(GL_POINTS, 0, 360);

	//달3 공전궤도
	mTransform = glm::mat4(1.0f);
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	mTransform = glm::translate(mTransform, glm::vec3(E3_x, 0.0, E3_z));
	mTransform = glm::scale(mTransform, glm::vec3(0.333, 0.333, 0.333));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(orbit_vao);
	glDrawArrays(GL_POINTS, 0, 360);

	//행성1	
	mTransform = glm::mat4(1.0f);	//초기화
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));	//상하좌우 움직임
	mTransform = glm::translate(mTransform, glm::vec3(E1_x, 0.0, E1_z));	//y축을 중심으로 하는 극좌표계위로 이동(공전)	
	mTransform = glm::scale(mTransform, glm::vec3(change_size*0.5, change_size*0.5, change_size*0.5));	//크기변환
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));	//자전
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(earth1_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints);
	else
		glDrawArrays(GL_POLYGON, 0, spherepoints);

	//달1
	mTransform = glm::mat4(1.0f);	//초기화
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));	//상하좌우 움직임
	mTransform = glm::translate(mTransform, glm::vec3(E1_x, 0.0, E1_z));	//y축을 중심으로 하는 극좌표계위로 이동(공전)	
	mTransform = glm::translate(mTransform, glm::vec3(M1_x, 0.0, M1_z));	//이동한곳 y축중심으로 공전
	mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.2, change_size * 0.2, change_size * 0.2));	//크기변환
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));	//자전
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(moon1_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints);
	else
		glDrawArrays(GL_POLYGON, 0, spherepoints);

	//행성2	
	mTransform = glm::mat4(1.0f);	//초기화
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));	//움직임
	mTransform = glm::rotate(mTransform, glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));	//xz평면을 z방향으로 -45도 기울임
	mTransform = glm::translate(mTransform, glm::vec3(E2_x, 0.0, E2_z));	//y축을 중심으로 하는 극좌표계위로 이동(공전)	
	mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.5, change_size * 0.5, change_size * 0.5));	//크기변환
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));	//자전
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(earth2_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints);
	else
		glDrawArrays(GL_POLYGON, 0, spherepoints);

	//달2
	mTransform = glm::mat4(1.0f);	//초기화
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));	//움직임
	mTransform = glm::rotate(mTransform, glm::radians(-45.0f), glm::vec3(0.0, 0.0, 1.0));	//xz평면을 z방향으로 -45도 기울임
	mTransform = glm::translate(mTransform, glm::vec3(E2_x, 0.0, E2_z));	//y축을 중심으로 하는 극좌표계위로 이동(공전)	
	mTransform = glm::translate(mTransform, glm::vec3(M2_x, 0.0, M2_z));	//이동한곳 y축중심으로 공전
	mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.2, change_size * 0.2, change_size * 0.2));	//크기변환
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));	//자전
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(moon2_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints);
	else
		glDrawArrays(GL_POLYGON, 0, spherepoints);

	//행성3	
	mTransform = glm::mat4(1.0f);	
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	mTransform = glm::translate(mTransform, glm::vec3(E3_x, 0.0, E3_z));
	mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.5, change_size * 0.5, change_size * 0.5));
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(earth3_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints);
	else
		glDrawArrays(GL_POLYGON, 0, spherepoints);

	//달3
	mTransform = glm::mat4(1.0f);
	mTransform = glm::translate(mTransform, glm::vec3(move_x, move_y, move_z));
	mTransform = glm::rotate(mTransform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	mTransform = glm::translate(mTransform, glm::vec3(E3_x, 0.0, E3_z));
	mTransform = glm::translate(mTransform, glm::vec3(M3_x, 0.0, M3_z));	//이동한곳 y축중심으로 공전
	mTransform = glm::scale(mTransform, glm::vec3(change_size * 0.2, change_size * 0.2, change_size * 0.2));	//크기변환
	mTransform = glm::rotate(mTransform, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));	//자전
	modelLoc = glGetUniformLocation(s_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

	glBindVertexArray(moon3_vao);
	if (wire == 1)
		glDrawArrays(GL_LINE_STRIP, 0, spherepoints);
	else
		glDrawArrays(GL_POLYGON, 0, spherepoints);



	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void TimerRotate_Revolution(int value)
{
	//행성의 공전
	angle_e1 += revolution_e1;
	E1_x = -1.5 * glm::cos(angle_e1);
	E1_z = -1.5 * glm::sin(angle_e1);

	angle_e2 += revolution_e2;
	E2_x = 1.5 * glm::cos(angle_e2);
	E2_z = 1.5 * glm::sin(angle_e2);

	angle_e3 += revolution_e3;
	E3_x = -1.5 * glm::cos(angle_e3);
	E3_z = -1.5 * glm::sin(angle_e3);

	//달의 공전
	angle_m1 += revolution_moon;
	M1_x = 0.5 * glm::cos(angle_m1);
	M1_z = 0.5 * glm::sin(angle_m1);

	angle_m2 += revolution_moon;
	M2_x = 0.5 * glm::cos(angle_m2);
	M2_z = 0.5 * glm::sin(angle_m2);

	angle_m3 += revolution_moon;
	M3_x = 0.5 * glm::cos(angle_m3);
	M3_z = 0.5 * glm::sin(angle_m3);

	glutPostRedisplay();
	glutTimerFunc(50, TimerRotate_Revolution, 1);
}

void TimerRotate_RY(int value)
{
	if (stop_animation == 0) {

		RY += r_y;

		if (RY == 360.0f)
			RY = 0.0f;
		if (RY == 0.0f)
			RY = 360.0f;

		glutPostRedisplay();
		glutTimerFunc(50, TimerRotate_RY, 1);
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
		stop_animation = 0;
		r_y = -1.0f;
		glutTimerFunc(0, TimerRotate_RY, 1);
		break;
	case 'Y':
		stop_animation = 0;
		r_y = 1.0f;
		glutTimerFunc(0, TimerRotate_RY, 1);
		break;

	case 'S':
		stop_animation = 0;
		glutTimerFunc(50, TimerChangeSize, 1);
		break;
	case 'C':
		stop_animation = 1;
		move_x = 0; move_y = -0.1f; move_z = -0.1f;
		RY = 0.0f;
		change_size = 2.0f;
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
	glutCreateWindow("Example15");

	glewExperimental = GL_TRUE;
	glewInit();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	glutTimerFunc(0, TimerRotate_Revolution, 1);
	//glutMouseFunc(Mouse);
	glutMainLoop();
}