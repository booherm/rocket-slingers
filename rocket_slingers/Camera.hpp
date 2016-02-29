#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glew.h>
#include <glm.hpp>
#include "GameState.hpp"

class GameState;

class Camera : public EventListener
{
public:
	Camera(GameState* gameState);
	const glm::mat4& getViewTransform();
	const glm::mat4& getProjectionTransform();
	void updatePosition(const glm::vec3& position);
	const glm::vec3& getPosition();

private:

	GameState* gameState;
	float orthoViewWidth;
	float orthoViewHeight;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::mat4 viewTransform;
	glm::mat4 projectionTransform;
	float yaw;
	float pitch;

	void updateCameraVectors();
};

#endif
