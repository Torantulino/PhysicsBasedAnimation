#include "RigidBody.h"



RigidBody::RigidBody()
{
	setMass(1.0f);
	m_CoM = glm::vec3(0.0f, 0.0f, 0.0f);
	m_CoF = 0.5f;
	setCor(1.0f);
}


RigidBody::~RigidBody()
{
}
