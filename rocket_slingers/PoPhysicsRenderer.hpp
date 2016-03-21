#ifndef POPHYSICSRENDERER_HPP
#define POPHYSICSRENDERER_HPP

#include "PhysicalObject.hpp"

class PoPhysicsRenderer : public PhysicalObject, public b2Draw
{
public:
	PoPhysicsRenderer(const std::string& objectId, GameState* gameState);
	void render();

	/// Draw a closed polygon provided in CCW order.
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	void DrawTransform(const b2Transform& xf);

	~PoPhysicsRenderer();

private:

	void initShaders();
	void initGeometry();
};

#endif
