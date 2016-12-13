#ifndef STAGE_HPP
#define STAGE_HPP

#include "AllPhysicalObjects.hpp"

class GameState;

class Stage
{
public:

	Stage(GameState* gameState);
	void initialize();
	void update();
	PhysicalObject* getPhysicalObject(const std::string& objectId);

	~Stage();

private:
	
	typedef std::pair<unsigned int, std::string> StageObjectKeyUpdateOrder;

	GameState* gameState;
	std::map<StageObjectKeyUpdateOrder, PhysicalObject*> stageObjectsInUpdateOrder;
	std::map<std::string, PhysicalObject*> stageObjectsById;
	void pushStageObject(PhysicalObject* physicalObject, unsigned int objectUpdateOrder);
};

#endif
