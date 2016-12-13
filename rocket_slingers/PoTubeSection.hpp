#ifndef POTUBESECTION_HPP
#define POTUBESECTION_HPP

#include "PhysicalObject.hpp"

class PoTubeSection : public PhysicalObject {
public:

	enum TileType {
		STRAIGHT_HORZ,
		STRAIGHT_VERT,
		LEFT_BOTTOM_90,
		LEFT_TOP_90,
		TOP_LEFT_90,
		TOP_RIGHT_90,
		RIGHT_TOP_90,
		RIGHT_BOTTOM_90,
		BOTTOM_LEFT_90,
		BOTTOM_RIGHT_90
	};

	PoTubeSection(const std::string& objectId, GameState* gameState, TileType tileType, const b2Vec2& scalers, const b2Vec2& position);
	b2Vec2 getFixtureAcceleration(b2Fixture* fixture);

	void render();
	~PoTubeSection();

private:

	struct TileTypeTransform {
		float rotation;
		glm::vec3 translation;
	};

	b2Body* rigidBody;
	TileType tileType;
	b2Vec2 position;
	b2Vec2 scalers;

	std::vector<b2Vec2> lowerFixture;
	std::vector<b2Vec2> centerFixture;
	std::vector<b2Vec2> upperFixture;

	std::map<b2Fixture*, b2Vec2> fixtureAccelerations;
	std::map<TileType, TileTypeTransform> tileTransforms;

	void initShaders();
	void initGeometry();
	void initPhysics();

};

#endif
