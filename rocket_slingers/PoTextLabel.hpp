#ifndef POTEXTLABEL_HPP
#define POTEXTLABEL_HPP

#include "PhysicalObject.hpp"
#include <map>

class PoTextLabel : public PhysicalObject {
public:

	struct TypeFaceCharacter {
		unsigned int textureId;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};
	
	PoTextLabel(const std::string& objectId, GameState* gameState);
	void render();
	void setTextValue(const std::string& value);
	void setColor(const glm::vec4& color);
	void setSizeScaler(float sizeScaler);
	void setWorldPosition(const glm::vec3& position);
	~PoTextLabel();

private:

	static std::map<unsigned char, TypeFaceCharacter> initTypeFaceCharactersMap();
	static std::map<unsigned char, TypeFaceCharacter> typeFaceCharacters;

	std::string currentValue;
	glm::vec3 worldPosition;
	float sizeScaler;
	glm::mat4 projectionTransform;

	void initShaders();
	void initGeometry();
};

#endif
