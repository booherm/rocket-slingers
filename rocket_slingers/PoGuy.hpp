#ifndef POGUY_HPP
#define POGUY_HPP

#include "PhysicalObject.hpp"
#include <map>

class PoGuy : public PhysicalObject {
public:
	PoGuy(GameState* gameState);
	void doPhysicalUpdate();
	void doRenderUpdate();
	void sdlInputEventCallback(const Event& eventObj);
	void getArmLocation(glm::vec3& armLocation);
	void getRocketForce(glm::vec3& rocketForce);
	void updateTransformFromRope(const btTransform& transform);
	//void addForceFromRope(const glm::vec3& ropeForce);
	btRigidBody* getRigidBody();
	~PoGuy();

private:

	const float scalerToMeter = 0.03936f;
	glm::vec3 initialPosition;
	glm::vec3 rocketForce;
	PhysicalMass* physicalMass;
	PhysicalMass* cameraFocalPointPhysicalMass;
	btGeneric6DofSpring2Constraint* cameraSpringConstraint;
	//PhysicalMassMultiBody* multiBody;

	bool rocketOn = false;
	bool ropeThrown = false;
	void initGeometry();
	void initPhysics();
	void initEventSubsriptions();
	unsigned int soundEffectInstanceId;
	
	unsigned int keyDownCount;
	std::map<int, bool> keyStates;

	float rocketForceMagnitude = 1800.0f;
};

#endif
