#pragma once
#include <glm/glm.hpp>

class Impulse
{
public:
	Impulse();
	~Impulse();

	//Constructor
	Impulse(glm::vec3 dir, float mag, glm::vec3 PoA) { m_dir = dir; m_mag = mag; m_PoA = PoA; }

	// - GETTERS -
	glm::vec3 getDir() { return m_dir; }
	glm::vec3 getPoA() { return m_PoA; }
	float getMag() { return m_mag; }

	// - SETTERS -
	void setDir(glm::vec3 Imp) { m_dir = Imp; }
	void setPoA(glm::vec3 PoA) { m_PoA = PoA; }
	void setMag(float mag) { m_mag = mag; }

private:
	glm::vec3 m_dir;	//Direction of impulse
	glm::vec3 m_PoA;	//Point of application coordinates
	float m_mag;		//Magnitude of Impulse
};

