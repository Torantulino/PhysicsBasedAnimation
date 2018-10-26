#include "Force.h"
#include "Body.h"
#include "Triangle.h"

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

	//Calculate spring force
	float f = m_ks * displacement;

	//Calculate scalar projection of relative velocity along spring axis
	//float projVel = dot(m_b1->getVel() - m_b2->getVel(), m_b2->getPos() - m_b1->getPos())/length(m_b2->getPos() - m_b1->getPos());
	float projVel = dot(m_b1->getVel() - m_b2->getVel(), glm::normalize(m_b2->getPos() - m_b1->getPos()));

	//Calculate damping force
	f -= m_kd * projVel;

	//Store position for next loop
	m_b1->setPrevPos();

	//Direct final force along spring vector
	glm::vec3 Fspring = glm::normalize(m_b2->getPos() - m_b1->getPos()) * f;
	return Fspring;
}

// - Wind Force -
glm::vec3 Wind::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	// Variables:
	// coneTip
	// coneAxis 
	// coneRad 
	// coneHeight

	// - Determine position within cone -

	//Calculate the distance along the 'height'/spine of the cone
	float projToAxis = dot(pos - m_coneTip, m_coneAxis);

	//If 'above' or 'below' cone, force is 0 (outsideb  the cone)
	if (projToAxis >= m_coneHeight || projToAxis <= 0) {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//Calculate the radius of the cone at the current projection
	float radAtProj = (projToAxis / m_coneHeight) * m_coneRad;

	//Calculate the current point's distance/radius from the spine of the cone
	float distFromSpine = length((pos - m_coneTip) - projToAxis * m_coneAxis);

	//If this distance is greater than the radius at the current point then it is outside the cone.
	if (distFromSpine >= radAtProj) {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// - Calculate force magnitude based on position within cone -
	//Calculate falloff due to 'height' along spine
	float hFalloff = 1 - (projToAxis / m_coneHeight);
	//Calculate falloff due to radial distance
	float rFalloff = 1 - (distFromSpine / radAtProj);
	// Calculate total falloff
	float tFalloff = hFalloff * rFalloff;

	// - Calculate force direction -
	//Get vector from cone tip to particle position.
	glm::vec3 tipToPos = pos - m_coneTip;
	//Use normalized vector as direction away from tip
	glm::vec3 windVel = glm::normalize(tipToPos) * tFalloff;
	
	windVel *= 10.0f;
	
	//Calculate velocity relative to wind
	glm::vec3 V = m_tri->getVel() - windVel;
	//Calculate cross-sectional area
	float cArea = m_tri->getArea() * (glm::dot(V, m_tri->getNormal()) / glm::length(V));

	glm::vec3 fAero = -(0.5f) * m_DoM * pow(glm::length(V), 2) * m_CoD * cArea * m_tri->getNormal();

	//Divide by 3 for particle
	fAero /= 3.0f;

	float rnd = ((double)rand() / (RAND_MAX)) + 1;

	fAero *= rnd;
	
	return fAero;
}
