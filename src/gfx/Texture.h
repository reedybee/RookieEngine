#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <iostream>
#include <thread>

struct Texture {
public:
	// constructor
	Texture();
	// creates the data required to send a texture to the gpu
	void CreateTexture(const char* filepath, GLenum format, GLenum wrap = GL_REPEAT, GLenum filter = GL_LINEAR);
	// binds the texture to target for use in shaders
	void Bind(GLenum target);
private:
	unsigned int m_texture;

	int m_width;
	int m_height;
	int m_nrChannels;

	unsigned char* m_data;
};