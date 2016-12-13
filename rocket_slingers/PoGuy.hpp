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
	void getArmLocation(b2Vec2& armLocation);
	void getRocketForce(glm::vec3& rocketForce);
	void setRopeAttachedToStructure(bool attachedToStructure);
	b2Body* getRigidBody();
	void processContactBegin(PhysicalObject* contactingObject, b2Contact* contact);
	void processContactEnd(PhysicalObject* contactingObject, b2Contact* contact);
	~PoGuy();

private:

	const float scalerToMeter = 0.03936f;
	float maxSpeed;
	glm::vec3 modelOriginOffset;
	glm::vec3 initialPosition;
	glm::vec3 rocketForce;
	b2Body* rigidBody;
	//PhysicalMass* cameraFocalPointPhysicalMass;
	//btGeneric6DofSpring2Constraint* cameraSpringConstraint;

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
	std::vector<b2Vec2> environmentForces;

	float rocketForceMagnitude;
};

#endif
