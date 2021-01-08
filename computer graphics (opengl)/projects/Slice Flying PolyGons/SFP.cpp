//made by Jeung Seung Won -20.11.15



#pragma warning(disable: 4996)
#include <random>
#include <iostream>

//오픈지엘
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

//소리재생
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace std;


std::random_device rd;
std::default_random_engine dre{ rd() };
std::uniform_real_distribution<> uid_s{ 0.05,0.25 };	//모양
std::uniform_real_distribution<> uid_c{ 0.0,1.0 };		//색
std::uniform_real_distribution<> uid_sy{ -0.25,0.4 };	//생성시 y위치
std::uniform_real_distribution<> uid_dx{ 0.05,0.065 };	//생성시 속도
std::uniform_real_distribution<> uid_dy{ 0.035,0.05 };
std::uniform_real_distribution<> uid_dfx{ -0.02,0.02 };	//떨어질때
std::uniform_real_distribution<> uid_dfy{ 0.01,0.02 };

#define Block_num	500

typedef struct Block {
	GLuint vao, vbo[2];		//vao,vbo
	GLfloat s[6][3];		//주소(모양)
	GLfloat c[6][3];		//색상
	size_t	status{ 0 };			//상태(0-대기, 1-슬라이스 가능 상태, 2-칸에 들어감, 5- 오각형으로 더 이상 잘리지 못함, 6- 오각형의 삼각형은 셀의 바운드 박스체크를 안 하기 위해), 7- 게임오버 폴링(엔딩시 사이즈 변화 막음)
	GLfloat m_x{ 0 }, m_y{ 0 };		//화면상 위치
	GLfloat m_dx, m_dy;		//움직이는 속도
	GLfloat l,r,b,u;		//바운딩박스를 위한 상하좌우값
	GLfloat size_x, size_y;	//셀 안 사이즈 조절을 위한 값
	int hexa_tri_index{ Block_num };		//오각형의 삼각형이 사각형을 따라가기 위해 삼각형의 인덱스 정보 저장

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
		else bu = max_u / 2;	//바운딩 박스 자를 시 모양 변함 조정

		l = (max_l + max_r) / 2 + bl; r = (max_l + max_r) / 2 + br; b = (max_b + max_u) / 2 + bb; u = (max_b + max_u) / 2 + bu;	
	}

}Block;

//블록
//#define Block_num	500
Block block[Block_num];

//바운딩 박스
GLuint bb_vao[Block_num], bb_vbo[Block_num][2];

//마우스 슬라이스
GLfloat mouse_s[2][3];
GLfloat mouse_c[2][3];
GLuint mouse_vao, mouse_vbo[2];

// 3x8 셀
typedef struct Cell{
	GLfloat s[4][3];	//좌표
	GLuint cell_vao, cell_vbo[2];
	bool fill_cell{ 0 };	//셀의 위치(왼쪽 아래 모서리부터) & 채워졌는 지 유무
	int index;	//담고있는 도형의 인덱스 넘버
	GLfloat gg_x, gg_y;	//게임오버시 움직이기 위해
	GLfloat gg_dx, gg_dy;
	
}Cell;

Cell cell[24];

//경로 
#define line_l	250
GLfloat trace_s[line_l][3];
GLfloat trace_c[line_l][3];
GLuint trace_vao[Block_num], trace_vbo[Block_num][2];

//전역 변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader;

GLuint s_program;

GLfloat window_w = 700.0f;
GLfloat window_h = 800.0f;

GLenum mode = GL_TRIANGLES;	//Line/ Triangles

bool click = 0;	//마우스 클릭

bool pause = 0;
int speed = 50;
bool trace{0};	//경로 on/off
bool show_bb = 0;	//바운딩박스 보기

int fill_num{ 0 };	//셀이 체워진 수
bool game_over{ 0 };	
GLfloat gg_R{ 0 };

bool cell_click = 0;	//셀 칸 클릭
int cell_click_index = 0;	//클릭한 셀의 인덱스
int cell_click_follow = 0;	//오각형의 삼각형 사각형 따라가기
GLfloat move_x = 0.0f, move_y = 0.0f, move_z = 0.0f;	//모델 이동




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

void InitModel() {
	// [모델 위치 조정 & InitVao]
	// cell_vao[0]인 가장 왼쪽 모소리부터 시작
	GLfloat s[4][3];
	GLfloat cell_c[4][3] = {
		{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} // 하늘색
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

	//경로 색깔
	for (int i = 0; i < line_l; ++i) {
		trace_c[i][0] = 0; trace_c[i][1] = 1; trace_c[i][2] = 0;	//초록색으로
	}
}

void CreateBlock() {
	for (int j = 0; j < Block_num; ++j) {
		if (block[j].status == 0) {

			// [색 초기화]
			GLfloat r = uid_c(dre), g = uid_c(dre), b = uid_c(dre);
			for (int i = 0; i < 6; ++i) {
				block[j].c[i][0] = r; block[j].c[i][1] = g; block[j].c[i][2] = b;
			}

			// [모델 위치 조정]
			if (uid_s(dre) <= 0.1)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] < 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] < 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1번 점은 항상 3사분면에 있도록
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] > 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] < 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2번 점은 항상 4사분면에 있도록
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] > 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] > 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3번 점은 항상 1사분면에 있도록
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//사각형이 된다.0.5
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] < 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] > 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6번 점은 항상 2사분면에 있도록
				}
				else {	//삼각형이 된다.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}
			else if (uid_s(dre) <= 0.15)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] > 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] < 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1번 점은 항상 4사분면에 있도록
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] > 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] > 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2번 점은 항상 1사분면에 있도록
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] < 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] > 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3번 점은 항상 2사분면에 있도록
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//사각형이 된다.
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] < 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] < 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6번 점은 항상 3사분면에 있도록
				}
				else {	//삼각형이 된다.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}
			else if (uid_s(dre) <= 0.2)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] > 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] > 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1번 점은 항상 1사분면에 있도록
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] < 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] > 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2번 점은 항상 2사분면에 있도록
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] < 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] < 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3번 점은 항상 3사분면에 있도록
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//사각형이 된다.
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] > 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] < 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6번 점은 항상 4사분면에 있도록
				}
				else {	//삼각형이 된다.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}
			else if (uid_s(dre) <= 0.25)
			{
				block[j].s[0][0] = uid_s(dre); block[j].s[0][1] = uid_s(dre); block[j].s[0][2] = 0;
				block[j].s[0][0] = block[j].s[0][0] < 0 ? block[j].s[0][0] : block[j].s[0][0] * -1; block[j].s[0][1] = block[j].s[0][1] > 0 ? block[j].s[0][1] : block[j].s[0][1] * -1;	//1번 점은 항상 2사분면에 있도록
				block[j].s[1][0] = uid_s(dre); block[j].s[1][1] = uid_s(dre); block[j].s[1][2] = 0;
				block[j].s[1][0] = block[j].s[1][0] < 0 ? block[j].s[1][0] : block[j].s[1][0] * -1; block[j].s[1][1] = block[j].s[1][1] < 0 ? block[j].s[1][1] : block[j].s[1][1] * -1;	//2번 점은 항상 3사분면에 있도록
				block[j].s[2][0] = uid_s(dre); block[j].s[2][1] = uid_s(dre); block[j].s[2][2] = 0;
				block[j].s[2][0] = block[j].s[2][0] > 0 ? block[j].s[2][0] : block[j].s[2][0] * -1; block[j].s[2][1] = block[j].s[2][1] < 0 ? block[j].s[2][1] : block[j].s[2][1] * -1;	//3번 점은 항상 4사분면에 있도록
				block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;
				block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;
				if (uid_c(dre) >= 0.5) {	//사각형이 된다.
					block[j].s[5][0] = uid_s(dre); block[j].s[5][1] = uid_s(dre); block[j].s[5][2] = 0;
					block[j].s[5][0] = block[j].s[5][0] > 0 ? block[j].s[5][0] : block[j].s[5][0] * -1; block[j].s[5][1] = block[j].s[5][1] > 0 ? block[j].s[5][1] : block[j].s[5][1] * -1;	//6번 점은 항상 1사분면에 있도록
				}
				else {	//삼각형이 된다.
					block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;
				}
			}

			block[j].get_bb();

			// [화면좌표상 나타날 위치]
			if (uid_c(dre) < 0.5) {	//왼편에서
				block[j].m_x = -1.1; block[j].m_y = uid_sy(dre);
				// [속도 조정]
				block[j].m_dx = uid_dx(dre); block[j].m_dy = uid_dy(dre);
			}
			else {	//오른편
				block[j].m_x = 1.1; block[j].m_y = uid_sy(dre);
				// [속도 조정]
				block[j].m_dx = uid_dx(dre) * (-1); block[j].m_dy = uid_dy(dre);
			}

			// [바운더리 박스 주소 & 색상]
			GLfloat bb_s[4][3] = {
				{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
			};
			GLfloat bb_c[4][3] = {
				{1,1,1},{0,1,0},{1,0,0},{0,0,1}	
			};

			// [InitVao]
			InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
			InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스

			// status 변경
			block[j].status = 1;

			//경로 vao
			trace_s[0][0] = block[j].m_x; trace_s[0][1] = block[j].m_y; trace_s[0][2] = 0;

			GLfloat tdx = block[j].m_dx;
			GLfloat tdy = block[j].m_dy;

			for (int i = 1; i < line_l; ++i) {
				trace_s[i][0] = trace_s[i - 1][0] + tdx / 5;
				trace_s[i][1] = trace_s[i - 1][1] + tdy / 5;
				tdy -= 0.0004f;//중력

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

// 두 선분 교차 확인 함수
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

	glEnable(GL_DEPTH_TEST);	// 은면 제거


	unsigned int modelLoc = glGetUniformLocation(s_program, "model");	//--- 버텍스 세이더에서 모델링 변환 행렬 변수값을 받아온다.
	glm::mat4 mTransform = glm::mat4(1.0f);		  // 단위행렬로 초기화

	//차있는(빨간) 셀
	for (int i = 0; i < 24; ++i) {
		if (cell[i].fill_cell == 1) {

			mTransform = glm::mat4(1.0f);	//모델 변환
			if (game_over == 1)
				mTransform = glm::translate(mTransform, glm::vec3(cell[i].gg_x, cell[i].gg_y, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(cell[i].cell_vao);
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	}

	//빈(푸른) 셀
	for (int i = 0; i < 24; ++i) {
		if (cell[i].fill_cell == 0) {

			mTransform = glm::mat4(1.0f);	//모델 변환
			if (game_over == 1)
				mTransform = glm::translate(mTransform, glm::vec3(cell[i].gg_x, cell[i].gg_y, 0.0));
			modelLoc = glGetUniformLocation(s_program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &mTransform[0][0]);

			glBindVertexArray(cell[i].cell_vao);
			glDrawArrays(GL_LINE_LOOP, 0, 4);
		}
	}

	//마우스 슬라이스
	glBindVertexArray(mouse_vao);
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	//바운딩 박스 표시
	if (show_bb == 1) {
		for (int i = 0; i < Block_num; ++i) {
			if (block[i].status != 0 && block[i].status != 6) {
				mTransform = glm::mat4(1.0f);	//모델 변환
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

	//블록
	for (int i = 0; i < Block_num; ++i) {
		if (block[i].status != 0) {
			mTransform = glm::mat4(1.0f);	//모델 변환
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
		//경로
		if (trace == 1 && block[i].status == 1 && game_over == 0) {
			mTransform = glm::mat4(1.0f);	//모델 변환
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
			block[i].m_dy -= 0.002;	//중력
		}
	}

	for (int i = 0; i < 24; ++i) {
		cell[i].gg_x += cell[i].gg_dx;
		cell[i].gg_y += cell[i].gg_dy;
		cell[i].gg_dy -= 0.002;//중력
	}

	++value;

	gg_R += 5;

	cout << (80 - value) + 1 << "ms left"<< endl;
	
	if (value == 80)	//4초 후 종료
		exit(0);

	glutPostRedisplay();
	glutTimerFunc(50, Timer_Game_Over, value);
}

void Timer_Block(int value) {
	if (pause == 0 && game_over != 1) {
		if (value == 1) {
			++value;
			PlaySound(L"배경음악.wav", 0, SND_FILENAME | SND_ASYNC | SND_LOOP);	//배경 노래

			glutPostRedisplay();
			glutTimerFunc(8000, Timer_Block, value);
		}
		else {
			++value;
			if (value == MAXINT)
				value = 2;

			//새로운 친구들 생성
			if (value % 25 == 0)
				CreateBlock();

			//중복체크해서 크기0,0인 친구 삭제 && 크기 너무 작은 친구 삭제
			for (int i = 0; i < Block_num; ++i) {
				if (block[i].status != 2) {	//셀에 들어간게 아니라면
					if (abs(block[i].s[0][0] - block[i].s[1][0]) <= 0.005 && abs(block[i].s[0][0] - block[i].s[2][0]) <= 0.005 && abs(block[i].s[1][0] - block[i].s[2][0]) <= 0.005)
						block[i].status = 0;
					else if (abs(block[i].s[1][0] - block[i].s[2][0]) <= 0.005 && abs(block[i].s[1][1] - block[i].s[2][1]) <= 0.005)
						block[i].status = 0;
					
				}
			}

			// 움직임
			for (int i = 0; i < Block_num; ++i) {
				if (block[i].status == 1 || block[i].status == 5 || block[i].status == 6) {	//날아가는 중 && 오각형
					block[i].m_x += block[i].m_dx; block[i].m_y += block[i].m_dy;
					block[i].m_dy -= 0.002;	//중력

					if (block[i].m_x >= 1.2 || block[i].m_x <= -1.2 || block[i].m_y <= -1.5)	//화면에서 나감
						block[i].status = 0;
				}
			}

			//셀 충돌 검사
			for (int i = 0; i < Block_num; ++i) {
				if ((block[i].status == 1 || block[i].status == 5) && (block[i].b + block[i].m_y) <= -0.4f) {
					for (int j = 0; j < 24; ++j) {
						//셀과 충돌시
						if (cell[j].fill_cell == 0 && abs(block[i].s[0][0]- block[i].s[1][0]) >= 0.005 && abs(block[i].s[0][0] - block[i].s[2][0]) >= 0.005 && abs(block[i].s[1][0] - block[i].s[2][0]) >= 0.005
							&& block[i].b + block[i].m_y >= cell[j].s[0][1] && block[i].b + block[i].m_y <= cell[j].s[0][1] +0.1
							&& (block[i].l + block[i].m_x + block[i].r + block[i].m_x) / 2.0f >= cell[j].s[0][0] && (block[i].l + block[i].m_x + block[i].r + block[i].m_x) / 2.0f <= cell[j].s[1][0])
						{
							++fill_num; --speed;
							if (speed <= 6)
								speed = 5;

							cout << 24 - fill_num << "- left" << endl;

							//오각형이였을 경우 삼각형 같이 변환
							if (block[i].status == 5) {
								int n = block[i].hexa_tri_index;
								
								//위치 조정
								float tmp1 = block[i].l + block[i].r;
								if (tmp1 < 0)
									block[n].m_x = cell[j].s[0][0] + 0.125 + abs(tmp1) / 2.0f;
								else
									block[n].m_x = cell[j].s[0][0] + 0.125 - abs(tmp1) / 2.0f;

								//사이즈 조절
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

							//위치 조정
							float tmp1 = block[i].l + block[i].r;
							if (tmp1 < 0)
								block[i].m_x = cell[j].s[0][0] + 0.125 + abs(tmp1) / 2.0f;
							else
								block[i].m_x = cell[j].s[0][0] + 0.125 - abs(tmp1) / 2.0f;

							//사이즈 조절
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
							{0.31,0.73,0.87},{0.31,0.73,0.87} ,{1,0,0},{1,0,0} // 위쪽이 빨간색
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

			//종료
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
	if (/*pause == 0 &&*/ b == GLUT_LEFT_BUTTON && s == GLUT_DOWN && y <= 560) {	// 슬라이스 시작
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
		Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

		mouse_s[0][0] = X; mouse_s[0][1] = Y; mouse_s[0][2] = 0;
		mouse_s[1][0] = X; mouse_s[1][1] = Y; mouse_s[1][2] = 0;

		mouse_c[0][0] = 1; mouse_c[0][1] = 1; mouse_c[0][2] = 1;	//흰색
		mouse_c[1][0] = 1; mouse_c[1][1] = 1; mouse_c[1][2] = 1;
		InitVao(mouse_vao, mouse_vbo, 2, mouse_s, mouse_c);
		click = 1;
	}
	if (click == 1 && b == GLUT_LEFT_BUTTON && s == GLUT_UP && y <= 560) {	// 슬라이스 끝
		click = 0;
		int t{ 0 }; //오각형 만들때 사각형의 인덱스 정보가 필요

		for (int i = 0; i < Block_num; ++i) {	// 슬라이스 확인		
			if (block[i].status == 1) {

				GLfloat o1[2] = { block[i].s[0][0] + block[i].m_x,block[i].s[0][1] + block[i].m_y };
				GLfloat o2[2] = { block[i].s[1][0] + block[i].m_x,block[i].s[1][1] + block[i].m_y };
				GLfloat o3[2] = { block[i].s[2][0] + block[i].m_x,block[i].s[2][1] + block[i].m_y };
				GLfloat o6[2] = { block[i].s[5][0] + block[i].m_x,block[i].s[5][1] + block[i].m_y };

				GLfloat IP[3];
				GLfloat SliceP1[3], SliceP2[3];
				int num = 0;
				bool line1to2{ false }, line1to3{ false }, line1to6{ false }, line2to3{ false }, line3to6{ false };

				//삼각형 충돌검사
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
				//사각형 충돌검사
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

				if (num == 2) {	// 나누기
					if (block[i].s[5][0] == block[i].s[0][0])	//삼각형
					{
						if (line1to2 == 1 && line1to3 == 1)	//1-2,1-3을 자를시
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//자식 사각형 만들기
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//움직임 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}
							//삼각형 떨어짐 (부모 삼각형->자식)
							block[i].s[1][0] = SliceP1[0] - block[i].m_x; block[i].s[1][1] = SliceP1[1] - block[i].m_y;
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
						else if (line1to3 == 1 && line2to3 == 1)	//1-3, 2-3을 자를시
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//자식 사각형 만들기
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[0][0]; block[j].s[1][1] = block[i].s[0][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[1][0]; block[j].s[2][1] = block[i].s[1][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//움직임 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}
							//삼각형 떨어짐 (부모 삼각형->자식)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[1][0] = SliceP2[0] - block[i].m_x; block[i].s[1][1] = SliceP2[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
						else if (line1to2 == 1 && line2to3 == 1)	//1-2,2-3을 자를시
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//자식 사각형 만들기
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[0][0]; block[j].s[1][1] = block[i].s[0][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//움직임 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}
							//삼각형 떨어짐 (부모 삼각형->자식)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
					}
					else			//사각형
					{
						//사각형 두개로 잘릴때
						if (line1to2 == 1 && line3to6 == 1)	//1-2,3-6을 자를시
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//자식 사각형 만들기
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP2[0] - block[i].m_x; block[j].s[0][1] = SliceP2[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[5][0]; block[j].s[1][1] = block[i].s[5][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[0][0]; block[j].s[2][1] = block[i].s[0][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP1[0] - block[i].m_x; block[j].s[5][1] = SliceP1[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//움직임 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}
							//사각형 떨어짐 (부모 사각형->자식)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[5][0] = SliceP2[0] - block[i].m_x; block[i].s[5][1] = SliceP2[1] - block[i].m_y;

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
						else if (line1to6 == 1 && line2to3 == 1)	//1-6,2-3을 자를시
						{
							for (int j = Block_num - 1; j >= 0; --j) {	//자식 사각형 만들기
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[0][0]; block[j].s[1][1] = block[i].s[0][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[1][0]; block[j].s[2][1] = block[i].s[1][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 1;//움직임 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}
							//사각형 떨어짐 (부모 사각형->자식)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];
							block[i].s[1][0] = SliceP2[0] - block[i].m_x; block[i].s[1][1] = SliceP2[1] - block[i].m_y;

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
						//사각형 과 삼각형으로 잘릴때
						else if (line1to6 == 1 && line3to6 == 1)	//1-6,3-6을 자를시
						{	//자식 오각형 만들기
							for (int j = Block_num - 1; j >= 0; --j) {
								//사각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = SliceP2[0] - block[i].m_x; block[j].s[5][1] = SliceP2[1] - block[i].m_y; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//오각형이므로 바운딩 박스 임의로 늘림
									t = j;

									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//삼각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP1[0] - block[i].m_x; block[j].s[2][1] = SliceP1[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//속도 사각형과 맞춤

									//바운딩 박스
									//block[j].l = block[t].l; block[j].r = block[t].r; block[j].b = block[t].b; block[j].u = block[t].u;	//위 사각형의 바운더리 박스를 따라감

									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[t].hexa_tri_index = j;	//삼각형 사각형에 종속시킴
									block[j].status = 6;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									//InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}
							//삼각형 떨어짐 (부모 사각형->자식 삼각형)
							block[i].s[0][0] = block[i].s[5][0]; block[i].s[0][1] = block[i].s[5][1];
							block[i].s[1][0] = SliceP1[0] - block[i].m_x; block[i].s[1][1] = SliceP1[1] - block[i].m_y;
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
						else if (line2to3 == 1 && line3to6 == 1)	//2-3,3-6을 자를시
						{	//자식 오각형 만들기
							for (int j = Block_num - 1; j >= 0; --j) {
								//사각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = SliceP1[0] - block[i].m_x; block[j].s[1][1] = SliceP1[1] - block[i].m_y; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP2[0] - block[i].m_x; block[j].s[2][1] = SliceP2[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[i].s[5][0]; block[j].s[5][1] = block[i].s[5][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//오각형이므로 바운딩 박스 임의로 늘림
									t = j;

									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//삼각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP1[0] - block[i].m_x; block[j].s[2][1] = SliceP1[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//속도 사각형과 맞춤

									block[t].hexa_tri_index = j;	//삼각형 사각형에 종속시킴
									block[j].status = 6;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									break;
								}
							}
							//삼각형 떨어짐 (부모 사각형->자식 삼각형)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;
							block[i].s[1][0] = SliceP2[0] - block[i].m_x; block[i].s[1][1] = SliceP2[1] - block[i].m_y;
							block[i].s[2][0]; block[i].s[2][1];
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
						else if (line1to2 == 1 && line2to3 == 1)	//1-2,2-3을 자를시
						{	//자식 오각형 만들기
							for (int j = Block_num - 1; j >= 0; --j) {
								//사각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = SliceP2[0] - block[i].m_x; block[j].s[1][1] = SliceP2[1] - block[i].m_y; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[i].s[5][0]; block[j].s[5][1] = block[i].s[5][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//오각형이므로 바운딩 박스 임의로 늘림
									t = j;

									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//삼각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = block[i].s[0][0]; block[j].s[0][1] = block[i].s[0][1]; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = SliceP1[0] - block[i].m_x; block[j].s[1][1] = SliceP1[1] - block[i].m_y; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[5][0]; block[j].s[2][1] = block[i].s[5][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//속도 사각형과 맞춤

									block[t].hexa_tri_index = j;	//삼각형 사각형에 종속시킴
									block[j].status = 6;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									break;
								}
							}
							//삼각형 떨어짐 (부모 사각형->자식 삼각형)
							block[i].s[0][0] = SliceP1[0] - block[i].m_x; block[i].s[0][1] = SliceP1[1] - block[i].m_y;	//1
							block[i].s[1][0]; block[i].s[1][1]; 	//2
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;	//3
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
						else if (line1to2 == 1 && line1to6 == 1)	//1-2,1-6을 자를시
						{	//자식 오각형 만들기
							for (int j = Block_num - 1; j >= 0; --j) {
								//사각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP2[0] - block[i].m_x; block[j].s[0][1] = SliceP2[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = block[i].s[2][0]; block[j].s[2][1] = block[i].s[2][1]; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[i].s[5][0]; block[j].s[5][1] = block[i].s[5][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = uid_dfx(dre); block[j].m_dy = uid_dfy(dre);	//속도 다시 설정

									//바운딩 박스
									block[j].get_bb();
									block[j].l -= 0.01; block[j].r += 0.01; block[j].b -= 0.01; block[j].u += 0.01;	//오각형이므로 바운딩 박스 임의로 늘림
									t = j;

									// [바운더리 박스 주소 & 색상]
									GLfloat bb_s[4][3] = {
										{block[j].r,block[j].u,0},{block[j].l,block[j].u,0},{block[j].l,block[j].b,0},{block[j].r,block[j].b,0}
									};
									GLfloat bb_c[4][3] = {
										{1,1,1},{0,1,0},{1,0,0},{0,0,1}
									};

									block[j].status = 5;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									InitVao(bb_vao[j], bb_vbo[j], 4, bb_s, bb_c);	//바운딩 박스
									break;
								}
							}

							for (int j = Block_num - 1; j >= 0; --j) {
								//삼각형
								if (block[j].status == 0) {
									// 모델 위치 지정
									block[j].s[0][0] = SliceP1[0] - block[i].m_x; block[j].s[0][1] = SliceP1[1] - block[i].m_y; block[j].s[0][2] = 0;	//1
									block[j].s[1][0] = block[i].s[1][0]; block[j].s[1][1] = block[i].s[1][1]; block[j].s[1][2] = 0;	//2
									block[j].s[2][0] = SliceP2[0] - block[i].m_x; block[j].s[2][1] = SliceP2[1] - block[i].m_y; block[j].s[2][2] = 0;	//3
									block[j].s[3][0] = block[j].s[0][0]; block[j].s[3][1] = block[j].s[0][1]; block[j].s[3][2] = 0;	//4
									block[j].s[4][0] = block[j].s[2][0]; block[j].s[4][1] = block[j].s[2][1]; block[j].s[4][2] = 0;	//5
									block[j].s[5][0] = block[j].s[0][0]; block[j].s[5][1] = block[j].s[0][1]; block[j].s[5][2] = 0;	//6

									block[j].m_x = block[i].m_x; block[j].m_y = block[i].m_y;	//화면상 위치 부모로 물려받음

									for (int n = 0; n < 6; ++n) {
										block[j].c[n][0] = block[i].c[n][0]; block[j].c[n][1] = block[i].c[n][1]; block[j].c[n][2] = block[i].c[n][2];//색은 부모로부터 물려받는다.
									}

									block[j].m_dx = block[t].m_dx; block[j].m_dy = block[t].m_dy;	//속도 사각형과 맞춤

									block[t].hexa_tri_index = j;	//삼각형 사각형에 종속시킴
									block[j].status = 6;//오각형 활성화

									InitVao(block[j].vao, block[j].vbo, 6, block[j].s, block[j].c);
									break;
								}
							}
							//삼각형 떨어짐 (부모 사각형->자식 삼각형)
							block[i].s[0][0]; block[i].s[0][1];	//1
							block[i].s[1][0] = SliceP1[0] - block[i].m_x; block[i].s[1][1] = SliceP1[1] - block[i].m_y; 	//2
							block[i].s[2][0] = SliceP2[0] - block[i].m_x; block[i].s[2][1] = SliceP2[1] - block[i].m_y;	//3
							block[i].s[3][0] = block[i].s[0][0]; block[i].s[3][1] = block[i].s[0][1];	//4
							block[i].s[4][0] = block[i].s[2][0]; block[i].s[4][1] = block[i].s[2][1];	//5
							block[i].s[5][0] = block[i].s[0][0]; block[i].s[5][1] = block[i].s[0][1];	//6

							block[i].m_dx = uid_dfx(dre); block[i].m_dy = uid_dfy(dre);	//속도 다시 설정

							InitVao(block[i].vao, block[i].vbo, 6, block[i].s, block[i].c);	//vao업데이트

							block[i].get_bb();	//바운드 박스 다시 설정
							// [바운더리 박스 주소 & 색상]
							GLfloat bb_s[4][3] = {
								{block[i].r,block[i].u,0},{block[i].l,block[i].u,0},{block[i].l,block[i].b,0},{block[i].r,block[i].b,0}
							};
							GLfloat bb_c[4][3] = {
								{1,1,1},{0,1,0},{1,0,0},{0,0,1}
							};
							InitVao(bb_vao[i], bb_vbo[i], 4, bb_s, bb_c);	//바운딩 박스
						}
					}



					mouse_c[0][0] = 1; mouse_c[0][1] = 0; mouse_c[0][2] = 0;	//성공시 빨간색
					mouse_c[1][0] = 1; mouse_c[1][1] = 0; mouse_c[1][2] = 0;
					InitVao(mouse_vao, mouse_vbo, 2, mouse_s, mouse_c);

					//break; //=> 중복체크 해제 ( break하면 한번에 여러번 썰기가 불가함, 대신 중복체크는 안함)
				}

			}
		}
	}
	else if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN && y > 560) {	// 채워진 셀 클릭
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
		Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

		for (int i = 0; i < 24; ++i) {
			if (cell[i].fill_cell == 1 && X >= cell[i].s[0][0] && X <= cell[i].s[1][0] && Y >= cell[i].s[0][1] && Y <= cell[i].s[2][1]) {
				move_x = X; move_y = Y;

				cell_click_index = i;

				if (block[cell[i].index].hexa_tri_index != Block_num) {//오각형 - 삼각형을 위해
					cell_click_follow = block[cell[i].index].hexa_tri_index;
					block[cell_click_follow].hexa_tri_index = cell_click_follow;	//옮기기에서 삼각형 오류를 고치기 위해(드로우 함수에서 검사)
				}
				else
					cell_click_follow = cell[i].index;

				cell_click = 1;
				break;
			}
		}
	}
	else if (b == GLUT_LEFT_BUTTON && s == GLUT_UP && y > 560 && cell_click == 1) {	//셀 위 도형 움직이기 멈춤
		GLfloat X, Y;

		X = static_cast<GLfloat>(x - (window_w / 2.0)) / (window_w / 2.0);
		Y = static_cast<GLfloat>((window_h / 2.0) - y) / (window_h / 2.0);

		for (int i = 0; i < 24; ++i) {
			if (cell[i].fill_cell == 0 && X >= cell[i].s[0][0] && X <= cell[i].s[1][0] && Y >= cell[i].s[0][1] && Y <= cell[i].s[2][1]) {
				cell[i].index = cell[cell_click_index].index;
				int num = cell[i].index;

				//위치 조정
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

				if (block[num].hexa_tri_index != Block_num) {//오각형의 삼각형 위치 변환
					int tri_n = block[num].hexa_tri_index;

					block[tri_n].m_x = block[num].m_x;
					block[tri_n].m_y = block[num].m_y;
				}

				cell[cell_click_index].fill_cell = 0;
				cell[i].fill_cell = 1;

				GLfloat cell_empty[4][3] = {
				{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{0.31,0.73,0.87} // 하늘색
				};
				GLfloat cell_fill[4][3] = {
				{0.31,0.73,0.87} ,{0.31,0.73,0.87} ,{1,0,0}  ,{1,0,0} // 위만 빨간색
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

			if (sqrt(tx + ty) <= 0.6) {					//길이 0.6 제한
				mouse_s[1][0] = X; mouse_s[1][1] = Y;
			}

			InitVao(mouse_vao, mouse_vbo, 2, mouse_s, mouse_c);
		}
		cell_click = 0;

		glutPostRedisplay();
	}
	else if (y > 560 && cell_click == 1)	//셀 위 도형 움직이기
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
		//강제로 끝내기
	case 'E':
		fill_num = 24;
		break;
		//일시정지
	case 'p':
	case 'P':
		if (pause == 0)
			pause = 1;
		else {
			pause = 0;
			glutTimerFunc(0, Timer_Block, 2);
		}
		break;
		//스피드
	case '+':	//빠르게!
		--speed;
		if (speed <= 6)
			speed = 5;
		break;
	case '-':	//느리게!
		++speed;
		if (speed >= 61)
			speed = 60;
		break;
		// 라인 또는 삼각형
	case 'l':
	case 'L':
		if (mode == GL_TRIANGLES)
			mode = GL_LINE_LOOP;
		else mode = GL_TRIANGLES;
		break;
		//경로 표시
	case 't':
	case 'T':
		if (trace == 0)
			trace = 1;
		else trace = 0;
		break;
		//바운딩 박스 보기
	case 'b':
	case 'B':
		if (show_bb == 0)
			show_bb = 1;
		else show_bb = 0;
		break;
		//초기 설정값으로
	case 'i':
	case 'I':
		break;
		//종료
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