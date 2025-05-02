#version 410 core
layout (location=0) in vec3 vp;
layout (location=1) in vec3 aNormal;

out vec3 Normal;
uniform mat3 normalMatrix;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() 
{
	Normal = aNormal * normalMatrix;
	FragPos = vec3(vec4(vp, 1.0) * model);
	gl_Position = projection * view * model * vec4(vp, 1.0);
}