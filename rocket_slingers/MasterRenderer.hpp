#ifndef MASTERRENDERER_HPP
#define MASTERRENDERER_HPP

#include <map>
#include "RenderableObject.hpp"

class MasterRenderer {
public:

	//MasterRenderer();
	void addRenderableObject(RenderableObject* renderableObject);
	void renderDefaultFrameBuffer();
	//void renderIntermediateFrameBuffer();

private:

	unsigned int intermediateFrameBufferId;
	std::map<std::pair<float, std::string>, RenderableObject*> renderableObjects;
};

#endif
