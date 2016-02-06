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
	double frameTimeStart;
	double framesPerSecond;
	double lastFrameTotalTime;

private:
	const unsigned int fpsFrameRange = 10;
	std::vector<double> frameTimes;
	unsigned int fpsIterationCounter = 0;
};

#endif
