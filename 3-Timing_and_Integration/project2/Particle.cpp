#define _USE_MATH_DEFINES
#include <cmath>
#include "Particle.h"


// Default constructor: Creates a particle represented by a default (square)
// Notes:
// - Particle rotated so that it is orthogonal to the z-axis.
// - Scaled to 0.1 on all axes.
// - No shader allocated by default to avoid creating a shader object for each particle.
Particle::Particle()
{
	setMesh(Mesh::Mesh(Mesh::MeshType::QUAD));
	scale(glm::vec3(0.1f, 0.1f, 0.1f));
	rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));

	// Set Dynamic Variables
	setAcc(glm::vec3(0.0f, 0.0f, 0.0f));
	setVel(glm::vec3(0.0f, 0.0f, 0.0f));

	// Physical Properties
	setMass(0.1f);
	setCor(1.0f);
}


Particle::~Particle()
{
}
