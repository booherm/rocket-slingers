#include "GameState.hpp"

GameState::GameState() {
	inputQueue = new InputQueue(this);
	renderWindow = new RenderWindow(this);
	camera = new Camera(this);
	physicalObjectRenderer = new PhysicalObjectRenderer();

	frameTimes.resize(fpsFrameRange);
}

void GameState::frameStart() {
	dFrameTimeStart = renderWindow->getTimeSeconds();
	fFrameTimeStart = (float)dFrameTimeStart;
}

void GameState::frameEnd() {
	renderWindow->getCurrentCursorPosition(&dLastFrameMousePosX, &dLastFrameMousePosY);
	fLastFrameMousePosX = (float) dLastFrameMousePosX;
	fLastFrameMousePosY = (float) dLastFrameMousePosY;

	// calculate frames per second
	dLastFrameTotalTimeSeconds = renderWindow->getTimeSeconds() - dFrameTimeStart;
	fLastFrameTotalTimeSeconds = (float) dLastFrameTotalTimeSeconds;

	if (fpsIterationCounter == fpsFrameRange) {
		double totalTime = 0;
		for (unsigned int s = 0; s < fpsFrameRange; s++) {
			totalTime += frameTimes[s];
		}
		dFramesPerSecond = fpsFrameRange / totalTime;
		fFramesPerSecond = (float) dFramesPerSecond;
		fpsIterationCounter = 0;
		//cout << "fps = " << framesPerSecond << endl;
	}
	else {
		frameTimes[fpsIterationCounter++] = dLastFrameTotalTimeSeconds;
	}
}

GameState::~GameState() {
	delete physicalObjectRenderer;
	delete camera;
	delete renderWindow;
	delete inputQueue;
}
