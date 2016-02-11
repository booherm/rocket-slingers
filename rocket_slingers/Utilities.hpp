#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <glm.hpp>
#include <string>

namespace Utilities {

	// gives the angle between the x and y components of 3d vectors
	float xyAngleBetweenVectors(glm::vec3 baseVector, glm::vec3 toVector);

	std::string vectorToString(glm::vec3 vector);
}

#endif
