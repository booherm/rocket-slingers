#ifndef POTEXTLABELFPS_HPP
#define POTEXTLABELFPS_HPP

#include "PoTextLabel.hpp"

class PoTextLabelFps : public PoTextLabel {
public:
	PoTextLabelFps(const std::string& objectId, GameState* gameState);
	void gameEventCallback(const Event& eventObj);

private:
};

#endif
