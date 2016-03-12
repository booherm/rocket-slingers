#ifndef POGUY_HPP
#define POGUY_HPP

#include "PhysicalObject.hpp"
#include <map>

class PoGuy : public PhysicalObject {
public:
	PoGuy(const std::string& objectId, GameState* gameState);
	void doPhysicalUpdate();
	void render();
	void sdlInputEventCallback(const Event& eventObj);
	void getArmLocation(glm::vec3& armLocation);
	void getRocketImpulse(glm::vec3& rocketImpulse);
	//void updateTransformFromRope(const btTransform& transform);
	void setRopeAttachedToStructure(bool attachedToStructure);
	//btRigidBody* getRigidBody();
	PhysicalMass* getPhysicalMass();
	~PoGuy();

private:

	const float scalerToMeter = 0.03936f;
	glm::vec3 modelOriginOffset;
	glm::vec3 initialPosition;
	glm::vec3 rocketImpulse;
	PhysicalMass* physicalMass;
	PhysicalMass* cameraFocalPointPhysicalMass;
	btGeneric6DofSpring2Constraint* cameraSpringConstraint;

	bool rocketOn;
	bool ropeAttachedToStructure;
	bool gravityOn;
	void initShaders();
	void initGeometry();
	void initPhysics();
	void initEventSubsriptions();
	unsigned int soundEffectInstanceId;
	
	unsigned int keyDownCount;
	std::map<int, bool> keyStates;

	float rocketImpulseMagnitude;
};

#endif
