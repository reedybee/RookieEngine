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

#include "Texture.h"

Texture::Texture() {
	m_data = NULL;
	m_texture = NULL;
	m_width = NULL;
	m_height = NULL;
	m_nrChannels = NULL;
}

void Texture::CreateTexture(const char* filepath, GLenum format, GLenum wrap, GLenum filter) {

	m_data = stbi_load(filepath, &m_width, &m_height, &m_nrChannels, 0);

	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (!m_data) {
		std::cout << "Image Failed to Load: " << stbi_failure_reason << "\n";
	}

	stbi_image_free(m_data);
}

void Texture::Bind(GLenum target) {
	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
