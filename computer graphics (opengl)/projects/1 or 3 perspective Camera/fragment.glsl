#version 330 core

//--- ex_Normal: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- gl_FragColor: ����� ������ ������ ���� ���α׷����� ���� ��.

in vec3 ex_Normal; //--- ���ؽ� ���̴����Լ� ���� ����
in vec2 ex_TexCoord;

out vec4 gl_FragColor; //--- ���� ���

uniform sampler2D outTexture;

void main(void)
{
	gl_FragColor = texture(outTexture,ex_TexCoord);
	
	//���� �����
	//gl_FragColor = vec4 (ex_Normal,1.0f);
	//gl_FragColor = texture(outTexture,ex_TexCoord)*gl_FragColor;
}