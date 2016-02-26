#ifndef PHYSICALOBJECTRENDERER_HPP
#define PHYSICALOBJECTRENDERER_HPP

#include <map>
#include "PhysicalObject.hpp"
#include "RenderingStructure.hpp"

class PhysicalObject;

class PhysicalObjectRenderer {
public:
	
	PhysicalObjectRenderer();
	void addPhysicalObject(PhysicalObject* renderableObject);
	void render();
	~PhysicalObjectRenderer();

private:

	void callInstancesCustomerRenderer(RenderingStructure* objectRenderingStructure);
	std::map<std::string, RenderingStructure> renderableMapping;
	void abortOnOpenGlError();
};

#endif
