#ifndef RENDERABLEOBJECT_HPP
#define RENDERABLEOBJECT_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <iostream>
#include <vector>
#include "OglShaderProgram.hpp"

class RenderableObject {
public:
	bool shouldRender;
	float zDepth;
	std::string id;

	RenderableObject(const std::string& id);
	virtual void render() = 0;
	~RenderableObject();

protected:

	enum VaoComponent {
		MODEL_VERTEX = 1,
		COLOR = 4,
		TEXTURE_COORDINATE = 8
	};

	unsigned int glRenderingMode;
	unsigned int masterVao;
	unsigned int modelVertexVbo;
	unsigned int colorVbo;
	unsigned int textureCoordinateVbo;
	std::vector<glm::vec3> modelVertexData;
	std::vector<glm::vec4> colorData;
	std::vector<glm::vec2> textureCoordinateData;
	std::vector<glm::mat4> transformData;
	std::vector<unsigned int> textureIds;
	OglShaderProgram shaderProg;

	virtual void initShaders() = 0;
	virtual void initGeometry() = 0;

	void initModelVertexBuffer();
	void initModelVertexBuffer(unsigned int reservedSize);
	void initColorBuffer();
	void initColorBuffer(unsigned int reservedSize);
	void initTextureCoordinateBuffer();

	void refreshModelVertexBuffer();
	void refreshColorBuffer();
	void refreshTextureCoordinateBuffer();

	void buildVao(unsigned int vaoComponents);
	void setUniformValue(const std::string& uniformId, const glm::mat4& mat4);
	void setUniformValue(const std::string& uniformId, const glm::vec3& vec3);
	void generateTexture(const std::string& fileName);
	void abortOnOpenGlError();

private:
	std::vector<unsigned int> generatedBufferIds;
	std::vector<unsigned int> generatedVaoIds;

};

#endif
