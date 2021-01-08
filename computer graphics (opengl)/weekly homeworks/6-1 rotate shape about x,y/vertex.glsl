#version 330 core

in vec3 in_Position; //--- 객체의 좌표값
in vec3 in_Color; //--- 색상 속성

out vec3 ex_Color; // 프래그먼트 세이더에게 전달

uniform mat4 modelTransform; // ---변환 행렬: uniform으로 선언하여 응용 프로그램에서 값을 저장한다.

void main()
{
	gl_Position = modelTransform *vec4 (in_Position, 1.0f); //--- 객체의 좌표에 변환 행렬을 적용한다.

	ex_Color = in_Color;
} 