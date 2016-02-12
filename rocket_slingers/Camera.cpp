#include "Camera.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>

Camera::Camera(GameState* gameState) {

	this->gameState = gameState;
	
	orthoViewWidth = gameState->worldViewportScaler * gameState->aspectRatio;
	orthoViewHeight = gameState->worldViewportScaler;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
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
	/*
	// perspective projection transform
	float yFieldOfView = glm::quarter_pi<float>();
	float nearClippingPlaneDistance = 0.1f;
	float farClippingPlaneDistance = 100.0f;
	projectionTransform = glm::perspective(yFieldOfView, gameState->aspectRatio, nearClippingPlaneDistance, farClippingPlaneDistance);
	*/

	// orthographic projection transform
//	projectionTransform = glm::ortho(-gameState->aspectRatio, gameState->aspectRatio, -1.0f, 1.0f);
	projectionTransform = glm::ortho(0.0f, orthoViewWidth, 0.0f, orthoViewHeight);
//projectionTransform = glm::ortho(-gameState->aspectRatio - (-gameState->aspectRatio, 2.0f * gameState->aspectRatio, -1.0f, 1.0f);

	// add camera as listener for input events
	InputQueue* iq = gameState->inputQueue;
	iq->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_LEFT, this);
	iq->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_RIGHT, this);
	iq->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_UP, this);
	iq->subscribeToKeyboardEvent(SDL_PRESSED, SDLK_DOWN, this);
}

void Camera::inputEventCallback(const SDL_Event& inputEvent) {

	// debug - constant time map
	switch (inputEvent.key.keysym.sym) {
		case SDLK_LEFT:
			translateCamera(CT_LEFT);
			break;
		case SDLK_RIGHT:
			translateCamera(CT_RIGHT);
			break;
		case SDLK_UP:
			translateCamera(CT_UP);
			break;
		case SDLK_DOWN:
			translateCamera(CT_DOWN);
			break;
/*		case InputEvent::IEK_MOUSE_MOVE:
			float xOffset = (float)(inputEvent.xCoordinate - gameState->lastFrameMousePosX);
			float yOffset = (float)(inputEvent.yCoordinate - gameState->lastFrameMousePosY);
			updateCameraDirection(xOffset, yOffset, false);
			break; */
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

void Camera::translateCamera(CameraTranslation direction) {
	float velocity = movementSpeed * gameState->fLastFrameTotalTimeSeconds;

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
