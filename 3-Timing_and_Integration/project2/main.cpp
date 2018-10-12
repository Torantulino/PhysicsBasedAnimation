#pragma once
// Math constants
#define _USE_MATH_DEFINES
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
#include "main.h"

// time
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Physics Objects
std::vector<Particle> particles;

// Global Properties
glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);

//cone properties
glm::vec3 coneTip;
glm::vec3 coneAxis;
float coneRad;
float coneHeight;
float coneForceScale = 5.0f;

//other
unsigned int mode = 10;
bool firstLoop;
bool pause;

// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 10.0f));
			
	// - Create Cube -
	Mesh cube = Mesh::Mesh(Mesh::CUBE);
	// scale up x5
	cube.scale(glm::vec3(10.0f, 10.0f, 10.0f));
	//Set Shader
	Shader transLambert = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/physics.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/physics_transparent.frag");
	cube.setShader(transLambert);

	Shader blueParticle = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_blue.frag");
	Shader redParticle = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_red.frag");

	//Create cone
	//Mesh cone = Mesh::Mesh("resources/models/cone2.obj"); 
	//Set Shader
	//cone.setShader(transLambert);
	//cone.rotate(-(GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));	

	//// time
	const GLfloat timestep = 0.01f;
	GLfloat initTime = (GLfloat)glfwGetTime();
	GLfloat timeAccumulated = 0.0f;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// - MODE SWITCHING -
		// 0 - Test Sim
		if (glfwGetKey(app.getWindow(), GLFW_KEY_0)) {
			mode = 0;
			particles.clear();
			pause = true;

			//Create Particles
			for (int i = 0; i < 50; i++) {
				// Create particle 
				Particle p = Particle::Particle();
				// Set Shader
				p.getMesh().setShader(Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_blue.frag"));

				//Set initial position
				p.setPos(glm::vec3(0.0f, 10.0f, 0.0f));

				//Set Random initial velocity values
				float rndX = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				float rndY = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				float rndZ = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				p.setVel(glm::vec3(rndX * 50, rndY * 40, rndZ * 50));

				//Add particle to collection
				particles.push_back(p);
			}

			//Reset Time
			timeAccumulated = 0.0f;
		}

		// 1 - Bouncing Particle in Cube
		// 3 - Integration Comparison
		if (glfwGetKey(app.getWindow(), GLFW_KEY_3)) {
			//Set mode and clear vectors
			mode = 3;
			particles.clear();
			pause = true;

			// Create particle
			Particle pRef = Particle::Particle();
			Particle pFU = Particle::Particle();
			Particle pSIE = Particle::Particle();

			// Set Shader
			pRef.getMesh().setShader(Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_blue.frag"));
			pFU.getMesh().setShader(Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_blue.frag"));
			pSIE.getMesh().setShader(Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_blue.frag"));

			pRef.setCor(1.0f);
			pFU.setCor(1.0f);
			pSIE.setCor(1.0f);

			//Set initial position
			pRef.setPos(glm::vec3(-0.5f, -8.0f, 0.0f));
			pFU.setPos(glm::vec3(0.0f, -8.0f, 0.0f));
			pSIE.setPos(glm::vec3(0.5f, -8.0f, 0.0f));

			//Add particle to collection
			particles.push_back(pRef);
			particles.push_back(pFU);
			particles.push_back(pSIE);

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 4 - Blow Dryer
		if (glfwGetKey(app.getWindow(), GLFW_KEY_4)) {
			//Set mode and clear vectors
			mode = 4;
			particles.clear();
			firstLoop = true;
			pause = true;

			//Create Particles
			for (int i = 0; i < 1000; i++) {
				// Create particle 
				Particle p = Particle::Particle();
				// Set Shader
				p.getMesh().setShader(Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_blue.frag"));

				//Set initial position
				p.setPos(glm::vec3(0.0f, -6.0f, 0.0f));

				//Set Random initial velocity values
				float rndX = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				float rndY = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				float rndZ = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				p.setVel(glm::vec3(rndX * 5, 0.0f, rndZ * 5));

				p.setCor(0.9f);

				//Add particle to collection
				particles.push_back(p);
			}

			//Set Cone properties
			coneTip = glm::vec3(0.0f, -21.0f, 0.0f);
			coneAxis = glm::vec3(0.0f, 1.0f, 0.0f);
			coneRad = 5.0f;
			coneHeight = 20.0f;

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 5 - Grid Test
		if (glfwGetKey(app.getWindow(), GLFW_KEY_5)) {
			//Set mode and clear vectors
			mode = 5;
			particles.clear();
			firstLoop = true;
			pause = true;

			//Create Particles
			// X
			for (int x = -5; x < 6; x++) {
				//Z
				for (int z = -5; z < 6; z++) {
					//Y
					for (int y = -5; y < 6; y++) {
						// Create particle 
						Particle p = Particle::Particle();
						// Set Shader
						p.getMesh().setShader(Shader("E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/3-Timing_and_Integration/project2/resources/shaders/solid_blue.frag"));

						//Set initial position
						p.setPos(glm::vec3((float)x*2.0f, (float)y*2.0f, (float)z*2.0f));

						particles.push_back(p);
					}
				}
			}

			//Set Cone properties
			coneTip = glm::vec3(0.0f, -21.0f, 0.0f);
			coneAxis = glm::vec3(0.0f, 1.0f, 0.0f);
			coneRad = 5.0f;
			coneHeight = 20.0f;

			//Reset Time
 			timeAccumulated = 0.0f;
		}
		// - OTHER USER INTERACTION -
		//Scale force
		if (mode == 5 || mode == 4) {
			if (glfwGetKey(app.getWindow(), GLFW_KEY_EQUAL)) {
				coneForceScale += timestep;
			}
			if (glfwGetKey(app.getWindow(), GLFW_KEY_MINUS)) {
				coneForceScale -= timestep;
			}
		}
		//Start Simulation
		if (glfwGetKey(app.getWindow(), GLFW_KEY_SPACE)) {
			pause = false;
		}

		// - TIME -
		// Set frame time
		GLfloat currentTime = (GLfloat)glfwGetTime() - initTime;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor. TODO: Add user control of this variable.
		//currentTime *= 0.1f;
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//Add deltaTime to accumulator.
		timeAccumulated += deltaTime;

		//Work off accumulated time
		while (timeAccumulated >= timestep) {
			// - CAMERA -
			// Manage interaction
			app.doMovement(timestep);

			// - SIMULATE -

			// 0 - Test Simulation
			if (mode == 0 && !pause) {
				for (unsigned int i = 0; i < particles.size(); i++) {
					//Calculate Forces
					glm::vec3 force = particles[i].getMass() * g;
					//Calculate Accelleration
					particles[i].setAcc(force / particles[i].getMass());
					//Calculate Current Velocity
					particles[i].setVel(particles[i].getVel() + timestep * particles[i].getAcc());
					//Calculate New Position
					particles[i].translate(timestep * particles[i].getVel());

					CheckCollisions(particles[i], cube);
				}
			}
			//Integration Methods Demo
			if (mode == 3 && !pause) {
				for (unsigned int i = 0; i < particles.size(); i++) {
					// - Leave 0 still, simulate 1 and 2 -
					//Forward Euler Integration
					if (i == 1) {
						//Calculate Forces
						glm::vec3 force = particles[i].getMass() * g;
						//Calculate Accelleration
						particles[i].setAcc(force / particles[i].getMass());
						//Calculate Current Velocity
						glm::vec3 prevV = particles[i].getVel();
						particles[i].setVel(prevV + timestep * particles[i].getAcc());
						//Calculate New Position
						particles[i].translate(timestep * prevV);
					}
					//Semi-Implicit Euler Integration
					if (i == 2) {
						//Calculate Forces
						glm::vec3 force = particles[i].getMass() * g;
						//Calculate Accelleration
						particles[i].setAcc(force / particles[i].getMass());
						//Calculate Current Velocity
						particles[i].setVel(particles[i].getVel() + timestep * particles[i].getAcc());
						//Calculate New Position
						particles[i].translate(timestep * particles[i].getVel());
					}

					// - Collisions -
					CheckCollisions(particles[i], cube);
				}
			}
			if (mode == 4 && !pause) {
				for (unsigned int i = 0; i < particles.size(); i++) {
					//Calculate Forces
					glm::vec3 force = particles[i].getMass() * g;

					if (!firstLoop) {
						glm::vec3 coneF = calcConeForce(particles[i].getPos());
						//If outside the cone, colour blue.
						if (coneF == glm::vec3(0.0f, 0.0f, 0.0f)) {

							particles[i].getMesh().setShader(blueParticle);
						}
						//If inside, colour red.
						else {
							particles[i].getMesh().setShader(redParticle);
							force += coneF * coneForceScale;
						}
					}

					//Calculate Accelleration
					particles[i].setAcc(force / particles[i].getMass());
					//Calculate Current Velocity
					particles[i].setVel(particles[i].getVel() + timestep * particles[i].getAcc());
					//Calculate New Position
					particles[i].translate(timestep * particles[i].getVel());

					CheckCollisions(particles[i], cube);
				}
				firstLoop = false;
			}

			if (mode == 5 && glfwGetKey(app.getWindow(), GLFW_KEY_LEFT_SHIFT) && !pause) {
				for (unsigned int i = 0; i < particles.size(); i++) {

					//Calculate Forces
					glm::vec3 force = particles[i].getMass() * g;

					glm::vec3 coneF = calcConeForce(particles[i].getPos());
					if (coneF == glm::vec3(0.0f, 0.0f, 0.0f)) {

						particles[i].getMesh().setShader(blueParticle);
					}
					else {
						particles[i].getMesh().setShader(redParticle);
						force += coneF * coneForceScale;
					}


					//Calculate Accelleration
					particles[i].setAcc(force / particles[i].getMass());
					//Calculate Current Velocity
					particles[i].setVel(particles[i].getVel() + timestep * particles[i].getAcc());
					//Calculate New Position
					particles[i].translate(timestep * particles[i].getVel());
					//Check for collisions with the environment
					CheckCollisions(particles[i], cube);
				}
				firstLoop = false;
			}


			timeAccumulated -= timestep;
		}
		

		/*
		**	RENDER 
		*/		
		// clear buffer
		app.clear();

		// draw particles
		for each (Particle p in particles)
		{
			app.draw(p.getMesh());

		}


		//- Render Environment Last for transparency -
		
//		if (mode == 4 || mode == 5) {
//			app.draw(cone);
//		}
		
		app.draw(cube);
		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

//Check for particle collisions with a bouding cube.
void CheckCollisions(Particle &p, Mesh &cube)
{
	glm::vec3 particlePos = p.getPos();
	glm::vec3 cubePos = cube.getPos();
	glm::mat4 cubeScale = cube.getScale();

	//Check for collision with ground
	//Right
	if (particlePos.x > cubePos.x + cubeScale[0][0]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(cubePos.x + cubeScale[0][0], particlePos.y, particlePos.z));
		//Invert velocity in the y axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(-curVel.x, curVel.y, curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
	//Left
	if (particlePos.x < cubePos.x - cubeScale[0][0]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(cubePos.x - cubeScale[0][0], particlePos.y, particlePos.z));
		//Invert velocity in the y axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(-curVel.x, curVel.y, curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
	//Up
	if (particlePos.y > cubePos.y + cubeScale[1][1]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(particlePos.x, cubePos.y + cubeScale[1][1], particlePos.z));
		//Invert velocity in the y axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(curVel.x, -curVel.y, curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
	//Down
	if (particlePos.y < cubePos.y - cubeScale[1][1]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(particlePos.x, cubePos.y - cubeScale[1][1], particlePos.z));
		//Invert velocity in the y axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(curVel.x, -curVel.y, curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
	//Front
	if (particlePos.z > cubePos.z + cubeScale[2][2]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(particlePos.x, particlePos.y, cubePos.z + cubeScale[2][2]));
		//Invert velocity in the y axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(curVel.x, curVel.y, -curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
	//Back
	if (particlePos.z < cubePos.z - cubeScale[2][2]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(particlePos.x, particlePos.y, cubePos.z - cubeScale[2][2]));
		//Invert velocity in the y axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(curVel.x, curVel.y, -curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
}

//Calculate the force excerted on a particle at pos by a force cone.
glm::vec3 calcConeForce(glm::vec3 pos) {
	// Variables:
	// coneTip
	// coneAxis 
	// coneRad 
	// coneHeight

	// - Determine position within cone -

	//Calculate the distance along the 'height'/spine of the cone
	float projToAxis = dot(pos - coneTip, coneAxis);
	
	//If 'above' or 'below' cone, force is 0 (outsideb  the cone)
	if (projToAxis > coneHeight || projToAxis < 0 ) {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
	
	//Calculate the radius of the cone at the current projection
	float radAtProj = (projToAxis / coneHeight) * coneRad;
	
	//Calculate the current point's distance/radius from the spine of the cone
	float distFromSpine = length((pos - coneTip) - projToAxis * coneAxis);
	
	//If this distance is greater than the radius at the current point then it is outside the cone.
	if (distFromSpine > radAtProj) {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// - Calculate force magnitude based on position within cone -
	//Calculate falloff due to 'height' along spine
	float hFalloff = 1 - (projToAxis/coneHeight);
	//Calculate falloff due to radial distance
	float rFalloff = 1 - (distFromSpine/radAtProj);
	// Calculate total falloff
	float tFalloff = hFalloff * rFalloff;

	// - Calculate force direction -
	//Get vector from cone tip to particle position.
	glm::vec3 tipToPos = pos - coneTip;
	//Use normalized vector as direction away from tip
	glm::vec3 force = glm::normalize(tipToPos) * tFalloff;


	return force;
}
