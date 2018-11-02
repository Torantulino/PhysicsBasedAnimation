#pragma once
#include "Body.h"

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	// - SETTERS -
	void setAngVel(const glm::vec3 &omega) { m_angVel = omega; }
	void setAngAccl(const glm::vec3 &alpha) { m_angAcc = alpha; }
	void setInvIneria(const glm::mat3 &invInertia) { m_invInertia = invInertia; }

	// - GETTERS -
	glm::vec3 getAngVel() { return m_angVel; }
	glm::vec3 getAngAcc() { return m_angAcc; }
	glm::mat3 getInvInertia() { return m_invInertia; }

	// - METHODS -
	void scale(glm::vec3 vect);

private:
	float m_density;
	glm::mat3 m_invInertia;		//Inverse inertia
	glm::vec3 m_angVel;			//Angular velocity
	glm::vec3 m_angAcc;			//Angular acceleration
};

