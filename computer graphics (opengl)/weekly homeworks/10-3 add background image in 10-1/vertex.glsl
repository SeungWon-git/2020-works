#version 330 core

layout (location = 0) in vec3 in_Position;	//--- 객체의 좌표값
layout (location = 1) in vec3 in_Normal;	//--- 법선 속성
layout (location = 2) in vec2 in_TexCoord;	//---텍스처 좌표

// 프래그먼트 세이더에게 전달
out vec3 ex_Normal; // 법선
out vec3 ex_FragPos;// 객체의 위치값을 프래그먼트 세이더에게 전달
out vec2 ex_TexCoord;// 텍스쳐 좌표

uniform mat4 model;				//--- 모델링 변환값: 응용 프로그램에서 전달 -> uniform 변수로 선언: 변수 이름“model”로 받아옴
uniform mat4 view;				//--- 뷰잉 변환값: 응용 프로그램에서 전달 -> uniform 변수로 선언: 변수 이름“view”로 받아옴
uniform mat4 projection;		//--- 투영 변환값: 응용 프로그램에서 전달 -> uniform 변수로 선언: 변수 이름“projection”로 받아옴

void main()
{
	gl_Position = projection *view *model *vec4 (in_Position, 1.0f); //--- 객체의 좌표에 변환 행렬을 적용한다.
																	//--- 변환은 ← 방향으로 적용
																	//--- in_Position: 객체의 원래 좌표값
	
	ex_FragPos = vec3(model *vec4(in_Position,1.0));						//--- 객체에 대한 조명 계산을 월드공간에서 한다.
																	//--- 따라서 월드공간에 있는 버텍스 값을 프래그먼트 셰이더로 보낸다.
	
	ex_Normal = in_Normal;											//--- 노멀값을 프래그먼트 세이더로 보낸다.

	ex_TexCoord = in_TexCoord;
} 