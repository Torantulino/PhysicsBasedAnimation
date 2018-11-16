#pragma once
#include "RigidBody.h"

class Sphere :
	public RigidBody
{
public:
	Sphere();
	~Sphere();


	//- GETTERS -
	float getRadius() { return m_radius; }
	glm::vec3 getCenter() { return m_center; }

private:
	float m_radius;
	glm::vec3 m_center;
};

