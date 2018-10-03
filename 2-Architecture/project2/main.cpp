#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>
#include <list>    
#include <vector>

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


// time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//Particles and Properties
std::vector<Mesh> particles;
std::vector<glm::vec3> forces;
std::vector<glm::vec3> accellerations;
std::vector<glm::vec3> velocities;

//Global Particle Properties
float mass = 10.0f;
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
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.setShader(lambert);

	for (int i = 0; i < 5; i++) {
		// create particle
		Mesh p = Mesh::Mesh(Mesh::QUAD);
		//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
		p.translate(glm::vec3(0.0f, 2.5f, 0.0f));
		p.scale(glm::vec3(.1f, .1f, .1f));
		p.rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
		p.setShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

		float rnd = ((float)rand() / (RAND_MAX)) + 1;

		particles.push_back(p);
		forces.push_back(glm::vec3());
		accellerations.push_back(glm::vec3());
		velocities.push_back(glm::vec3(rnd , rnd, 0.0f));
	}
	/*
	// create demo objects (a cube and a sphere)
	Mesh sphere = Mesh::Mesh("resources/models/sphere.obj");
	sphere.translate(glm::vec3(-1.0f, 1.0f, 0.0f));
	sphere.setShader(lambert);
	Mesh cube = Mesh::Mesh("resources/models/cube.obj");
	cube.translate(glm::vec3(1.0f, .5f, 0.0f));
	cube.setShader(lambert);
	*/

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

		for (int i = 0; i < particles.size(); i++) {
			//Calculate Forces
			forces[i] = mass * g;
			//Calculate Accelleration
			accellerations[i] = forces[i] / mass;
			//Calculate Current Velocity
			velocities[i] = velocities[i] + deltaTime * accellerations[i];
			//Calculate New Position
			particles[i].translate(deltaTime * velocities[i]);
	
			glm::vec3 particlePos = particles[i].getPos();
			glm::vec3 groundPos = plane.getPos();

			//Check for collision with ground
			if (particlePos.y < groundPos.y) {
				//Move particle back up to ground pos
				particles[i].setPos(glm::vec3(particlePos.x, groundPos.y, particlePos.z));
				//Invert velocity in the y axis and apply damping
				velocities[i] = glm::vec3(velocities[i].x * 0.9, -velocities[i].y* 0.9, velocities[i].z);
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
		for each (Mesh p in particles)
		{
			app.draw(p);
		}

		// draw demo objects
		//app.draw(cube);
		//app.draw(sphere);

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

