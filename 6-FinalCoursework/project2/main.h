#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>
#include <iostream>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Particle.h"
#include "Triangle.h"
#include "RigidBody.h"
#include "Sphere.h"

glm::vec3 sumImpulsesLin(RigidBody &rb);

glm::vec3 sumImpulsesAng(RigidBody &rb);

void CheckCollisions(Particle &p, Mesh &cube);

void CheckCollisions(RigidBody &rb, Mesh &cube);

void CheckCollisions(Sphere &rb, Mesh &cube);

void CheckCollisions(std::vector<Sphere> &spheres);

void CollisionResponse(Sphere & rb, glm::vec3 &overShoot, glm::vec3 &planeNormal);

void CollisionResponse(Sphere & sp1, Sphere & sp2, float overshoot);

void CreateCloth(std::vector<std::vector<Particle> >& p2D, float stiffness, float damping, float rest);

Impulse calculateFriction(glm::vec3 vRel, glm::vec3 planeNormal, RigidBody &rb, glm::vec3 jn, glm::vec3 r);

std::vector<Triangle*> TriangulateGrid(std::vector<std::vector<Particle> > &p2D);

glm::vec3 calcConeForce(glm::vec3 pos);

void positionSpheres(std::vector<Sphere> spheres);


