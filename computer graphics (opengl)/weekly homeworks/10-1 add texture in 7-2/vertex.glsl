#version 330 core

layout (location = 0) in vec3 in_Position; //--- ��ü�� ��ǥ��
layout (location = 1) in vec3 in_Normal; //--- ���� �Ӽ�
layout (location = 2) in vec2 in_TexCoord;	//---�ؽ�ó ��ǥ

out vec3 ex_Normal; // �����׸�Ʈ ���̴����� ����
out vec2 ex_TexCoord;

uniform mat4 model;				//--- �𵨸� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���model���� �޾ƿ�
uniform mat4 view;				//--- ���� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���view���� �޾ƿ�
uniform mat4 projection;		//--- ���� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���projection���� �޾ƿ�

void main()
{
	gl_Position = projection *view *model *vec4 (in_Position, 1.0f); //--- ��ü�� ��ǥ�� ��ȯ ����� �����Ѵ�.
																	//--- ��ȯ�� �� �������� ����
																	//--- in_Position: ��ü�� ���� ��ǥ��
	ex_Normal = in_Normal;
	ex_TexCoord = in_TexCoord;
} 