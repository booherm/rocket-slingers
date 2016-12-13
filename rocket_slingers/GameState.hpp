#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "RenderWindow.hpp"
#include "EventBus.hpp"
#include "Camera.hpp"
#include "MasterRenderer.hpp"
#include "AudioManager.hpp"
#include "PhysicsManager.hpp"

class EventBus;
class RenderWindow;
class Camera;
class Stage;
class MasterRenderer;

class GameState {
public:
	GameState();
	void frameStart();
	void frameEnd();
	~GameState();

	EventBus* eventBus;
	RenderWindow* renderWindow;
	Camera* camera;
	MasterRenderer* masterRenderer;
	AudioManager* audioManager;
	Stage* activeStage;
	PhysicsManager* physicsManager;

	const std::string version = "0.1.0";
	const unsigned int resolutionWidth = 1920;  // debug hardcode
	const unsigned int resolutionHeight = 1080;
	double dFrameTimeStart;
	float fFrameTimeStart;
	double dFramesPerSecond;
	float fFramesPerSecond;
	double dLastFrameTotalTimeSeconds;
	float fLastFrameTotalTimeSeconds;

	const float worldViewportHeight = 20.0f;
	float aspectRatio = (float) resolutionWidth / resolutionHeight;
	const float worldViewportWidth = worldViewportHeight * aspectRatio;

	glm::vec3 gravitationalAcceleration;

private:
	const unsigned int fpsFrameRange = 10;
	std::vector<double> frameTimes;
	unsigned int fpsIterationCounter = 0;
};

#endif
