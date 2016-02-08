#include "Utilities.hpp"
#include <gtc/matrix_transform.hpp>

float Utilities::angleBetweenVectors(glm::vec2 baseVector, glm::vec2 toVector) {
	float theta = glm::acos(glm::dot(baseVector, toVector) / (glm::length(baseVector) * glm::length(toVector)));
	if (toVector.y < baseVector.y)
		theta = glm::two_pi<float>() - theta;

	return theta;
}
