#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "RenderWindow.hpp"
#include "InputQueue.hpp"
#include "Camera.hpp"

class InputQueue;
class RenderWindow;
class Camera;

class GameState {
public:
	GameState();
	void frameStart();
	void frameEnd();
	~GameState();

	InputQueue* inputQueue;
	RenderWindow* renderWindow;
	Camera* camera;
	unsigned int resolutionWidth = 1920;  // debug hardcode
	unsigned int resolutionHeight = 1080;
	double frameTimeStart;
	double framesPerSecond;
	double lastFrameTotalTime;
	double lastFrameMousePosX = (double)resolutionWidth / 2;
	double lastFrameMousePosY = (double)resolutionHeight / 2;
	float aspectRatio = (float) resolutionWidth / resolutionHeight;

private:
	const unsigned int fpsFrameRange = 10;
	std::vector<double> frameTimes;
	unsigned int fpsIterationCounter = 0;
};

#endif
