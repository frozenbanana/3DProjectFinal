#ifndef CAMERA_HPP
#define CAMERA_HPP
// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
    // Camera Attributes
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;
  glm::mat4 m_view; // Will be passed to shader

	// Eular Angles
    GLfloat m_yaw;
    GLfloat m_pitch;

    // Camera options
    GLfloat m_movementSpeed;
    GLfloat m_mouseSensitivity;
    GLfloat m_zoom;

  // Perspective attributes
  GLfloat m_fov;
  GLfloat m_aspect;
  GLfloat m_nearPlane;
  GLfloat m_farPlane;
  glm::mat4 m_pers; // perspective will be passed to shader
  glm::mat4 m_viewPers; // perspective will be passed to shader

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void UpdateCameraVectors();
public:
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = -90.0f, GLfloat pitch = 0.0f);

	// Constructor with scalar values
	// Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();
	glm::mat4 GetPersMatrix();
	glm::mat4& GetViewPersMatrixRef();
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yOffset);
	GLfloat GetZoom();
	glm::vec3 GetPosition();
	glm::vec3 GetFront();
	glm::vec3 m_position;
  glm::mat4 GetCamView();
  glm::mat4 GetCamPers();
  // Get Mouse data
  void MouseCallback(double xPos, double yPos);
  void KeyCallback(int key, int scan, int act, int mode);
};

#endif
