#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

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

// time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Physics Objects
std::vector<Particle> particles;

// Global Properties
glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);


// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));
			
	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.scale(glm::vec3(10.0f, 5.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);

	//Create Particles
	for (int i = 0; i < 5; i++) {
		// Create particle
		Particle p = Particle::Particle();
		// Set Shader
		p.getMesh().setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

		//Set Random initial velocity values
		float rndX = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
		float rndY = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
		p.setVel(glm::vec3(rndX, rndY, 0.0f));

		//Add particle to collection
		particles.push_back(p);
	}

	// time
	GLfloat firstFrame = (GLfloat) glfwGetTime();
	
	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat currentFrame = (GLfloat)  glfwGetTime() - firstFrame;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.
		currentFrame *= 1.5f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*
		**	INTERACTION
		*/
		// Manage interaction
		app.doMovement(deltaTime);


		/*
		**	SIMULATION
		*/

		for (unsigned int i = 0; i < particles.size(); i++) {
			//Calculate Forces
			glm::vec3 force = particles[i].getMass() * g;
			//Calculate Accelleration
			particles[i].setAcc( force / particles[i].getMass());
			//Calculate Current Velocity
			particles[i].setVel(particles[i].getVel() + deltaTime * particles[i].getAcc());
			//Calculate New Position
			particles[i].translate(deltaTime * particles[i].getVel());

			glm::vec3 particlePos = particles[i].getPos();
			glm::vec3 groundPos = plane.getPos();

			//Check for collision with ground
			if (particlePos.y < groundPos.y) {
				//Move particle back up to ground pos
				particles[i].setPos(glm::vec3(particlePos.x, groundPos.y, particlePos.z));
				//Invert velocity in the y axis and apply damping
				glm::vec3 curVel = particles[i].getVel();
				particles[i].setVel(glm::vec3(curVel.x * 0.9, -curVel.y* 0.9, curVel.z));
			}
		}

		/*
		**	RENDER 
		*/		
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);


		// draw particles
		for each (Particle p in particles)
		{
			app.draw(p.getMesh());
		}

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

