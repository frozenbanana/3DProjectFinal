#include "Camera.hpp"
#include <iostream>

// Constructor with vectors
	void Camera::UpdateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length Gets closer to 0 the more you look up or down which results in slower movement.
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}

	Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY), m_zoom(ZOOM)
	{
		m_position = position;
		m_worldUp = up;
		m_yaw = yaw;
		m_pitch = pitch;
    UpdateCameraVectors();
	}

	// Constructor with scalar values
	Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY), m_zoom(ZOOM)
	{
		m_position = glm::vec3(posX, posY, posZ);
		m_worldUp = glm::vec3(upX, upY, upZ);
		m_yaw = yaw;
		m_pitch = pitch;
		UpdateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = m_movementSpeed * deltaTime;

		if (direction == FORWARD) {
			m_position += m_front * velocity;
		}

		if (direction == BACKWARD) {
			m_position -= m_front * velocity;
		}

		if (direction == LEFT) {
			m_position -= m_right * velocity;
		}

		if (direction == RIGHT) {
			m_position += m_right * velocity;
		}

		if (direction == UP) {
			m_position += m_up * velocity;
		}
    std::cout << "Updating camera positions" << std::endl;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
	{
		xOffset *= m_mouseSensitivity;
		yOffset *= m_mouseSensitivity;

		m_yaw += xOffset;
		m_pitch += yOffset;

		// Make sure that when pitch is out of bounds, screen doesn't Get flipped
		if (constrainPitch) {
			if (m_pitch > 89.0f) {
				m_pitch = 89.0f;
			}

			if (m_pitch < -89.0f) {
				m_pitch = -89.0f;
			}
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
    std::cout << "Updating camera vectors" << std::endl;
		UpdateCameraVectors();
	}

	GLfloat Camera::GetZoom()
	{
		return m_zoom;
	}
	glm::vec3 Camera::GetPosition() {
		return m_position;
	}

	glm::vec3 Camera::GetFront()
	{
		return m_front;
	}
