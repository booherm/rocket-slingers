#include "PoBackground.hpp"
#include "SOIL.h"

PoBackground::PoBackground(GameState* gameState) : PhysicalObject("PO_BACKGROUND", gameState) {
	initGeometry();
	initShaders();
	shouldRender = true;
	gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoBackground::initGeometry() {


	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

	/*
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(100.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(100.0f, 100.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelVertices.push_back(glm::vec3(100.0f, 100.0f, 0.0f));
	modelVertices.push_back(glm::vec3(0.0f, 100.0f, 0.0f));
	*/

	colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colorData.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	colorData.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	colorData.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colorData.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	colorData.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 1.0f));

	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	textures.push_back(textureId);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
//	unsigned char* image = SOIL_load_image("c:\\1\\img_cheryl.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* image = SOIL_load_image("c:\\1\\img_cheryl.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	//	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	std::cout << "background init geometry" << std::endl;

}

void PoBackground::doRenderUpdate() {

	transformData.clear();

	// model transform: translate, scale, rotate
	glm::mat4 modelTransform;
//	modelTransform = glm::translate(modelTransform, mainComponentMass->worldPosition);
	modelTransform = glm::scale(modelTransform, glm::vec3(gameState->worldViewportScaler * gameState->aspectRatio, gameState->worldViewportScaler, 1.0f));

	// view
	glm::mat4 viewTransform = gameState->camera->getViewTransform();

	// projection
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();

	// combine transform
	glm::mat4 transform = projectionTransform * viewTransform * modelTransform;
	transformData.push_back(transform);

}

void PoBackground::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 modelOriginOffset;\n"
		"layout (location = 2) in vec4 inColor;\n"
		"layout (location = 3) in mat4 transformMatrix;\n"
		"layout (location = 4) in vec2 textureCoordinate;\n"
		"\n"
		"out vec4 fragShaderColor;\n"
		"out vec2 fragTextureCoordinate;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(position + modelOriginOffset, 1.0f);\n"
		"    fragShaderColor = inColor;\n"
		"    fragTextureCoordinate = textureCoordinate;\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec4 fragShaderColor;\n"
		"in vec2 fragTextureCoordinate;\n"
		"out vec4 color;\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = texture(texture0, fragTextureCoordinate);\n"
//		"    color = texture(texture0, fragTextureCoordinate).rbg;\n"
		//		"    color = texture(texture0, fragTextureCoordinate) * fragShaderColor;\n"  
//		"    color = fragShaderColor;\n"  
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

	std::cout << "background build shaders" << std::endl;

}
