#ifndef MASTERRENDERER_HPP
#define MASTERRENDERER_HPP

#include <map>
#include "RenderableObject.hpp"

class MasterRenderer {
public:
	
	void addRenderableObject(RenderableObject* renderableObject);
	void render();

private:

	std::map<std::pair<float, std::string>, RenderableObject*> renderableObjects;
};

#endif
