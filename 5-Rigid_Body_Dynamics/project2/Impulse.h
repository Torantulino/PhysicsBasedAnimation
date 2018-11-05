#pragma once
#include <glm/glm.hpp>

class Impulse
{
public:
	Impulse();
	~Impulse();

	//Constructor
	Impulse(glm::vec3 Imp, glm::vec3 PoA) { m_impulse = Imp; m_PoA = PoA;}

	// - GETTERS -
	glm::vec3 getValue() { return m_impulse; }
	glm::vec3 getPoA() { return m_PoA; }

	// - SETTERS -
	void setValue(glm::vec3 Imp) { m_impulse = Imp; }
	void setPoA(glm::vec3 PoA) { m_PoA = PoA; }

private:
	glm::vec3 m_impulse;	//Magnitude and direction of the impulse
	glm::vec3 m_PoA;		//Point of application coordinates
};

