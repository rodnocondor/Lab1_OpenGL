#version 410 core
uniform vec4 ourColor;
uniform vec3 lightColor;
out vec4 Colour;
void main() 
{
	Colour = ourColor;
}