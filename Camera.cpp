#include "Camera.hpp"
#include "GLOBALS.hpp"
#include <iostream>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
{
  m_front = glm::vec3(0.0f, 0.0f, 1.0f);
  m_position = position;
  m_worldUp = up;
  m_yaw = yaw;
  m_pitch = pitch;
  m_fov = glm::radians(45.0f);
  m_aspect = ((GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT);
  m_nearPlane = 0.1f;
  m_farPlane = 100.0f;

  m_heightOffset = 5.0f;
  // make camera perspective matrix
  m_pers = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);

  m_movementSpeed = 5.0f;
  m_mouseSensitivity = 0.6f;
  m_zoom = 1.0f;
  UpdateCameraVectors();
}

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

  // Update view matrix
  m_view = glm::lookAt(m_position, m_position + m_front, m_up); // might me m_up instead

  // For frustum
  m_viewPers = m_pers * m_view;
}

void Camera::ApplyGravity(GLfloat heightLimit, GLfloat deltaTime) {
  GLfloat realHeightLimit = m_heightOffset + heightLimit;
  if (m_position.y > realHeightLimit)
    m_position.y -= m_up.y * deltaTime;
  else
    m_position.y = realHeightLimit;

  UpdateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() {
  return m_view;
}

glm::mat4 Camera::GetPersMatrix() {
  return m_pers;
}

glm::mat4 Camera::GetViewPersMatrix() {
  return m_viewPers;
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

  UpdateCameraVectors();
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch) {
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
  UpdateCameraVectors();
}

GLfloat Camera::GetZoom() {
  return m_zoom;
}

GLfloat Camera::GetPosX() {
  return m_position.x;
}

GLfloat Camera::GetPosZ() {
  return m_position.z;
}

glm::vec3 Camera::GetPosition() {
  return m_position;
}

glm::vec3 Camera::GetFront() {
  return m_front;
}
