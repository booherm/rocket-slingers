#include "PoTextLabelFps.hpp"

PoTextLabelFps::PoTextLabelFps(const std::string& objectId, GameState* gameState) : PoTextLabel(objectId, gameState) {
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::FPS_LABEL_UDPATE, this);
}

void PoTextLabelFps::gameEventCallback(const Event& eventObj) {
	setTextValue("FPS: " + std::to_string(gameState->fFramesPerSecond).substr(0, 5));
}
