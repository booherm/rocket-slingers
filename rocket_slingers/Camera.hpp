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
	glm::mat4 getViewTransform();
	glm::mat4 getProjectionTransform();
	void inputEventCallback(InputEvent inputEvent);

private:
	enum CameraTranslation {
		CT_FORWARD,
		CT_BACKWARD,
		CT_LEFT,
		CT_RIGHT,
		CT_UP,
		CT_DOWN
	};

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
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	void translateCamera(CameraTranslation direction);
	void updateCameraDirection(float xoffset, float yoffset, bool constrainPitch);
	void updateCameraZoom(float yoffset);
	void updateCameraVectors();
};

#endif
