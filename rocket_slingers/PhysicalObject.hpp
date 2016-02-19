#ifndef PHYSICALOBJECT_HPP
#define PHYSICALOBJECT_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <iostream>
#include <vector>
#include "OglShaderProgram.hpp"
#include "EventListener.hpp"
#include "GameState.hpp"
#include "Utilities.hpp"
#include "PhysicalMass.hpp"

class EventBus;
class PhysicalMass;

class PhysicalObject : public EventListener {
public:
	PhysicalObject(const std::string& objectType, GameState* gameState);
	virtual ~PhysicalObject();

	virtual void sdlInputEventCallback(const Event& eventObj);
	virtual void gameEventCallback(const Event& eventObj);
	std::string objectType;
	unsigned int glRenderingMode;
	const unsigned int maxInstanceCount = 1000;

	// rendering
	bool shouldRender;
	virtual void updateRenderState();
	std::vector<glm::vec3>* getModelVertices();
	std::vector<glm::vec3>* getModelOriginOffsetData();
	std::vector<glm::vec4>* getColorData();
	std::vector<glm::mat4>* getTransformData();
	std::vector<glm::vec2>* getTextureCoordinateData();
	std::vector<unsigned int>* getTextures();
	OglShaderProgram* getShaderProgram();
	virtual void afterRender();

	// physics
	PhysicalMass* getMainComponentMass();
	void updatePhysicalState();

protected:

	GameState* gameState;

	// rendering
	virtual void doRenderUpdate();
	virtual void initGeometry();
	virtual void initShaders();
	std::vector<glm::vec3> modelVertices;
	std::vector<glm::vec3> modelOriginOffsetData;
	std::vector<glm::vec4> colorData;
	std::vector<glm::mat4> transformData;
	std::vector<glm::vec2> textureCoordinateData;
	std::vector<unsigned int> textures;
	OglShaderProgram shaderProg;

	// physics
	virtual void doPhysicalUpdate();
	bool shouldDoPhysicalUpdate;
	PhysicalMass* mainComponentMass;
	std::vector<PhysicalMass> componentMasses;
};

#endif
