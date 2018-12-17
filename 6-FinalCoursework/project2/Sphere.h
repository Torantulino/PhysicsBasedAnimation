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
	unsigned int getCellX() { return cellX; }
	unsigned int getCellZ() { return cellZ; }

	//- SETTERS -
	void setCellX(unsigned int x) { cellX = x; }
	void setCellZ(unsigned int z) { cellZ = z; }

	unsigned int ID;

private:
	float m_radius;
	glm::vec3 m_center;
	unsigned int cellX;
	unsigned int cellZ;
};

