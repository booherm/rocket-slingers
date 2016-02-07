#include "Camera.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>

Camera::Camera(GameState* gameState) {

	this->gameState = gameState;
	position = glm::vec3(0.0f, 0.0f, 3.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -glm::half_pi<float>();
	pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = 1.0f;
	mouseSensitivity = 0.0005f;
	zoom = 45.0f;   // debug, need radians?
	updateCameraVectors();

	// view transform
	viewTransform = glm::lookAt(position, position + front, up);

	// projection transform
	float yFieldOfView = glm::quarter_pi<float>();
	float nearClippingPlaneDistance = 0.1f;
	float farClippingPlaneDistance = 100.0f;
	projectionTransform = glm::perspective(yFieldOfView, gameState->aspectRatio, nearClippingPlaneDistance, farClippingPlaneDistance);

	// add camera as listener for input events
	InputQueue* iq = gameState->inputQueue;
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_LEFT, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_RIGHT, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_UP, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_DOWN, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_W, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_A, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_S, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_D, InputEvent::IEKS_PRESS, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_LEFT, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_RIGHT, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_UP, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_DOWN, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_W, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_A, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_S, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_KEY_D, InputEvent::IEKS_REPEAT, this);
	iq->subscribeToInputEvent(InputEvent::IEK_MOUSE_MOVE, InputEvent::IEKS_NO_STATE, this);
}

void Camera::inputEventCallback(InputEvent inputEvent) {
	switch (inputEvent.eventKey) {
		case InputEvent::IEK_KEY_A:
		case InputEvent::IEK_KEY_LEFT:
			translateCamera(CT_LEFT, (float) glfwGetTime());
			break;
		case InputEvent::IEK_KEY_D:
		case InputEvent::IEK_KEY_RIGHT:
			translateCamera(CT_RIGHT, (float)glfwGetTime());
			break;
		case InputEvent::IEK_KEY_W:
			translateCamera(CT_FORWARD, (float)glfwGetTime());
			break;
		case InputEvent::IEK_KEY_S:
			translateCamera(CT_BACKWARD, (float)glfwGetTime());
			break;
		case InputEvent::IEK_KEY_UP:
			translateCamera(CT_UP, (float)glfwGetTime());
			break;
		case InputEvent::IEK_KEY_DOWN:
			translateCamera(CT_DOWN, (float)glfwGetTime());
			break;
		case InputEvent::IEK_MOUSE_MOVE:
			float xOffset = (float)(inputEvent.xCoordinate - gameState->lastFrameMousePosX);
			float yOffset = (float)(inputEvent.yCoordinate - gameState->lastFrameMousePosY);
			updateCameraDirection(xOffset, yOffset, false);
			break;
	}

	// update view and projection transform matrix
	viewTransform = glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getViewTransform() {
	return viewTransform;
}

glm::mat4 Camera::getProjectionTransform() {
	return projectionTransform;
}

void Camera::translateCamera(CameraTranslation direction, float deltaTime) {
	float velocity = movementSpeed * (float)gameState->lastFrameTotalTime;

	switch (direction) {
	case CT_LEFT:
		position -= right * velocity;
		break;
	case CT_RIGHT:
		position += right * velocity;
		break;
	case CT_FORWARD:
		position += front * velocity;
		break;
	case CT_BACKWARD:
		position -= front * velocity;
		break;
	case CT_UP:
		position += up * velocity;
		break;
	case CT_DOWN:
		position -= up * velocity;
		break;
	}
}

void Camera::updateCameraDirection(float xoffset, float yoffset, bool constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)  // debug, need radians?
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraZoom(float yoffset) {
	// debug, need radians?
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors() {
	glm::vec3 newFront;
	newFront.x = cos(yaw) * cos(pitch);
	newFront.y = sin(pitch);
	newFront.z = sin(yaw) * cos(pitch);
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
