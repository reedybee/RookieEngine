#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Renderer {
public:
	Renderer();

	/* creates a glfw window */
	void CreateWindow(int width, int height, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
	/* sets a glfw window if one is already made */
	void SetWindow(GLFWwindow* window);
	/* returns the glfw window */
	GLFWwindow* GetWindow();
	/* returns window close state */
	bool WindowShouldClose();
	/* makes the window the current context */ 
	void MakeWindowCurrentContext();

	void PollDeltaTime();

	/* gets the deltatime */
	float GetDeltaTime();

	/* does all rendering */ 
	void SwapBuffers(float framerate = 60);
private:
	GLFWwindow* m_window;

	float m_deltatime;
	float m_lastFrame;
	double m_lasttime;
};
