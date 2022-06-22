#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "Shader.h"

// converts glsl to raw char for use in opengl, probably much longer than needed
Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {
	std::string line;

	std::string vertexCode;
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::fstream vertexFile = std::fstream(vertexShaderFilePath);
	while (vertexFile.is_open()) {
		while (std::getline(vertexFile, line)) {
			line.append("\n");
			vertexCode.append(line);
		}
		vertexFile.close();
	}

	const char* vertexCodeChar = vertexCode.c_str();
	glShaderSource(m_vertexShader, 1, &vertexCodeChar, NULL);
	glCompileShader(m_vertexShader);

	line.clear();

	std::string fragmentCode;
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::fstream fragmentFile = std::fstream(fragmentShaderFilePath);
	while (fragmentFile.is_open()) {
		while (std::getline(fragmentFile, line)) {
			line.append("\n");
			fragmentCode.append(line);
		}
		fragmentFile.close();
	}

	const char* fragmentCodeChar = fragmentCode.c_str();
	glShaderSource(m_fragmentShader, 1, &fragmentCodeChar, NULL);
	glCompileShader(m_fragmentShader);

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glLinkProgram(m_program);

	glUseProgram(m_program);
}

void Shader::Use() {
	glUseProgram(m_program);
}

void Shader::SetInt(const char* name, int value) const {
	glUniform1i(glGetUniformLocation(m_program, name), value);
}

void Shader::SetFloat(const char* name, float value) const {
	glUniform1f(glGetUniformLocation(m_program, name), value);
}

void Shader::SetVector2(const char* name, float x, float y) const {
	glUniform2f(glGetUniformLocation(m_program, name), x, y);
}

void Shader::SetVector2(const char* name, glm::vec2 vector) const {
	glUniform2f(glGetUniformLocation(m_program, name), vector.x, vector.y);
}

void Shader::SetVector3(const char* name, glm::vec3 vector) const {
	glUniform3f(glGetUniformLocation(m_program, name), vector.x, vector.y, vector.z);
}

void Shader::SetVector3(const char* name, int vector[]) const {
	glUniform3i(glGetUniformLocation(m_program, name), vector[0], vector[1], vector[2]);
}

void Shader::SetVector3(const char* name, float vector[]) const {
	glUniform3f(glGetUniformLocation(m_program, name), vector[0], vector[1], vector[2]);
}

void Shader::SetVector3(const char* name, int x, int y, int z) const {
	glUniform3i(glGetUniformLocation(m_program, name), x, y, z);
}

void Shader::SetVector4(const char* name, glm::vec4 vector) {
	glUniform4f(glGetUniformLocation(m_program, name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetMatrix4(const char* name, glm::mat4 matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(m_program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
