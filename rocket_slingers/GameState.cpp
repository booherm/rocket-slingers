#include "GameState.hpp"
#include "Stage.hpp"

GameState::GameState() {
	eventBus = new EventBus(this);
	renderWindow = new RenderWindow(this);
	camera = new Camera(this);
	physicalObjectRenderer = new PhysicalObjectRenderer();
	audioManager = new AudioManager();
	activeStage = new Stage(this);

	frameTimes.resize(fpsFrameRange);

	gravitationalAcceleration = glm::vec3(0.0f, -9.80665f, 0.0f);
}

void GameState::frameStart() {
	dFrameTimeStart = SDL_GetTicks() / 1000.0;
	fFrameTimeStart = (float)dFrameTimeStart;
}

void GameState::frameEnd() {

	// calculate frames per second
	dLastFrameTotalTimeSeconds = (SDL_GetTicks() / 1000.0) - dFrameTimeStart;
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
	delete activeStage;
	delete audioManager;
	delete physicalObjectRenderer;
	delete camera;
	delete renderWindow;
	delete eventBus;
}
