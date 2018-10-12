#include "Force.h"



Force::Force()
{
}


Force::~Force()
{
}

//NOTE:The following methods are provided with more arguments than a single one may require to allow for all force types


glm::vec3 Force::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	return glm::vec3(0.0f, 0.0f, 0.0f);
}

// - Gravity -
//Returns the force due to gravity on a body
glm::vec3 Gravity::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	//F = ma
	glm::vec3 Fg = mass * m_gravity;
}

// - Drag -
//Returns the force due to drag on a body
glm::vec3 Drag::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	glm::vec3 Faero = (airDensity * pow(length(vel), 2) *  Cd * pCrossSecArea * -glm::normalize(vel)) / 2.0f;
}