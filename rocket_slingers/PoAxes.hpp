#ifndef POAXES_HPP
#define POAXES_HPP

#include "PhysicalObject.hpp"

class PoAxes : public PhysicalObject {
public:
	PoAxes(const std::string& objectId, GameState* gameState);

private:

	void initShaders();
	void initGeometry();
	void render();
};

#endif
