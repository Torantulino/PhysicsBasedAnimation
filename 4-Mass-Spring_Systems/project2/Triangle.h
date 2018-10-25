#pragma once

#include "Particle.h"

class Triangle
{
public:
	Triangle() {}
	~Triangle();

	Triangle(Particle* p1, Particle* p2, Particle* p3) { setP1(p1), setP2(p2), setP3(p3); }

	// - GETTERS -
	//Particles
	Particle* getP1() { return m_p1; }
	Particle* getP2() { return m_p2; }
	Particle* getP3() { return m_p3; }

	//Properties
	glm::vec3 getVel() { CalculateVelocity(); return m_velocity; }
	glm::vec3 getNormal() { CalculateNormal(); return m_normal; }
	float getArea() { CalculateArea(); return m_area; }


	// - SETTERS -
	//Particles
	void setP1(Particle* p) { m_p1 = p; }
	void setP2(Particle* p) { m_p2 = p; }
	void setP3(Particle* p) { m_p3 = p; }


private:
	//Particles (Pointers)
	Particle* m_p1;
	Particle* m_p2;
	Particle* m_p3;

	//Dynamic Properties
	glm::vec3 m_velocity;
	glm::vec3 m_normal;
	float m_area;

	//Methods
	void CalculateVelocity() { m_velocity = (m_p1->getVel() + m_p2->getVel() + m_p3->getVel()) / 3.0f; }
	void CalculateArea() { m_area = 1 / 2 * glm::length(glm::cross((m_p1->getPos() - m_p2->getPos()), (m_p3->getPos() - m_p1->getPos()))); }
	void CalculateNormal() { m_normal = glm::cross((m_p2->getPos() - m_p1->getPos()), (m_p3->getPos() - m_p1->getPos())) / glm::length(glm::cross((m_p2->getPos() - m_p1->getPos()), (m_p3->getPos() - m_p1->getPos()))); }
};

