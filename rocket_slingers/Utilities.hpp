#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <glm.hpp>
#include <Box2D/Box2D.h>
#include <string>

namespace Utilities {

	// gives the angle between the x and y components of 3d vectors
	float xyAngleBetweenVectors(glm::vec3 baseVector, glm::vec3 toVector);
	float xyAngleBetweenVectors(const b2Vec2& baseVector, const b2Vec2& toVector);

	std::string vectorToString(const glm::vec3& vector);
	std::string vectorToString(const b2Vec2& vector);
	b2Vec2 glmVec3ToB2Vec2(const glm::vec3& vector);
	glm::vec3 b2Vec2ToGlmVec3(const b2Vec2& vector);
}

b2Vec2 operator*(const b2Vec2& vector, float value);
b2Vec2 operator/(const b2Vec2& vector, float value);

#endif
