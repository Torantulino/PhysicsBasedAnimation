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

unsigned int mode = 10;


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
	Shader transLambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics_transparent.frag");
	cube.setShader(transLambert);

	//Create cone
	Mesh cone = Mesh::Mesh("resources/models/cone.obj");
	//Set Shader
	cone.setShader(transLambert);
	cone.scale(glm::vec3(3.0f));


	// time
	const GLfloat dt = 0.01f;

	GLfloat initTime = (GLfloat)glfwGetTime();
	GLfloat timeAccumulated = 0.0f;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat currentTime = (GLfloat) glfwGetTime() - initTime;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor. TODO: Add user control of this variable.
		//currentFrame *= 1.0f;
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		timeAccumulated += deltaTime;

		//Work off accumulated time
		while (timeAccumulated >= deltaTime) {
			/*
			**	INTERACTION
			*/
			// Manage interaction
			app.doMovement(deltaTime);


			/*
			**	SIMULATION
			*/

			// 0 - Test Simulation
			if (mode == 0) {
				for (unsigned int i = 0; i < particles.size(); i++) {
					//Calculate Forces
					glm::vec3 force = particles[i].getMass() * g;
					//Calculate Accelleration
					particles[i].setAcc(force / particles[i].getMass());
					//Calculate Current Velocity
					particles[i].setVel(particles[i].getVel() + deltaTime * particles[i].getAcc());
					//Calculate New Position
					particles[i].translate(deltaTime * particles[i].getVel());

					CheckCollisions(particles[i], cube);
				}
			}
			//Integration Methods Demo
			if (mode == 3) {
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
						particles[i].setVel(prevV + deltaTime * particles[i].getAcc());
						//Calculate New Position
						particles[i].translate(deltaTime * prevV);
					}
					//Semi-Implicit Euler Integration
					if (i == 2) {
						//Calculate Forces
						glm::vec3 force = particles[i].getMass() * g;
						//Calculate Accelleration
						particles[i].setAcc(force / particles[i].getMass());
						//Calculate Current Velocity
						particles[i].setVel(particles[i].getVel() + deltaTime * particles[i].getAcc());
						//Calculate New Position
						particles[i].translate(deltaTime * particles[i].getVel());
					}


					// - Collisions -
					CheckCollisions(particles[i], cube);
				}
			}
			if (mode == 4) {
				for (unsigned int i = 0; i < particles.size(); i++) {
				
				}
			}


			timeAccumulated -= deltaTime;
		}

		// - MODE SWITCHING -
		// 0 - Test Sim
		if (glfwGetKey(app.getWindow(), GLFW_KEY_0)) {
			mode = 0;
			particles.clear();


			//Create Particles
			for (int i = 0; i < 50; i++) {
				// Create particle 
				Particle p = Particle::Particle();
				// Set Shader
				p.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

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

		}
		// 1 - Bouncing Particle in Cube
		// 3 - Integration Comparison
		if (glfwGetKey(app.getWindow(), GLFW_KEY_3)) {
			//Set mode and clear vectors
			mode = 3;
			particles.clear();

			// Create particle
			Particle pRef = Particle::Particle();
			Particle pFU = Particle::Particle();
			Particle pSIE = Particle::Particle();

			// Set Shader
			pRef.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
			pFU.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
			pSIE.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

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
		}
		// 4 - Blow Dryer
		if (glfwGetKey(app.getWindow(), GLFW_KEY_4)) {
			//Set mode and clear vectors
			mode = 4;
			particles.clear();


			//Create Particles
			for (int i = 0; i < 20; i++) {
				// Create particle 
				Particle p = Particle::Particle();
				// Set Shader
				p.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
				
				//Set initial position
				p.setPos(glm::vec3(0.0f, 10.0f, 0.0f));

				//Set Random initial velocity values
				float rndX = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				float rndY = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				float rndZ = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
				p.setVel(glm::vec3(rndX * 5, rndY * 4, rndZ * 5));

				//Add particle to collection
				particles.push_back(p);
			}
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


		//Render Environment Last for transparency
		// draw groud plane
		if (mode == 4) {
			app.draw(cone);
		}
		app.draw(cube);
		


		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

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

