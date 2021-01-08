#version 330 core

layout (location = 0) in vec3 vPos; //--- ��ü�� ��ǥ��
layout (location = 1) in vec3 vNormal; //--- ��� �Ӽ�

out vec3 FragPos;	// ��ü�� ��ġ���� �����׸�Ʈ ���̴����� ���� 
out vec3 Normal;	// ��ְ��� �����׸�Ʈ ���̴��� ����	

uniform mat4 model;				//--- �𵨸� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���model���� �޾ƿ�
uniform mat4 view;				//--- ���� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���view���� �޾ƿ�
uniform mat4 projection;		//--- ���� ��ȯ��: ���� ���α׷����� ���� -> uniform ������ ����: ���� �̸���projection���� �޾ƿ�

void main()
{
	gl_Position = projection *view *model *vec4 (vPos, 1.0f);	//--- ��ü�� ��ǥ�� ��ȯ ����� �����Ѵ�.
																//--- ��ȯ�� �� �������� ����
																//--- in_Position: ��ü�� ���� ��ǥ��
	
	FragPos = vec3(model *vec4(vPos,1.0));						//--- ��ü�� ���� ���� ����� ����������� �Ѵ�.
																//--- ���� ��������� �ִ� ���ؽ� ���� �����׸�Ʈ ���̴��� ������.
	
	Normal = vNormal;											//--- ��ְ��� �����׸�Ʈ ���̴��� ������.
} 