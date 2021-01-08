#version 330 core

in vec3 in_Position; //--- ��ü�� ��ǥ��
in vec3 in_Color; //--- ���� �Ӽ�

out vec3 ex_Color; // �����׸�Ʈ ���̴����� ����

uniform mat4 modelTransform; // ---��ȯ ���: uniform���� �����Ͽ� ���� ���α׷����� ���� �����Ѵ�.

void main()
{
	gl_Position = modelTransform *vec4 (in_Position, 1.0f); //--- ��ü�� ��ǥ�� ��ȯ ����� �����Ѵ�.

	ex_Color = in_Color;
} 