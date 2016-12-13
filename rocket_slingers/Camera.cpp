#include "Camera.hpp"
#include <iostream>
#include <gtc/matrix_transform.hpp>

Camera::Camera(GameState* gameState) {

	this->gameState = gameState;
	
	orthoViewWidth = gameState->worldViewportWidth;
	orthoViewHeight = gameState->worldViewportHeight;

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -glm::half_pi<float>();
	pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	updateCameraVectors();

	viewTransform = glm::lookAt(position, position + front, up);

	// orthographic projection transform
	projectionTransform = glm::ortho(0.0f, orthoViewWidth, 0.0f, orthoViewHeight);
}

const glm::mat4& Camera::getViewTransform() {
	return viewTransform;
}

const glm::mat4& Camera::getProjectionTransform() {
	return projectionTransform;
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

void Camera::updatePosition(const glm::vec3& position) {

	glm::vec3 oldPosition = this->position;
	this->position = position;
	viewTransform = glm::lookAt(position, position + front, up);

	if (oldPosition != position) {
		Event e;
		e.eventPoster = this;
		e.eventType = Event::EventType::GAME_EVENT;
		e.gameEvent = Event::GameEvent::CAMERA_MOVED;
		e.eventWorldCoordinateX = position.x;
		e.eventWorldCoordinateY = position.y;
		gameState->eventBus->postEvent(e);
	}
}

const glm::vec3& Camera::getPosition() {
	return position;
}