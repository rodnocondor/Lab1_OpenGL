#pragma once
#define GLEW_DLL
#define GLFW_DLL

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;

float fov = 45.0f;

const unsigned int WIN_WIDTH = 1024;
const unsigned int WIN_HEIGHT = 768;

bool firstMouse = true;

float	lastx = WIN_WIDTH / 2, lasty = WIN_HEIGHT / 2;

GLfloat pitch = 0.0f, yaw = -90.0f;
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 5.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
bool firstmouse = true;

void uni4(GLuint program, const string& nameColor, float red, float green, float blue, float alpha)
{
	glUniform4f(glGetUniformLocation(program, nameColor.c_str()), red, green, blue, alpha);
}

void settingVec3(GLuint program, const string& name, glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void settingMat4(GLuint ID, const char* name, glm::mat4 type)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &type[0][0]);
}

void settingMat3(GLuint ID, const char* name, glm::mat4 type)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &type[0][0]);
}



void settingAllMatrix(GLuint shader_program)
{
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	settingMat4(shader_program, "projection", projection);
	settingMat4(shader_program, "view", view);
	settingMat4(shader_program, "model", model);

	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// матрицы
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	settingMat3(shader_program, "normalMatrix", normalMatrix);

	//  свет
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambient = lightColor * glm::vec3(0.1);
	glm::vec3 diffuse = lightColor * glm::vec3(0.7);
	glm::vec3 specular = lightColor;
	glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f);
	//пересылка света
	settingVec3(shader_program, "light_1.ambient", ambient);
	settingVec3(shader_program, "light_1.diffuse", diffuse);
	settingVec3(shader_program, "light_1.specular", specular);
	settingVec3(shader_program, "light_1.position", position);

	// материал
	glm::vec3 matambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 matdiffuse = glm::vec3(0.1f, 0.35f, 0.1f);
	glm::vec3 matspecular = glm::vec3(0.45f, 0.55f, 0.45f);
	glm::vec3 matshininess = glm::vec3(32.0f);
	//пересылка материала
	settingVec3(shader_program, "mat_1.ambient", matambient);
	settingVec3(shader_program, "mat_1.diffuse", matdiffuse);
	settingVec3(shader_program, "mat_1.specular", matspecular);
	settingVec3(shader_program, "mat_1.shininess", matshininess);

	//позиция

	settingVec3(shader_program, "viewPos", cameraPos);

}



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

