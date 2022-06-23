#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

struct Shader {
public:
	/* constructor */
	Shader(const char* vertexShaderFilePath = "", const char* fragmentShaderFilePath = "");
	/* sets the shader to be used, this needs to be called before editing anything in shader */
	void Use();

	/* a whole bunch of functions for sending values to shader, 
	explaining everyone of them would drive me into the abyss */

	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;
	void SetVector2(const char* name, float x, float y) const;
	void SetVector2(const char* name, glm::vec2 vector) const;
	void SetVector3(const char* name, glm::vec3 vector) const;
	void SetVector3(const char* name, int vector[]) const;
	void SetVector3(const char* name, float vector[]) const;
	void SetVector3(const char* name, int r, int g, int b) const;
	void SetVector4(const char* name, glm::vec4 vector);
	void SetMatrix4(const char* name, glm::mat4 matrix) const;
private:
	unsigned int m_program;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
};