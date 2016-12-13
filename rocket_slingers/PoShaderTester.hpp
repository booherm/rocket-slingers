#ifndef POSHADERTESTER_HPP
#define POSHADERTESTER_HPP

#include "PhysicalObject.hpp"

class PoShaderTester : public PhysicalObject {
public:
	PoShaderTester(const std::string& objectId, GameState* gameState);
	void render();

private:

	glm::mat4 projectionTransform;
	float sizeScaler;
	void initShaders();
	void initGeometry();

};

#endif
