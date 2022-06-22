#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <thread>

#include <irrKlang.h>

#include "SoundEngine.h"

SoundEngine::SoundEngine(Camera* listener) {
	m_engine = irrklang::createIrrKlangDevice();
	m_listener = listener;
}

void SoundEngine::Play2D(const char* filepath) {
	m_engine->play2D(filepath);
}

void SoundEngine::Play3D(const char* filepath) {
	m_engine->play3D(filepath, irrklang::vec3df(m_listener->position.x, m_listener->position.y, m_listener->position.z));
}
