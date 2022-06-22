#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <thread>

#include "Renderer.h"

Renderer::Renderer() {
	m_window = NULL;
	m_lasttime = glfwGetTime();
	m_deltatime = glfwGetTime();
	m_lastFrame = 0.0f;
}

void Renderer::CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
	m_window = glfwCreateWindow(width, height, title, monitor, share);
	MakeWindowCurrentContext();
}

void Renderer::SetWindow(GLFWwindow* window) {
	m_window = window;
	MakeWindowCurrentContext();
}

GLFWwindow* Renderer::GetWindow() {
	return m_window;
}

bool Renderer::WindowShouldClose() {
	MakeWindowCurrentContext();
	return glfwWindowShouldClose(m_window);
}

void Renderer::MakeWindowCurrentContext() {
	glfwMakeContextCurrent(m_window);
}

void Renderer::PollDeltaTime() {
	float currentFrame = (float)glfwGetTime();
	m_deltatime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
}

float Renderer::GetDeltaTime() {
	return m_deltatime;
}

void Renderer::SwapBuffers(float framerate) {
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	while (glfwGetTime() < m_lasttime + 1.0 / framerate) {
		std::this_thread::yield();
	}
	m_lasttime += 1.0 / framerate;
}
