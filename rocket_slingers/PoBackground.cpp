#include "PoBackground.hpp"

PoBackground::PoBackground(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();

	cameraPosition = gameState->camera->getPosition();
	gameState->eventBus->subscribeToGameEvent(Event::GameEvent::CAMERA_MOVED, this);
}

void PoBackground::gameEventCallback(const Event& eventObj) {

	// update parallax layer positions based on camera movement
	glm::vec3 translation = glm::vec3(cameraPosition.x - eventObj.eventWorldCoordinateX, cameraPosition.y - eventObj.eventWorldCoordinateY, 0.0f);
	for (unsigned int i = 0; i < parallaxLayers; ++i) {
		transformData[i] = glm::translate(transformData[i], translation * (float) (i + 1) * 0.1f);
	}
	cameraPosition = glm::vec3(eventObj.eventWorldCoordinateX, eventObj.eventWorldCoordinateY, 0.0f);

}

void PoBackground::initGeometry() {

	zDepth = -1.0f;
	sizeScaler = 1.0f;
	float xScaler = sizeScaler * gameState->worldViewportWidth;
	float yScaler = sizeScaler * gameState->worldViewportHeight;

	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 0.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(0.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);

	/*
	they don't tile :( 
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 0.0f * yScaler, 0.0f));
	modelVertexData.push_back(glm::vec3(2.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(2.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 0.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(2.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	modelVertexData.push_back(glm::vec3(1.0f * xScaler, 1.0f * yScaler, 0.0f) * sizeScaler);
	*/

	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 1.0f));

	/*
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 1.0f));
	*/

	unsigned int backgroundId = 4;
	if (backgroundId == 1) {

		// greenish dark space
		parallaxLayers = 3;
		std::string fileName = "resources/backgrounds/background_01_parallax_01.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
		fileName = "resources/backgrounds/background_01_parallax_02.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
		fileName = "resources/backgrounds/background_01_parallax_05.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());

	}
	else if (backgroundId == 2) {

		// blue dark space with planet
		parallaxLayers = 2;
		std::string fileName = "resources/backgrounds/background_02_parallax_01.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
		fileName = "resources/backgrounds/background_02_parallax_02.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
	}
	else if (backgroundId == 3) {

		// dark purple space
		parallaxLayers = 3;
		std::string fileName = "resources/backgrounds/background_03_parallax_01.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
		fileName = "resources/backgrounds/background_03_parallax_02.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
		fileName = "resources/backgrounds/background_03_parallax_03.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
	}
	else if (backgroundId == 4) {

		// colorful space
		parallaxLayers = 2;
		std::string fileName = "resources/backgrounds/background_04_parallax_01.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());
		fileName = "resources/backgrounds/background_04_parallax_04.png";
		generateTexture(fileName);
		transformData.push_back(glm::mat4());

	}

		/*
	parallaxLayers = 2;
	for (unsigned int i = 0; i < parallaxLayers; ++i) {
		//std::string fileName = "resources/backgrounds/background_01_parallax_0" + std::to_string(i + 1) + ".png"; // blue space with planet
		//std::string fileName = "resources/backgrounds/background_02_parallax_0" + std::to_string(i + 1) + ".png"; // blue space with planet
		//std::string fileName = "resources/backgrounds/background_03_parallax_0" + std::to_string(i + 1) + ".png";  // dark space
		std::string fileName = "resources/backgrounds/background_04_parallax_0" + std::to_string(i + 1) + ".png";  // colorful space

		generateTexture(fileName);
		transformData.push_back(glm::mat4());
	}
	*/

	initModelVertexBuffer();
	initTextureCoordinateBuffer();
	buildVao(MODEL_VERTEX | TEXTURE_COORDINATE);
	shouldRender = true;

	projectionTransform = gameState->camera->getProjectionTransform();

	gameState->masterRenderer->addRenderableObject(this);
}

void PoBackground::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec2 textureCoordinate;\n"
		"\n"
		"out vec2 fragTextureCoordinate;\n"
		"\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex, 1.0f);\n"
		"    fragTextureCoordinate = vec2(textureCoordinate.x, 1.0f - textureCoordinate.y);\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec2 fragTextureCoordinate;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = texture(texture0, fragTextureCoordinate);\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

}

void PoBackground::render() {
	
	shaderProg.use();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(masterVao);

	for (unsigned int i = 0; i < parallaxLayers; i++) {
		glm::mat4 transform = projectionTransform * transformData[i];
		setUniformValue("transformMatrix", transform);
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		glDrawArrays(glRenderingMode, 0, modelVertexData.size());
	}

	abortOnOpenGlError();

}
