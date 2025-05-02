#define GLEW_DLL
#define GLFW_DLL

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh.h"
#include "Model.h"
#include "shader_loader.h"
#include "funcs.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;


int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window;

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Arm", NULL, NULL);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	glewInit();


	GLuint shader_program = load_shader_program("assets\\shader\\vert_shader.glsl", "assets\\shader\\frag_shader.glsl");

	Model Arm("assets\\model\\model.obj");


	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	while (!glfwWindowShouldClose(window))
	{
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2, 0.3, 0.4, 1);


		glUseProgram(shader_program);

		settingAllMatrix(shader_program);


		Arm.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
}

