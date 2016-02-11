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


std::string Utilities::vectorToString(glm::vec3 vector) {
	return "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ", " + std::to_string(vector.z) + ")";
}