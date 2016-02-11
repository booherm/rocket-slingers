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
	const float meterToWorldUnitScalar = 0.05f;
	double dFrameTimeStart;
	float fFrameTimeStart;
	double dFramesPerSecond;
	float fFramesPerSecond;
	double dLastFrameTotalTime;
	float fLastFrameTotalTime;
	double dLastFrameMousePosX = (double)resolutionWidth / 2;
	float fLastFrameMousePosX = (float)resolutionWidth / 2;
	double dLastFrameMousePosY = (double)resolutionHeight / 2;
	float fLastFrameMousePosY = (float)resolutionHeight / 2;

	float aspectRatio = (float) resolutionWidth / resolutionHeight;

private:
	const unsigned int fpsFrameRange = 10;
	std::vector<double> frameTimes;
	unsigned int fpsIterationCounter = 0;
};

#endif
