#version 330 core

//--- ex_Normal: 버텍스 세이더에서 입력받는 법선 값
//--- gl_FragColor: 출력할 색상의 값으로 응용 프로그램으로 전달 됨.

in vec3 ex_Normal; //--- 버텍스 세이더에게서 전달 받음
in vec2 ex_TexCoord;

out vec4 gl_FragColor; //--- 색상 출력

uniform sampler2D outTexture;

void main(void)
{
	gl_FragColor = texture(outTexture,ex_TexCoord);
	
	//조명 적용시
	//gl_FragColor = vec4 (ex_Normal,1.0f);
	//gl_FragColor = texture(outTexture,ex_TexCoord)*gl_FragColor;
}