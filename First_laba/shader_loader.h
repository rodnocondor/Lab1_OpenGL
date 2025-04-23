#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

using namespace std;

string load_data_from_file(const string& path)
{
	ifstream inFile(path, ios::in);

	string fileText = "";
	string temp = "";
	while (getline(inFile, temp)) 
	{
		fileText += temp + "\n";
	}
	inFile.close();
	return fileText;
}

void uni4(GLuint program, const string& nameColor, float red, float green, float blue, float alpha)
{
	glUniform4f(glGetUniformLocation(program, "ourColor"), red, green, blue, alpha);
}

void checkCompileErrors(GLuint shader, const string& type)
{
	GLint success;
	if (type == "fragment_shader" || type == "vertex_shader")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[1024];
			glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
			cerr << "Error: " << type << " compilation error\n" << infoLog << "\n";
		}
		else
		{
			cout << type << " compilation was successful\n";
		}
	}
	else if (type == "program")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[1024];
			glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
			cerr << "Error: program linking error\n" << infoLog << "\n";
		}
		else
		{
			cout << "Program linking was successful\n";
		}
	}
}

GLuint compileShader(string& code, GLuint shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	const char* shaderSource = code.c_str();
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	if (shaderType == GL_FRAGMENT_SHADER)
	{
		checkCompileErrors(shader, "fragment_shader");
	}
	else if (shaderType == GL_VERTEX_SHADER)
	{
		checkCompileErrors(shader, "vertex_shader");
	}

	return shader;
}

void settingMat4(GLuint ID, const char* name, glm::mat4 type)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &type[0][0]);
}

GLuint load_shader_program(const string& vertexShaderPath, const string& fragmentShaderPath)
{
	string vertexCode = load_data_from_file(vertexShaderPath);
	string fragCode = load_data_from_file(fragmentShaderPath);

	GLuint vs = compileShader(vertexCode, GL_VERTEX_SHADER);
	GLuint fs = compileShader(fragCode, GL_FRAGMENT_SHADER);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
	glLinkProgram(shader_program);

	checkCompileErrors(shader_program, "program");

	return shader_program;
}
#endif
