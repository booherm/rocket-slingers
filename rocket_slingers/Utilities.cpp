#include "Utilities.hpp"
#include <gtc/matrix_transform.hpp>

float Utilities::xyAngleBetweenVectors(glm::vec3 baseVector, glm::vec3 toVector) {
	// throw out the z dimension
	baseVector.z = 0.0f;
	toVector.z = 0.0f;

	float theta = glm::acos(glm::dot(baseVector, toVector) / (glm::length(baseVector) * glm::length(toVector)));
	if (toVector.y < baseVector.y)
		theta = glm::two_pi<float>() - theta;

	return theta;
}

float Utilities::xyAngleBetweenVectors(const b2Vec2& baseVector, const b2Vec2& toVector) {
	
	float theta = glm::acos(b2Dot(baseVector, toVector) / (baseVector.Length() * toVector.Length()));
	if (toVector.y < baseVector.y)
		theta = glm::two_pi<float>() - theta;

	return theta;
}

std::string Utilities::vectorToString(const glm::vec3& vector) {
	return "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z) + ")";
}

std::string Utilities::vectorToString(const b2Vec2& vector) {
	return "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ")";
}

b2Vec2 Utilities::glmVec3ToB2Vec2(const glm::vec3& vector) {
	return b2Vec2(vector.x, vector.y);
}

glm::vec3 Utilities::b2Vec2ToGlmVec3(const b2Vec2& vector) {
	return glm::vec3(vector.x, vector.y, 0.0f);
}

b2Vec2 operator*(const b2Vec2& vector, float value) {
	return b2Vec2(vector.x * value, vector.y * value);
}

b2Vec2 operator/(const b2Vec2& vector, float value) {
	return b2Vec2(vector.x / value, vector.y / value);
}
