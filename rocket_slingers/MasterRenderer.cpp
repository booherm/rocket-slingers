#include "MasterRenderer.hpp"

void MasterRenderer::addRenderableObject(RenderableObject* renderableObject) {
	std::pair<float, std::string> renderableObjectKey(renderableObject->zDepth, renderableObject->id);
	renderableObjects[renderableObjectKey] = renderableObject;
}

void MasterRenderer::render() {

	glClear(GL_COLOR_BUFFER_BIT);

	std::map<std::pair<float, std::string>, RenderableObject*>::iterator objectIterator = renderableObjects.begin();
	std::map<std::pair<float, std::string>, RenderableObject*>::iterator objectIteratorEnd = renderableObjects.end();

	while (objectIterator != objectIteratorEnd) {

		RenderableObject* ro = objectIterator->second;
		if (ro->shouldRender)
			ro->render();
		objectIterator++;

	}
	
}
