#include "Sphere.h"



Sphere::Sphere()
{
	m_radius = 1.0f;
	m_center = glm::vec3(0.0f, 0.0f, 0.0f);
	cellX = INT_MAX;
	cellZ = INT_MAX;
}


Sphere::~Sphere()
{
}
