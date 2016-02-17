#ifndef POPHYSICSRENDERER_HPP
#define POPHYSICSRENDERER_HPP

#include "PhysicalObject.hpp"

class PoPhysicsRenderer : public PhysicalObject, public btIDebugDraw
{
public:
	PoPhysicsRenderer(GameState* gameState);
	void doRenderUpdate();
	void sdlInputEventCallback(const Event& eventObj);
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void flushLines();
	void afterRender();


	~PoPhysicsRenderer();

	// btIDebugDraw interface implementation
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;
	//Svoid setDefaultColors(const DefaultColors&);
	
private:
	void initGeometry();

};

#endif













