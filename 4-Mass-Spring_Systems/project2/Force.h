#pragma once
#include <glm/glm.hpp>
#include <iostream>

class Body;

class Force
{
public:
	Force();
	~Force();

	virtual glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);
};

// - Gravity -
class Gravity : public Force {

public:
	//Constructors
	Gravity() {}
	Gravity(const glm::vec3 &gravity) { m_gravity = gravity; }

	//Getters
	glm::vec3 getGravity() const { return m_gravity; }
	
	//Setters
	void setGravity(glm::vec3 gravity) { m_gravity = gravity; }

	//Apply force
	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 & vel);

private:
	glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);
};

// - Drag -
class Drag : public Force {
public:
	Drag(){}

	//Apply force
	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);

private:
	float airDensity = 1.225;
	float Cd = 1.05;			//Drag coefficent for a cube
	float pCrossSecArea = 0.1f;	//Cross sectional area of a particle
};

// - Hooke's Law -
class Hooke : public Force {
public:
	Hooke() {
		Hooke(Body* b1, Body* b2, float ks, float kd, float rest) {

		}
	}
};

