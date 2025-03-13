#define GLEW_DLL
#define GLFW_DLL

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include "shader_loader.h"

GLfloat points[] =
{
	0, 0.5, 0.0,
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
};

GLuint index[] = { 0, 1, 2 };


int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;

	window = glfwCreateWindow(512, 512, "Triangle", NULL, NULL);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	glewInit();

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	GLuint shader_program = load_shader_program("assets\\shader\\vert_shader.vs", "assets\\shader\\frag_shader.fs");

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.3, 0.4, 1);
		float timeValue = glfwGetTime();
		float Red = abs(sin(timeValue));
		float Green = abs(sin(timeValue * 2.0f));
		float Blue = abs(sin(timeValue * 1.5f));

		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		
		uni4(shader_program, "ourColor", Red, Green, Blue, 1.0);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
}

