#ifndef POROPETARGET_HPP
#define POROPETARGET_HPP

#include "PhysicalObject.hpp"
#include "PoGuyBox2d.hpp"

class PoRopeTarget : public PhysicalObject {
public:
	PoRopeTarget(const std::string& objectId, GameState* gameState);
	void render();

private:

	glm::vec3 worldPosition;
	void initShaders();
	void initGeometry();
	void doPhysicalUpdate();
};

#endif
