#version 410 core
layout (location=0) in vec3 vp;
layout (location=1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() 
{
	FragPos = vec3(model * vec4(vp, 1.0));
	
	Normal = aNormal * mat3(transpose(inverse(model)));
	
	gl_Position = projection * view * vec4(FragPos, 1.0);
}