#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float fov, float yaw, float pitch) {
	Camera::position = position;
	m_up = up;
	m_yaw = yaw;
	m_pitch = pitch;
    Camera::fov = fov;

    m_movementSpeed = 2.5f;
    m_mouseSensitivity = 0.1f;
    m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + m_front, m_up);
}

void Camera::ProcessKeyboard(CameraMovement cameraMovement, float deltatime) {
    float velocity = m_movementSpeed * deltatime;
    switch (cameraMovement) {
    case (FORWARD):
        position += m_front * velocity;
        break;
    case (BACKWARD):
        position -= m_front * velocity;
        break;
    case (LEFT):
        position -= m_right * velocity;
        break;
    case (RIGHT):
        position += m_right * velocity;
        break;
    case (UP):
        position += m_up * velocity;
        break;
    case (DOWN):
        position -= m_up * velocity;
        break;
    }
}

void Camera::ProcessMouse(float xoffset, float yoffset, bool mouseHidden, GLboolean constrainPitch) {
    if (mouseHidden) {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }
}

void Camera::UpdateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front = glm::vec3();
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
