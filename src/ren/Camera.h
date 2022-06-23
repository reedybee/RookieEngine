#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Camera {
public:
	/* Constructor */
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 45.0f, float yaw = -90.0f, float pitch = 0.0f);
	/* calculates and returns new view matrix */
	glm::mat4 GetViewMatrix();
	/* polls keyboard events */
	void ProcessKeyboard(CameraMovement cameraMovement, float deltatime);
	/* polls mouse events */
	void ProcessMouse(float xoffset, float yoffset, bool mouseHidden, GLboolean constrainPitch = true);

	float fov;
	glm::vec3 position;
private:
	void UpdateCameraVectors();

	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;

	float m_movementSpeed;
	float m_mouseSensitivity;
};
