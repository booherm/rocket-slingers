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

class InputQueue;

class PhysicalObject : public EventListener {
public:
	PhysicalObject(const std::string& objectType, GameState* gameState);
	virtual ~PhysicalObject();

	virtual void inputEventCallback(InputEvent inputEvent);
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
	OglShaderProgram* getShaderProgram();

	// physics
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
	OglShaderProgram shaderProg;

	// physics
	virtual void doPhysicalUpdate();
	bool shouldDoPhysicalUpdate;
	void resetForces();
	float changeInTime;
	float maxAllowedChangeInTime;
	PhysicalMass* mainComponentMass;
	std::vector<PhysicalMass> componentMasses;
};

#endif
