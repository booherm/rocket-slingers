#include "PhysicalObjectRenderer.hpp"

PhysicalObjectRenderer::PhysicalObjectRenderer() {
	objectIdCounter = 0;
}

unsigned int PhysicalObjectRenderer::addPhysicalObject(PhysicalObject* renderableObject) {

	if (renderableMapping.count(renderableObject->objectType) == 0) {

		// first time seeing an object of this type, setup renderable structure
		renderableMapping[renderableObject->objectType] = RenderingStructure{};
		RenderingStructure* rs = &(renderableMapping[renderableObject->objectType]);
		rs->objectInstances.push_back(renderableObject);
		rs->shaderProgram = renderableObject->getShaderProgram();
		rs->glRenderingMode = renderableObject->glRenderingMode;
			
		// create vao, vbos
		glGenVertexArrays(1, &rs->masterVao);
		glGenBuffers(1, &rs->modelVbo);
		glGenBuffers(1, &rs->modelOriginOffsetVbo);
		glGenBuffers(1, &rs->colorVbo);
		glGenBuffers(1, &rs->transformVbo);

		// buffer model
		std::vector<glm::vec3>* modelVertices = renderableObject->getModelVertices();
		rs->modelVerticesCount = modelVertices->size();
		glBindBuffer(GL_ARRAY_BUFFER, rs->modelVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * rs->modelVerticesCount, modelVertices->data(), GL_STATIC_DRAW);

		// initialize model origin offset buffer
		glBindBuffer(GL_ARRAY_BUFFER, rs->modelOriginOffsetVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * renderableObject->maxInstanceCount, NULL, GL_STATIC_DRAW);

		// initialize color buffer
		glBindBuffer(GL_ARRAY_BUFFER, rs->colorVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * renderableObject->maxInstanceCount, NULL, GL_STATIC_DRAW);

		// initialize transform buffer
		glBindBuffer(GL_ARRAY_BUFFER, rs->transformVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 3, NULL, GL_STATIC_DRAW);

		// start vertex array object setup
		glBindVertexArray(rs->masterVao);

		// define position attribute (model)
		glBindBuffer(GL_ARRAY_BUFFER, rs->modelVbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*) 0);
		glEnableVertexAttribArray(0);

		// define model origin offset attribute (instanced)
		glBindBuffer(GL_ARRAY_BUFFER, rs->modelOriginOffsetVbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
		glVertexAttribDivisor(1, 1);
		glEnableVertexAttribArray(1);

		// define color attribute (instanced)
		glBindBuffer(GL_ARRAY_BUFFER, rs->colorVbo);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
		glVertexAttribDivisor(2, 1);
		glEnableVertexAttribArray(2);

		// define transform attribute (instanced)
		glBindBuffer(GL_ARRAY_BUFFER, rs->transformVbo);
		for (unsigned int i = 3; i <= 6; i++) {
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)((i - 3) * sizeof(glm::vec4)));
			glVertexAttribDivisor(i, 1);
			glEnableVertexAttribArray(i);
		}

		// end vertex array object setup
		glBindVertexArray(0);
	}
	else {
		// object type has been added before, just add new instance 
		renderableMapping[renderableObject->objectType].objectInstances.push_back(renderableObject);
	}

	GLenum openGlErrorCode = glGetError();
	if (openGlErrorCode != GL_NO_ERROR)
		throw "PhysicalObjectRenderer::addPhysicalObject - OpenGL error: " + openGlErrorCode;

	return objectIdCounter++;
}

void PhysicalObjectRenderer::render() {

	glClear(GL_COLOR_BUFFER_BIT);

	std::map<std::string, RenderingStructure>::iterator objectTypeIterator = renderableMapping.begin();
	std::map<std::string, RenderingStructure>::iterator objectTypeIteratorEnd = renderableMapping.end();

	while (objectTypeIterator != objectTypeIteratorEnd) {

		// get the rendering structure for this object type
		RenderingStructure* objectRenderingStructure = &objectTypeIterator->second;

		// reset buffer offsets
		unsigned int modelOriginOffsetBufferOffset = 0;
		unsigned int colorBufferOffset = 0;
		unsigned int transformBufferOffset = 0;

		// iterate over object instances and buffer all rendering data for this object type
		unsigned int componentsToRenderCount = 0;
		unsigned int instanceCount = objectTypeIterator->second.objectInstances.size();
		for (unsigned int instanceIndex = 0; instanceIndex < instanceCount; instanceIndex++) {

			PhysicalObject* po = objectTypeIterator->second.objectInstances[instanceIndex];

			if (po->shouldRender) {
				// buffer model origin offset data
				std::vector<glm::vec3>* modelOriginOffsetData = po->getModelOriginOffsetData();
				unsigned int modelOriginOffsetDataSize = sizeof(glm::vec3) * modelOriginOffsetData->size();
				glBindBuffer(GL_ARRAY_BUFFER, objectRenderingStructure->modelOriginOffsetVbo);
				glBufferSubData(GL_ARRAY_BUFFER, modelOriginOffsetBufferOffset, modelOriginOffsetDataSize, modelOriginOffsetData->data());
				modelOriginOffsetBufferOffset += modelOriginOffsetDataSize;

				// buffer color data
				std::vector<glm::vec4>* colorData = po->getColorData();
				unsigned int colorDataSize = sizeof(glm::vec4) * colorData->size();
				glBindBuffer(GL_ARRAY_BUFFER, objectRenderingStructure->colorVbo);
				glBufferSubData(GL_ARRAY_BUFFER, colorBufferOffset, colorDataSize, colorData->data());
				colorBufferOffset += colorDataSize;

				// buffer transform data
				std::vector<glm::mat4>* transformData = po->getTransformData();
				unsigned int transformDataCount = transformData->size();
				componentsToRenderCount += transformDataCount;
				unsigned int transformDataSize = sizeof(glm::mat4) * transformDataCount;
				glBindBuffer(GL_ARRAY_BUFFER, objectRenderingStructure->transformVbo);
				glBufferSubData(GL_ARRAY_BUFFER, transformBufferOffset, transformDataSize, transformData->data());
				transformBufferOffset += transformDataSize;
			}
		}
		
		// render the buffered data
		objectRenderingStructure->shaderProgram->use();
		glBindVertexArray(objectRenderingStructure->masterVao);
		glDrawArraysInstanced(objectRenderingStructure->glRenderingMode, 0, objectRenderingStructure->modelVerticesCount, componentsToRenderCount);

		objectTypeIterator++;
	}

	GLenum openGlErrorCode = glGetError();
	if (openGlErrorCode != GL_NO_ERROR)
		throw "PhysicalObjectRenderer::render - OpenGL error: " + openGlErrorCode;
}

PhysicalObjectRenderer::~PhysicalObjectRenderer() {

	// delete all gl buffers, vaos
	std::map<std::string, RenderingStructure>::iterator objectTypeIterator = renderableMapping.begin();
	std::map<std::string, RenderingStructure>::iterator objectTypeIteratorEnd = renderableMapping.end();
	while (objectTypeIterator != objectTypeIteratorEnd) {
		RenderingStructure* objectRenderingStructure = &objectTypeIterator->second;
		glDeleteBuffers(1, &objectRenderingStructure->modelVbo);
		glDeleteBuffers(1, &objectRenderingStructure->modelOriginOffsetVbo);
		glDeleteBuffers(1, &objectRenderingStructure->colorVbo);
		glDeleteBuffers(1, &objectRenderingStructure->transformVbo);
		glDeleteVertexArrays(1, &objectRenderingStructure->masterVao);
		objectTypeIterator++;
	}

}
