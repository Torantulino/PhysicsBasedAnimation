#pragma once
#include <glm/glm.hpp>
#include <iostream>

class Triangle;
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
	Hooke() {}
	Hooke(Body* b1, Body* b2, float ks, float kd, float rest) {
		m_ks = ks; m_kd = kd; m_rest = rest; m_b1 = b1; m_b2 = b2;
	}

	//Getters
	Body* getB1() { return m_b1; }

	//Setters
	void setB1(Body* b) { m_b1 = b; }
	void setB2(Body* b) { m_b2 = b; }
	void setRest(float rest) { m_rest = rest; }
	void setStiffness(float stiff) { m_ks = stiff; }
	void setDamping(float damp) { m_kd = damp; }

	//Apply force
	glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);

private:
	float m_ks;		//Spring stiffness
	float m_kd;		//Damping coefficent
	float m_rest;	//Spring rest length

	Body* m_b1;	//Pointer to the body connected to one extremity of the spring
	Body* m_b2;	//Pointer to the body connected to the other extremity
};

class Wind : public Force {

	public:
		Wind() {}
		Wind(Triangle * tri, float DoM, float CoM) {
			setT(tri); m_DoM = DoM; m_CoD = CoM;
		}

		//Apply force
		glm::vec3 apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel);

		//Setters
		void setT(Triangle* t) { m_tri = t; }
		void setDoM(float dom) { m_DoM = dom; }
		void setCoD(float cod) { m_CoD = cod; }
		void setTri(Triangle *tri) { m_tri = tri; }

	private:
		Triangle* m_tri;	//Pointer to the triangle being acted upon
		float m_DoM;
		float m_CoD;
		glm::vec3 m_coneTip = glm::vec3(0.0f, -21.0f, 0.0f);
		glm::vec3 m_coneAxis = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_coneRad = 5.0f;
		float m_coneHeight = 20.0f;
		float m_coneForceScale = 5.0f;
};

