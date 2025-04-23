#define GLEW_DLL
#define GLFW_DLL

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh.h"
#include "Model.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include "shader_loader.h"

using namespace std;

GLfloat points[] =
{
	0, 0.5, 0.0,
	-0.5, -0.5, 0.0,
	0.5, -0.5, 0.0,
};

GLuint index[] = { 0, 1, 2 };
const unsigned int WIN_WIDTH = 1024;
const unsigned int WIN_HEIGHT = 768;
float fov = 45.0f;
bool firstMouse = true;

float	lastx = WIN_WIDTH / 2, lasty = WIN_HEIGHT / 2;

GLfloat pitch = 0.0f, yaw = -90.0f;
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 5.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
bool firstmouse = true;



void scroll_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float ypos = static_cast<float>(yposIn);
	fov -= (float)ypos;
	if (fov < 1.0f) 
		fov = 1.0f;
	if (fov > 45.0f) 
		fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastx = xpos;
		lasty = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastx;
	float yoffset = lasty - ypos;

	lastx = xpos;
	lasty = ypos;

	float sensitivity = 0.08f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);


}

void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 0.005f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}





int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window;

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Triangle", NULL, NULL);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	glewInit();


	GLuint shader_program = load_shader_program("assets\\shader\\vert_shader.vs", "assets\\shader\\frag_shader.fs");

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

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		settingMat4(shader_program, "projection", projection);
		settingMat4(shader_program, "view", view);
		settingMat4(shader_program, "model", model);
		
		uni4(shader_program, "ourColor", 1.0f, 1.0f, 1.0f, 1.0);

		Arm.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
}

