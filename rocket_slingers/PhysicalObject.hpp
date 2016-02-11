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

class InputQueue;

class PhysicalObject : public EventListener {
public:
	PhysicalObject(const std::string& objectType, GameState* gameState);
	virtual ~PhysicalObject();

	std::string objectType;
	unsigned int glRenderingMode;
	const unsigned int maxInstanceCount = 1000;

	// rendering
	std::vector<glm::vec3>* getModelVertices();
	std::vector<glm::vec3>* getModelOriginOffsetData();
	std::vector<glm::vec4>* getColorData();
	std::vector<glm::mat4>* getTransformData();
	OglShaderProgram* getShaderProgram();
	bool shouldRender;

	// physics
	virtual void updatePhysicalState();
	virtual void inputEventCallback(InputEvent inputEvent);

protected:

	GameState* gameState;

	// rendering
	virtual void initGeometry();
	virtual void initShaders();
	std::vector<glm::vec3> modelVertices;
	std::vector<glm::vec3> modelOriginOffsetData;
	std::vector<glm::vec4> colorData;
	std::vector<glm::mat4> transformData;
	OglShaderProgram shaderProg;

	// physics
	void prepareTimeChangeValues();
	virtual void resetForce();
	virtual void applyForce(glm::vec3 force);
	virtual void applyAcceleration(glm::vec3 acceleration);
	virtual void updatePhysics();
	float mass;
	unsigned int physicsUpdateIterationsRequired;
	float changeInTime;
	float maxAllowedChangeInTime;
	glm::vec3 worldPosition;
	glm::vec3 velocity;
	glm::vec3 force;
};

#endif
