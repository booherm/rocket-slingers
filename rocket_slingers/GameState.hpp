#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "RenderWindow.hpp"
#include "InputQueue.hpp"
#include "Camera.hpp"
#include "PhysicalObjectRenderer.hpp"

class InputQueue;
class RenderWindow;
class Camera;
class PhysicalObjectRenderer;

class GameState {
public:
	GameState();
	void frameStart();
	void frameEnd();
	~GameState();

	InputQueue* inputQueue;
	RenderWindow* renderWindow;
	Camera* camera;
	PhysicalObjectRenderer* physicalObjectRenderer;

	const unsigned int resolutionWidth = 1920;  // debug hardcode
	const unsigned int resolutionHeight = 1080;
	double dFrameTimeStart;
	float fFrameTimeStart;
	double dFramesPerSecond;
	float fFramesPerSecond;
	double dLastFrameTotalTimeSeconds;
	float fLastFrameTotalTimeSeconds;

	const float worldViewportScaler = 20.0f;
	float aspectRatio = (float) resolutionWidth / resolutionHeight;

private:
	const unsigned int fpsFrameRange = 10;
	std::vector<double> frameTimes;
	unsigned int fpsIterationCounter = 0;
};

#endif
