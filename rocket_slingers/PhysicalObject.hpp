#ifndef PHYSICALOBJECT_HPP
#define PHYSICALOBJECT_HPP

#include <glm.hpp>
#include <vector>
#include "OglShaderProgram.hpp"
#include "EventListener.hpp"
#include "GameState.hpp"

class InputQueue;

class PhysicalObject : public EventListener {
public:
	PhysicalObject(GameState* gameState);
	virtual void updatePhysicalState();
	virtual void render();
	virtual void inputEventCallback(InputEvent inputEvent);
	virtual ~PhysicalObject();

protected:
	virtual void initGeometry();
	virtual void initShaders();

	GameState* gameState;
	GLuint masterVao;
	GLuint modelVbo;
	GLuint colorVbo;
	GLuint transformVbo;
	unsigned int transformSize;
	std::vector<glm::mat4> transformData;
	unsigned int colorSize;
	std::vector<glm::vec4> colorData;
	OglShaderProgram shaderProg;
};

#endif
