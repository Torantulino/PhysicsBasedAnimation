#include "Force.h"
#include "Body.h"


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
	return Fg;
}

// - Drag -
//Returns the force due to drag on a body
glm::vec3 Drag::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	glm::vec3 Faero = (airDensity * pow(length(vel), 2) *  Cd * pCrossSecArea * -glm::normalize(vel)) / 2.0f;
	return Faero;
}

// - Hooke's Law -
//Returns the 
glm::vec3 Hooke::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	//Get distance between the two bodies
	float curDist = length(m_b1->getPos() - m_b2->getPos());

	//Find the spring's displacement from rest position
	float displacement = curDist - m_rest;


	//if (displacement > m_rest * 2) {
	//	m_b1->setPos(m_b1->getPrevPos());

	//	// - Get values again -
	//	//Get distance between the two bodies
	//	curDist = length(m_b1->getPos() - m_b2->getPos());

	//	//Find the spring's displacement from rest position
	//	displacement = curDist - m_rest;
	//}

	//Calculate spring force
	float f = m_ks * displacement;

	//Calculate scalar projection of velocity along spring axis
	float projVel = dot(vel, m_b2->getPos() - m_b1->getPos());

	//Calculate damping force
	f -= m_kd * projVel;


	//Store position for next loop
	m_b1->setPrevPos();

	//Direct final force along spring vector
	glm::vec3 Fspring = glm::normalize(m_b2->getPos() - m_b1->getPos()) * f;
	return Fspring;
}