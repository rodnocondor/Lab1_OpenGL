#define GLEW_DLL
#define GLFW_DLL

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
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
const unsigned int WIN_WIDTH = 1024;
const unsigned int WIN_HEIGHT = 768;

float	lastx = WIN_WIDTH / 2, lasty = WIN_HEIGHT / 2;

GLfloat pitch = -21.0f, yaw = -97.5f;
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 5.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
bool firstmouse = true;



void scroll_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float ypos = static_cast<float>(yposIn);
	float sensitivity = 0.5f; 
	cameraPos += cameraFront * ypos * sensitivity;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
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




void settingMat4(GLuint ID, const char* name, glm::mat4 type) 
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &type[0][0]);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	while (!glfwWindowShouldClose(window))
	{
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.3, 0.4, 1);
		float timeValue = glfwGetTime();
		float Red = abs(sin(timeValue));
		float Green = abs(sin(timeValue * 2.0f));
		float Blue = abs(sin(timeValue * 1.5f));

		glUseProgram(shader_program);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		settingMat4(shader_program, "projection", projection);
		settingMat4(shader_program, "view", view);

		glBindVertexArray(VAO);
		
		uni4(shader_program, "ourColor", Red, Green, Blue, 1.0);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
}

