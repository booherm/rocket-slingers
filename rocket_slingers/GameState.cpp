#include "GameState.hpp"

GameState::GameState() {
	inputQueue = new InputQueue();
	renderWindow = new RenderWindow(this);
	camera = new Camera(this);

	frameTimes.resize(fpsFrameRange);
}

void GameState::frameStart() {
	frameTimeStart = renderWindow->getTimeSeconds();
}

void GameState::frameEnd() {
	renderWindow->getCurrentCursorPosition(&lastFrameMousePosX, &lastFrameMousePosY);

	// calculate frames per second
	lastFrameTotalTime = renderWindow->getTimeSeconds() - frameTimeStart;
	if (fpsIterationCounter == fpsFrameRange) {
		double totalTime = 0;
		for (unsigned int s = 0; s < fpsFrameRange; s++) {
			totalTime += frameTimes[s];
		}
		framesPerSecond = fpsFrameRange / totalTime;
		fpsIterationCounter = 0;
		//cout << "fps = " << framesPerSecond << endl;
	}
	else {
		frameTimes[fpsIterationCounter++] = lastFrameTotalTime;
	}
}

GameState::~GameState() {
	delete camera;
	delete renderWindow;
	delete inputQueue;
}
