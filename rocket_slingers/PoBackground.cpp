#include "PoBackground.hpp"
#include "SOIL.h"
#include <gtc/type_ptr.hpp>

PoBackground::PoBackground(GameState* gameState) : PhysicalObject("PO_BACKGROUND", gameState) {
	initGeometry();
	initShaders();
	shouldRender = false;
	//gameState->physicalObjectRenderer->addPhysicalObject(this);
}

void PoBackground::initGeometry() {

	/*
	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // Top Left

		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left

	};
	*/

	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(0.5f, 0.5f, 0.0f));
	positions.push_back(glm::vec3(0.5f, -0.5, 0.0f));
	positions.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));
	positions.push_back(glm::vec3(0.5f, -0.5, 0.0f));
	positions.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	positions.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));

	std::vector<glm::vec3> modelOriginOffsets;
	modelOriginOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelOriginOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelOriginOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelOriginOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelOriginOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	modelOriginOffsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	std::vector<glm::vec3> colors;
	colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

	std::vector<glm::mat4> transforms;
	transforms.push_back(glm::mat4());
	transforms.push_back(glm::mat4());
	transforms.push_back(glm::mat4());
	transforms.push_back(glm::mat4());
	transforms.push_back(glm::mat4());
	transforms.push_back(glm::mat4());

	std::vector<glm::vec2> textureCoords;
	textureCoords.push_back(glm::vec2(1.0f, 1.0f));
	textureCoords.push_back(glm::vec2(1.0f, 0.0f));
	textureCoords.push_back(glm::vec2(0.0f, 1.0f));
	textureCoords.push_back(glm::vec2(1.0f, 0.0f));
	textureCoords.push_back(glm::vec2(0.0f, 0.0f));
	textureCoords.push_back(glm::vec2(0.0f, 1.0f));


	GLuint positionVbo, modelOriginOffsetVbo, colorVbo, transformVbo, textureCoordVbo;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &positionVbo);
	glGenBuffers(1, &modelOriginOffsetVbo);
	glGenBuffers(1, &colorVbo);
	glGenBuffers(1, &transformVbo);
	glGenBuffers(1, &textureCoordVbo);

	glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, modelOriginOffsetVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * modelOriginOffsets.size(), modelOriginOffsets.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transforms.size(), glm::value_ptr(transforms.data()[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textureCoords.size(), textureCoords.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	unsigned int vertexAttribIndex = 0;
	// Position attribute
	glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
	glVertexAttribPointer(vertexAttribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(vertexAttribIndex++);
	// model origin offset attribute
	glBindBuffer(GL_ARRAY_BUFFER, modelOriginOffsetVbo);
	glVertexAttribPointer(vertexAttribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(vertexAttribIndex++);
	// Color attribute
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glVertexAttribPointer(vertexAttribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(vertexAttribIndex++);

	// transforms attribute
	glBindBuffer(GL_ARRAY_BUFFER, transformVbo);
	glVertexAttribPointer(vertexAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)((0) * sizeof(glm::vec4)));
	glEnableVertexAttribArray(vertexAttribIndex++);
	glVertexAttribPointer(vertexAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)((1) * sizeof(glm::vec4)));
	glEnableVertexAttribArray(vertexAttribIndex++);
	glVertexAttribPointer(vertexAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)((2) * sizeof(glm::vec4)));
	glEnableVertexAttribArray(vertexAttribIndex++);
	glVertexAttribPointer(vertexAttribIndex, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)((3) * sizeof(glm::vec4)));
	glEnableVertexAttribArray(vertexAttribIndex++);

	// TexCoord attribute
	glBindBuffer(GL_ARRAY_BUFFER, textureCoordVbo);
	glVertexAttribPointer(vertexAttribIndex, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
	glEnableVertexAttribArray(vertexAttribIndex++);



	glBindVertexArray(0); // Unbind VAO


						  // Load and create a texture 
	
	// ====================
	// Texture 1
	// ====================
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
											// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("c:\\1\\background.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.



	std::cout << "background init geometry" << std::endl;

}

void PoBackground::doCustomRender() {
	// Activate shader
	shaderProg.use();

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shaderProg.getProgramId(), "ourTexture"), 0);

	// Draw container
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}
/*
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
*/

void PoBackground::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 modelOriginOffset;\n"
		"layout (location = 2) in vec3 color;\n"
		"layout (location = 3) in mat4 transform;\n"
		"layout (location = 7) in vec2 texCoord;\n"
		"\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = vec4(position + modelOriginOffset, 1.0f);\n"
		"    ourColor = color;\n"
		"    TexCoord = texCoord;\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"uniform sampler2D ourTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = texture(ourTexture, TexCoord);\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

	std::cout << "background build shaders" << std::endl;

}
