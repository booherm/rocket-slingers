#ifndef PHYSICALOBJECTRENDERER_HPP
#define PHYSICALOBJECTRENDERER_HPP

#include <map>
#include "PhysicalObject.hpp"

class PhysicalObject;

class PhysicalObjectRenderer {
public:

	PhysicalObjectRenderer();
	unsigned int addPhysicalObject(PhysicalObject* renderableObject);
	void render();
	~PhysicalObjectRenderer();

private:

	struct RenderingStructure {
		unsigned int masterVao;
		unsigned int modelVbo;
		unsigned int modelOriginOffsetVbo;
		unsigned int colorVbo;
		unsigned int transformVbo;
		unsigned int modelVerticesCount;
		unsigned int glRenderingMode;
		OglShaderProgram* shaderProgram;
		std::vector<PhysicalObject*> objectInstances;
	};

	std::map<std::string, RenderingStructure> renderableMapping;
	unsigned int objectIdCounter;
};

#endif
