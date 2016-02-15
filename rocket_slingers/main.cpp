#include <iostream>
#include <windows.h>
#include "GuiConsole.hpp"
#include "RocketSlingersController.hpp"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>



int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	startConsole();
	try {
		std::cout << "Starting Rocket Slingers" << std::endl;

		//RocketSlingersController rsc;
		//rsc.start();




		// bullet test
		// Build the broadphase
		btBroadphaseInterface* broadphase = new btDbvtBroadphase();

		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

		// The actual physics solver
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		// The world.
		btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		dynamicsWorld->setGravity(btVector3(0, -10, 0));

		// Do_everything_else_here
		btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
		btCollisionShape* fallShape = new btSphereShape(1);
		btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
		btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

		dynamicsWorld->addRigidBody(groundRigidBody);

		btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);

		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
		dynamicsWorld->addRigidBody(fallRigidBody);


		for (int i = 0; i < 300; i++) {

			dynamicsWorld->stepSimulation(1 / 60.f, 10);

			btTransform trans;
			fallRigidBody->getMotionState()->getWorldTransform(trans);

			std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
		}

		// Clean up behind ourselves like good little programmers
		delete groundShape;
		delete fallShape;
		delete dynamicsWorld;
		delete solver;
		delete dispatcher;
		delete collisionConfiguration;
		delete broadphase;







		std::cout << "Exiting Rocket Slingers" << std::endl;
	}
	catch (std::string e) {
		std::cout << "Exception: " << e << std::endl;
	}

	system("pause");
	return 0;
}
