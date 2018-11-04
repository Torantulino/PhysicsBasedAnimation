#pragma once
#include "Body.h"

class RigidBody :
	public Body
{
public:
	RigidBody();
	~RigidBody();

	// - SETTERS -
	void setAngVel(const glm::vec3 &omega) { m_angVel = omega; }
	void setAngAcc(const glm::vec3 &alpha) { m_angAcc = alpha; }
	void setInvIneria(const glm::mat3 &invInertia) { m_invInertia = invInertia; }
	void setInvIneria() { m_invInertia = getRotate() * m_InertiaBS * pow(getRotate(), getTranslate()); } //is T the Translation matrix?
	void setInertiaBS() { m_InertiaBS = glm::mat3((
						(1 / 12) * getMass() * (pow(getScale()[2][2], 2) + pow(getScale()[3][3], 2))), 0.0f, 0.0f,
						0.0f, ((1 / 12) * getMass() * (pow(getScale()[1][1], 2) + pow(getScale()[3][3], 2))), 0.0f,
						0.0f, 0.0f, ((1 / 12) * getMass() * (pow(getScale()[1][1], 2) + pow(getScale()[2][2], 2)))); }

	// - GETTERS -
	glm::vec3 getAngVel() { return m_angVel; }
	glm::vec3 getAngAcc() { return m_angAcc; }
	glm::mat3 getInvInertia() { return m_invInertia; }

	// - METHODS -
	void rbScale(glm::vec3 vect) { scale(vect); setInertiaBS(); setInvIneria(); }					//Update body space inertia tensor and inverse global space inertia tensor
	void rbSetMass(float mass) { setMass(mass); setInertiaBS(); setInvIneria(); }					//Update body space inertia tensor and inverse global space inertia tensor
	void rbSetRotate(const glm::mat4 &mat) { setRotate(mat); setInvIneria(); }						//Update only inverse global space inertia tensor
	void rbRotate(float angle, const glm::vec3 &vect) { rotate(angle, vect); setInvIneria(); }		//Update only inverse global space inertia tensor

private:
	float m_density;
	glm::mat3 m_InertiaBS;		//Inertia tensor in body space (Only needs to be recalculated when the dimensions or mass of the rb are altered.
	glm::mat3 m_invInertia;		//Inverse inertia
	glm::vec3 m_angVel;			//Angular velocity
	glm::vec3 m_angAcc;			//Angular acceleration
};

