#include "Camera.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>

Camera::Camera(GameState* gameState) {
	this->Position = glm::vec3(0.0f, 0.0f, 3.0f);
	this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->Yaw = YAW;
	this->Pitch = PITCH;
	this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVTY;
	this->Zoom = ZOOM;
	this->updateCameraVectors();

	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_LEFT, InputEvent::IEKS_PRESS, this);
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_RIGHT, InputEvent::IEKS_PRESS, this);
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_UP, InputEvent::IEKS_PRESS, this);
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_DOWN, InputEvent::IEKS_PRESS, this);
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_W, InputEvent::IEKS_PRESS, this);
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_A, InputEvent::IEKS_PRESS, this);
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_S, InputEvent::IEKS_PRESS, this);
	gameState->inputQueue->subscribeToInputEvent(InputEvent::IEK_KEY_D, InputEvent::IEKS_PRESS, this);
}

void Camera::inputEventCallback(InputEvent inputEvent) {
	std::cout << "event call back to camera, event = " << std::endl;
	inputEvent.print();
	switch (inputEvent.eventKey) {
		case InputEvent::IEK_KEY_A:
		case InputEvent::IEK_KEY_LEFT:
			ProcessKeyboard(LEFT, (float) glfwGetTime());
			break;
		case InputEvent::IEK_KEY_D:
		case InputEvent::IEK_KEY_RIGHT:
			ProcessKeyboard(RIGHT, (float)glfwGetTime());
			break;
		case InputEvent::IEK_KEY_W:
		case InputEvent::IEK_KEY_UP:
			ProcessKeyboard(FORWARD, (float)glfwGetTime());
			break;
		case InputEvent::IEK_KEY_S:
		case InputEvent::IEK_KEY_DOWN:
			ProcessKeyboard(BACKWARD, (float)glfwGetTime());
			break;
	}
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	//GLfloat velocity = this->MovementSpeed * deltaTime;
	GLfloat velocity = this->MovementSpeed;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
//void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}
