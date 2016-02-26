#ifndef RENDERINGSTRUCTURE_HPP
#define RENDERINGSTRUCTURE_HPP

#include <vector>
#include "OglShaderProgram.hpp"

class PhysicalObject;

struct RenderingStructure {
	unsigned int masterVao;
	unsigned int modelVbo;
	unsigned int modelOriginOffsetVbo;
	unsigned int colorVbo;
	unsigned int transformVbo;
	unsigned int textureCoordinateVbo;
	unsigned int modelVerticesCount;
	unsigned int glRenderingMode;
	std::vector<unsigned int>* textures;
	OglShaderProgram* shaderProgram;
	std::vector<PhysicalObject*> objectInstances;
	bool useCustomRenderer;
};

#endif
