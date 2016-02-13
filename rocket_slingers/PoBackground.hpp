#ifndef POBACKGROUND_HPP
#define POBACKGROUND_HPP

#include "PhysicalObject.hpp"

class PoBackground : public PhysicalObject {
public:
	PoBackground(GameState* gameState);
	void doRenderUpdate();

private:
	void initShaders();
	void initGeometry();
};

#endif
