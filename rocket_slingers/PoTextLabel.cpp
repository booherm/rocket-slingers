#include "PoTextLabel.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

std::map<unsigned char, PoTextLabel::TypeFaceCharacter> PoTextLabel::initTypeFaceCharactersMap() {

	std::map<unsigned char, PoTextLabel::TypeFaceCharacter> faceCharacters;

	FT_Library freeType;
	if (FT_Init_FreeType(&freeType))
		throw std::string("PoTextLabel::initGeometry - free type init error");

	FT_Face typeFace;
	if (FT_New_Face(freeType, "c:/1/consola.ttf", 0, &typeFace))
		throw std::string("PoTextLabel::initGeometry - font file load error");

	if (FT_Set_Pixel_Sizes(typeFace, 0, 48))
		throw std::string("PoTextLabel::initGeometry - typeface size error");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(typeFace, c, FT_LOAD_RENDER))
			throw std::string("PoTextLabel::initGeometry - typeface character load error");

		// Generate texture
		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			typeFace->glyph->bitmap.width,
			typeFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			typeFace->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		TypeFaceCharacter character = {
			textureId,
			glm::ivec2(typeFace->glyph->bitmap.width, typeFace->glyph->bitmap.rows),
			glm::ivec2(typeFace->glyph->bitmap_left, typeFace->glyph->bitmap_top),
			(unsigned int)typeFace->glyph->advance.x
		};

		faceCharacters[c] = character;
	}

	if (FT_Done_Face(typeFace))
		throw std::string("PoTextLabel::initGeometry - typeface release error");

	if (FT_Done_FreeType(freeType))
		throw std::string("PoTextLabel::initGeometry - freetype release error");

	std::cout << "init face chars called" << std::endl;

	return faceCharacters;
}

std::map<unsigned char, PoTextLabel::TypeFaceCharacter> PoTextLabel::typeFaceCharacters;

PoTextLabel::PoTextLabel(GameState* gameState) : PhysicalObject("PO_TEXT_LABEL", gameState) {

	if (typeFaceCharacters.size() == 0) {
		typeFaceCharacters = initTypeFaceCharactersMap();
	}

	currentValue = "";
	sizeScaler = 0.01f;
	useCustomRenderer = true;

	initShaders();
	initGeometry();
	initPhysics();
}

void PoTextLabel::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 modelOriginOffset;\n"
		"layout (location = 2) in vec4 inColor;\n"
		"layout (location = 3) in mat4 transformMatrix;\n"
		"layout (location = 7) in vec2 inTextureCoordinate;\n"
		"\n"
		"out vec4 fragShaderColor;\n"
		"out vec2 fragTextureCoordinate;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(position, 1.0f);\n"
		"    fragShaderColor = inColor;\n"
		"    fragTextureCoordinate = vec2(inTextureCoordinate.x, 1.0f - inTextureCoordinate.y);\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec4 fragShaderColor;\n"
		"in vec2 fragTextureCoordinate;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"uniform sampler2D texture0;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    float presenceAlpha = texture(texture0, fragTextureCoordinate).r;\n"
		"    color = vec4(fragShaderColor.r, fragShaderColor.g, fragShaderColor.b, fragShaderColor.a * presenceAlpha);\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

}

void PoTextLabel::initGeometry() {
	/*
	FT_Library freeType;
	if (FT_Init_FreeType(&freeType))
		throw std::string("PoTextLabel::initGeometry - free type init error");

	FT_Face typeFace;
	if (FT_New_Face(freeType, "c:/1/consola.ttf", 0, &typeFace))
		throw std::string("PoTextLabel::initGeometry - font file load error");

	if (FT_Set_Pixel_Sizes(typeFace, 0, 48))
		throw std::string("PoTextLabel::initGeometry - typeface size error");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(typeFace, c, FT_LOAD_RENDER))
			throw std::string("PoTextLabel::initGeometry - typeface character load error");

		// Generate texture
		unsigned int textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			typeFace->glyph->bitmap.width,
			typeFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			typeFace->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		TypeFaceCharacter character = {
			textureId,
			glm::ivec2(typeFace->glyph->bitmap.width, typeFace->glyph->bitmap.rows),
			glm::ivec2(typeFace->glyph->bitmap_left, typeFace->glyph->bitmap_top),
			(unsigned int) typeFace->glyph->advance.x
		};

		typeFaceCharacters[c] = character;
	}

	if (FT_Done_Face(typeFace))
		throw std::string("PoTextLabel::initGeometry - typeface release error");

	if (FT_Done_FreeType(freeType))
		throw std::string("PoTextLabel::initGeometry - freetype release error");
	*/

	// init color and model vertices buffers for size
	modelVertices.resize(6);
	colorData.resize(6);
	
	// init texture coordinate data
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 0.0f));
	textureCoordinateData.push_back(glm::vec2(1.0f, 1.0f));
	textureCoordinateData.push_back(glm::vec2(0.0f, 1.0f));

	transformData.push_back(gameState->camera->getProjectionTransform());
	gameState->physicalObjectRenderer->addPhysicalObject(this);
	
	setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	// buffer transform data
	unsigned int transformDataSize = sizeof(glm::mat4) * transformData.size();
	glBindBuffer(GL_ARRAY_BUFFER, renderingStructure->transformVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, transformDataSize, transformData.data());
	abortOnOpenGlError();

	shouldRender = true;
}

void PoTextLabel::initPhysics() {
}

void PoTextLabel::doPhysicalUpdate() {
}

void PoTextLabel::doRenderUpdate() {
}

void PoTextLabel::customRender() {

	shaderProg.use();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(renderingStructure->masterVao);
	float charXPosition = worldPosition.x;
	abortOnOpenGlError();

	for (unsigned int i = 0; i < currentValue.length(); ++i) {

		TypeFaceCharacter typeFaceChar = typeFaceCharacters[currentValue[i]];

		float xPosition = charXPosition + (typeFaceChar.bearing.x * sizeScaler);
		float yPosition = worldPosition.y - ((typeFaceChar.size.y - typeFaceChar.bearing.y) * sizeScaler);
		float width = typeFaceChar.size.x * sizeScaler;
		float height = typeFaceChar.size.y * sizeScaler;

		// vertex locations
		modelVertices.clear();
		modelVertices.push_back(glm::vec3(xPosition, yPosition, 0.0f));                  // bottom left
		modelVertices.push_back(glm::vec3(xPosition + width, yPosition, 0.0f));          // bottom right
		modelVertices.push_back(glm::vec3(xPosition + width, yPosition + height, 0.0f)); // top right
		modelVertices.push_back(glm::vec3(xPosition, yPosition, 0.0f));                  // bottom left
		modelVertices.push_back(glm::vec3(xPosition + width, yPosition + height, 0.0f)); // top right
		modelVertices.push_back(glm::vec3(xPosition, yPosition + height, 0.0f));         // top left

		// buffer model
		glBindBuffer(GL_ARRAY_BUFFER, renderingStructure->modelVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * 6, modelVertices.data());
		abortOnOpenGlError();

		// render
		glBindTexture(GL_TEXTURE_2D, typeFaceChar.textureId);
		glDrawArraysInstanced(glRenderingMode, 0, 6, 1);
		abortOnOpenGlError();

		// advance to next glyph
		charXPosition += (typeFaceChar.advance >> 6) * sizeScaler;
	}

	glBindVertexArray(0);
	abortOnOpenGlError();
}

void PoTextLabel::abortOnOpenGlError() {
	GLenum openGlErrorCode = glGetError();
	if (openGlErrorCode != GL_NO_ERROR) {
		std::cout << "opengl error: " << openGlErrorCode << std::endl;
		throw "PhysicalObjectRenderer::render - OpenGL error: " + openGlErrorCode;
	}
}

void PoTextLabel::setTextValue(const std::string& value) {
	this->currentValue = value;
}

void PoTextLabel::setColor(const glm::vec4& color) {

	colorData.clear();
	for (unsigned int i = 0; i < 6; ++i)
		colorData.push_back(color);

	// buffer color data
	glBindBuffer(GL_ARRAY_BUFFER, renderingStructure->colorVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * 6, colorData.data());
	abortOnOpenGlError();

}

void PoTextLabel::setSizeScaler(float sizeScaler) {
	this->sizeScaler = sizeScaler;
}

void PoTextLabel::setWorldPosition(const glm::vec3& position) {
	worldPosition = position;
}

PoTextLabel::~PoTextLabel() {
}
