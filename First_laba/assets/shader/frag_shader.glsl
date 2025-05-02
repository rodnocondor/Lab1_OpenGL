#version 410 core

uniform vec4 lightColor;
uniform vec3 viewPos;

out vec4 Colour;
in vec3 Normal;
in vec3 FragPos;

struct lightStruct
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

uniform lightStruct light_1;

struct MatStruct
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 shininess;
};

uniform MatStruct mat_1;
void main() 
{
	//ambient
	vec3 ambient = light_1.ambient * mat_1.ambient;

	//difuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_1.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light_1.diffuse * (diff * mat_1.diffuse);

	//specular
	vec3 viewDir = normalize(viewPos);
	vec3 ref = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, ref), 0.0), 32.0);
	vec3 specular = light_1.specular * (spec * mat_1.specular);


	vec3 result = (ambient + diffuse + specular);
	Colour = vec4(result, 1.0);
}