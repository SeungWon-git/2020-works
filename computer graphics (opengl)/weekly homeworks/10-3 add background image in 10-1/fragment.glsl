#version 330 core

in vec3 ex_FragPos; //--- ��ְ��� ����ϱ� ���� ��ü�� ��ġ���� ���ؽ� ���̴����� �޾ƿ´�.
in vec3 ex_Normal;	//--- ���ؽ� ���̴����Լ� ���� ����
in vec2 ex_TexCoord;

out vec4 gl_FragColor; //--- ���� ���

uniform vec3 lightPos;			//--- ���� ���α׷����� ������ ������ ��ġ
uniform vec3 objectColor;		//--- ���� ���α׷����� ������ ��ü�� ����
uniform vec3 lightColor;		//--- ���� ���α׷����� ������ ���� ����
uniform sampler2D outTexture;	//--- ���� ���α׷����� ������ �ؽ�ó ����

void main(void)
{
	float ambientLight = 0.3;										//--- �ֺ� ���� ����
	vec3 ambient = ambientLight * lightColor;						//--- �ֺ� ���� ��

	vec3 normalVector = normalize (ex_Normal);
	vec3 lightDir = normalize (lightPos - ex_FragPos);				//--- ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�.
	float diffuseLight = max (dot (normalVector, lightDir), 0.0);	//--- N�� L�� ���� ������ ���� ����: ���� ����
	vec3 diffuse = diffuseLight * lightColor;						//--- ��� �ݻ� ����: ����ݻ簪 * �������
	
	int shininess = 128;											//--- ���� ���
	vec3 viewDir = normalize (lightPos - ex_FragPos);				//--- �������� ����
	vec3 reflectDir = reflect (-lightDir, normalVector);			//--- �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���
	float specularLight = max (dot (viewDir, reflectDir), 0.0);		//--- V�� R�� ���������� ���� ����: ���� ����
	specularLight = pow(specularLight, shininess);					//--- shininess ���� ���־� ���̶���Ʈ�� ������ش�.
	vec3 specular = specularLight * lightColor;						//--- �ſ� �ݻ� ����: �ſ�ݻ簪 * �������
	
	vec3 result = (ambient + diffuse + specular) * objectColor;		//--- ���� ���� ������ �ȼ� ����: (�ֺ�+����ݻ�+�ſ�ݻ�����)*��ü ����
	
	gl_FragColor = vec4 (result, 1.0);								// --- ���� ����
	
	gl_FragColor = texture(outTexture,ex_TexCoord)*gl_FragColor;	// --- �ؽ�ó ���� -> �ȼ� �� ���
}