#version 330 core

layout (location = 0) in vec3 in_Position;	//--- ��ü�� ��ǥ��
layout (location = 1) in vec3 in_Normal;	//--- ���� �Ӽ�
layout (location = 2) in vec2 in_TexCoord;	//---�ؽ�ó ��ǥ

// �����׸�Ʈ ���̴����� ����
out vec3 ex_Normal; // ����
out vec3 ex_FragPos;// ��ü�� ��ġ���� �����׸�Ʈ ���̴����� ����
out vec2 ex_TexCoord;// �ؽ��� ��ǥ

uniform mat4 model;				//--- �𵨸� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���model���� �޾ƿ�
uniform mat4 view;				//--- ���� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���view���� �޾ƿ�
uniform mat4 projection;		//--- ���� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���projection���� �޾ƿ�

void main()
{
	gl_Position = projection *view *model *vec4 (in_Position, 1.0f); //--- ��ü�� ��ǥ�� ��ȯ ����� �����Ѵ�.
																	//--- ��ȯ�� �� �������� ����
																	//--- in_Position: ��ü�� ���� ��ǥ��
	
	ex_FragPos = vec3(model *vec4(in_Position,1.0));						//--- ��ü�� ���� ���� ����� ����������� �Ѵ�.
																	//--- ���� ��������� �ִ� ���ؽ� ���� �����׸�Ʈ ���̴��� ������.
	
	ex_Normal = in_Normal;											//--- ��ְ��� �����׸�Ʈ ���̴��� ������.

	ex_TexCoord = in_TexCoord;
} 