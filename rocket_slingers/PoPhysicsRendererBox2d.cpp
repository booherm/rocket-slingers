#include "PoPhysicsRendererBox2d.hpp"

PoPhysicsRendererBox2d::PoPhysicsRendererBox2d(const std::string& objectId, GameState* gameState) : PhysicalObject(objectId, gameState) {
	initShaders();
	initGeometry();
	gameState->physicsManager->setBox2dDebugRenderer(this);
}

void PoPhysicsRendererBox2d::initShaders() {

	// vertex shader
	std::string vertexShaderSource =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 modelVertex;\n"
		"layout (location = 1) in vec4 colorValue;\n"
		"\n"
		"out vec4 fragColor;\n"
		"\n"
		"uniform mat4 transformMatrix;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    gl_Position = transformMatrix * vec4(modelVertex, 1.0f);\n"
		"    fragColor = colorValue;\n"
		"}\n";

	// fragment shader
	std::string fragmentShaderSource =
		"#version 330 core\n"
		"\n"
		"in vec4 fragColor;\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    color = fragColor;\n"
		"}\n";

	shaderProg = OglShaderProgram();
	shaderProg.createVertexShaderFromSourceString(vertexShaderSource);
	shaderProg.createFragmentShaderFromSourceString(fragmentShaderSource);
	shaderProg.build();

}

void PoPhysicsRendererBox2d::initGeometry() {

	glRenderingMode = GL_LINES;

	zDepth = 1.0f;

	unsigned int maxVertices = 10000;
	initModelVertexBuffer(maxVertices);
	initColorBuffer(maxVertices);

	buildVao(MODEL_VERTEX | COLOR);
	gameState->masterRenderer->addRenderableObject(this);
	shouldRender = true;

}

PoPhysicsRendererBox2d::~PoPhysicsRendererBox2d() {};

/// Draw a closed polygon provided in CCW order.
void PoPhysicsRendererBox2d::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

	for (int i = 0; i < vertexCount - 1; i++) {
		DrawSegment(vertices[i], vertices[i + 1], color);
	}

	DrawSegment(vertices[vertexCount - 1], vertices[0], color);

}

/// Draw a solid closed polygon provided in CCW order.
void PoPhysicsRendererBox2d::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	for (int i = 0; i < vertexCount - 1; i++) {
		DrawSegment(vertices[i], vertices[i + 1], color);
	}

	DrawSegment(vertices[vertexCount - 1], vertices[0], color);

}

/// Draw a circle.
void PoPhysicsRendererBox2d::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {

	unsigned int resolution = 50;
	float increment = glm::two_pi<float>() / resolution;

	b2Vec2* vertices = new b2Vec2[resolution];
	
	for (unsigned int i = 0; i < resolution; i++) {
		vertices[i] = b2Vec2(
			center.x + (radius * glm::cos(i * increment)),
			center.y + (radius * glm::sin(i * increment))
		);
	}

	DrawPolygon(vertices, resolution, color);

	delete vertices;
}

/// Draw a solid circle.
void PoPhysicsRendererBox2d::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
	DrawCircle(center, radius, color);
}

/// Draw a line segment.
void PoPhysicsRendererBox2d::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	modelVertexData.push_back(glm::vec3(p1.x, p1.y, 0.0f));
	modelVertexData.push_back(glm::vec3(p2.x, p2.y, 0.0f));
	colorData.push_back(glm::vec4(color.a, color.r, color.g, color.a));
	colorData.push_back(glm::vec4(color.a, color.r, color.g, color.a));

}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void PoPhysicsRendererBox2d::DrawTransform(const b2Transform& xf) {
}



void PoPhysicsRendererBox2d::render() {

	refreshModelVertexBuffer();
	refreshColorBuffer();

	glm::mat4 viewTransform = gameState->camera->getViewTransform();
	glm::mat4 projectionTransform = gameState->camera->getProjectionTransform();
	glm::mat4 transform = projectionTransform * viewTransform;

	shaderProg.use();
	setUniformValue("transformMatrix", transform);
	glBindVertexArray(masterVao);
	glDrawArrays(glRenderingMode, 0, modelVertexData.size());

	modelVertexData.clear();
	colorData.clear();

	abortOnOpenGlError();
}
