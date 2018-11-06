#pragma once
#include "Body.h"
#include "Impulse.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

class RigidBody :
	public Body
{
public:
	RigidBody();
	~RigidBody();

	// - SETTERS -
	void setAngVel(const glm::vec3 &omega) { m_angVel = omega; }
	void setAngAcc(const glm::vec3 &alpha) { m_angAcc = alpha; }
	void setCoM(glm::vec3 CoM) { m_CoM = CoM; }
	void setInvIneria(const glm::mat3 &invInertia) { m_invInertia = invInertia; }
	void setInvIneria() { m_invInertia = Body::getRotate() * glm::inverse(m_InertiaBS) * glm::transpose(Body::getRotate()); } //is T the Translation matrix?
	void setInertiaBS() {
		m_InertiaBS = glm::mat3((
			(1.0f / 12.0f) * Body::getMass() * (pow(Body::getScale()[1][1], 2.0f) + pow(Body::getScale()[2][2], 2.0f))), 0.0f, 0.0f,
			0.0f, ((1.0f / 12.0f) * Body::getMass() * (pow(Body::getScale()[0][0], 2.0f) + pow(Body::getScale()[2][2], 2.0f))), 0.0f,
			0.0f, 0.0f, ((1.0f / 12.0f) * Body::getMass() * (pow(Body::getScale()[0][0], 2.0f) + pow(Body::getScale()[1][1], 2.0f))));
	}

	// - GETTERS -
	glm::vec3 getAngVel() { return m_angVel; }
	glm::vec3 getAngAcc() { return m_angAcc; }
	glm::vec3 getCoM() { return m_CoM; }
	glm::mat3 getInvInertia() { return m_invInertia; }

	// - METHODS -
	void scale(glm::vec3 vect) { Body::scale(vect); setInertiaBS(); setInvIneria(); }						//Update body space inertia tensor and inverse global space inertia tensor
	void setMass(float mass) { Body::setMass(mass); setInertiaBS(); setInvIneria(); }						//Update body space inertia tensor and inverse global space inertia tensor
	void setRotate(const glm::mat4 &mat) { Body::setRotate(mat); setInvIneria(); }						//Update only inverse global space inertia tensor
	void rotate(float angle, const glm::vec3 &vect) { Body::rotate(angle, vect); setInvIneria(); }		//Update only inverse global space inertia tensor

	//Properties
	std::vector<Impulse> impulses;	//Impulses due to act on this object

private:
	float m_density;
	glm::mat3 m_InertiaBS;				//Inertia tensor in body space (Only needs to be recalculated when the dimensions or mass of the rb are altered.
	glm::mat3 m_invInertia;				//Inverse inertia
	glm::vec3 m_angVel;					//Angular velocity
	glm::vec3 m_angAcc;					//Angular acceleration
	glm::vec3 m_CoM;					//Center of mass
};

