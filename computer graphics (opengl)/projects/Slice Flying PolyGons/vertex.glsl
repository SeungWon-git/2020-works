#version 330 core

layout (location = 0) in vec3 in_Position; //--- ��ü�� ��ǥ��
layout (location = 1) in vec3 in_Color; //--- ���� �Ӽ�

out vec3 ex_Color; // �����׸�Ʈ ���̴����� ���� 

uniform mat4 model;				//--- �𵨸� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���model���� �޾ƿ�

void main()
{
	gl_Position = model *vec4 (in_Position, 1.0f);	//--- ��ü�� ��ǥ�� ��ȯ ����� �����Ѵ�.
													//--- ��ȯ�� �� �������� ����
													//--- in_Position: ��ü�� ���� ��ǥ��
	ex_Color = in_Color;
} 