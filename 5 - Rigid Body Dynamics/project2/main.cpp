#pragma once
// Math constants
#define _USE_MATH_DEFINES

#include "main.h"

// time
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Physics Objects
std::vector<Particle> particles;
std::vector < std::vector<Particle> > particles2D;
std::vector<Triangle*> triangles;


// Global Properties
glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);

//cone properties
glm::vec3 coneTip = glm::vec3(0.0f, -21.0f, 0.0f);
glm::vec3 coneAxis = glm::vec3(0.0f, 1.0f, 0.0f);
float coneRad = 5.0f;
float coneHeight = 20.0f;
float coneForceScale = 5.0f;

//other
unsigned int mode = 10;
bool firstLoop;
bool pause;


//Force
Hooke hooke;
Gravity gravity = Gravity::Gravity();
Gravity *grav = new Gravity(glm::vec3(0.0f, -9.8f, 0.0f));




// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 10.0f));
			
	// Create Shaders
	Shader blue = Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag");
	Shader red = Shader("resources/shaders/solid.vert", "resources/shaders/solid_red.frag");
	Shader transLambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics_transparent.frag");

	// - Create Cube -
	Mesh cube = Mesh::Mesh(Mesh::CUBE);
	// scale up x5
	cube.scale(glm::vec3(10.0f, 10.0f, 10.0f));
	//Set Shader
	cube.setShader(transLambert);

	//// time
	const GLfloat timestep = 0.005f;
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
			particles2D.clear();
			triangles.clear();
			pause = true;

			//Create cube rigidbody
			RigidBody rbCube = RigidBody();
			//rbCube.setMesh();

			//Reset Time
			timeAccumulated = 0.0f;
		}

		// - OTHER USER INTERACTION -
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


			// - Remove calculated time from the accumulator -
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
		for each (std::vector<Particle> vp in particles2D) {
			for each (Particle p in vp) {
				app.draw(p.getMesh());
			}
		}

		//- Render Environment Last for transparency -		
		app.draw(cube);
		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

void CreateCloth(std::vector<std::vector<Particle> > &p2D, float stiffness, float damping, float rest)
{
	//Add forces
	for (unsigned int i = 0; i < p2D.size(); i++) {
		for (unsigned int j = 0; j < p2D[i].size(); j++) {
			
			//Gravity Force
			p2D[i][j].addForce(grav);

			//Hooke Forces
			// - SIDE CONNECTIONS -

			// - ALL BUT LEFT EDGE -
			if (i != 0) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i - 1][j], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}

			// - ALL BUT RIGHT EDGE -
			if (i != p2D.size() - 1) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i + 1][j], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}

			// - ALL BUT FRONT EDGE - 
			if (j != 0) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i][j - 1], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}

			// - ALL BUT BACK EDGE -
			if (j != p2D[i].size() - 1) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i][j + 1], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}

			// - DIAGONAL CONNECTIONS

			// - ALL BUT BACK OR RIGHT EDGES- 
			if (j != p2D[i].size() - 1 && i != p2D.size() - 1) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i + 1][j + 1], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}

			// - ALL BUT FRONT OR RIGHT EDGES- 
			if (j != 0 && i != p2D.size() - 1) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i + 1][j - 1], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}

			// - ALL BUT FRONT OR LEFT EDGES- 
			if (j != 0 && i != 0) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i - 1][j - 1], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}

			// - ALL BUT BACK OR LEFT EDGES- 
			if (j != p2D[i].size() - 1 && i != 0) {
				//Add Hooke force to Body
				Hooke *h = new Hooke(&p2D[i][j], &p2D[i - 1][j + 1], stiffness, damping, rest);
				p2D[i][j].addForce(h);
			}
		}
	}
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
		//Invert velocity in the x axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(-curVel.x, curVel.y, curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
	//Left
	if (particlePos.x < cubePos.x - cubeScale[0][0]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(cubePos.x - cubeScale[0][0], particlePos.y, particlePos.z));
		//Invert velocity in the x axis and apply damping
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
		//Invert velocity in the z axis and apply damping
		glm::vec3 curVel = p.getVel();
		p.setVel(glm::vec3(curVel.x, curVel.y, -curVel.z));
		//Apply COR
		p.setVel(p.getVel()*p.getCor());
	}
	//Back
	if (particlePos.z < cubePos.z - cubeScale[2][2]) {
		//Move particle back in to plane pos
		p.setPos(glm::vec3(particlePos.x, particlePos.y, cubePos.z - cubeScale[2][2]));
		//Invert velocity in the z axis and apply damping
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

std::vector<Triangle*> TriangulateGrid(std::vector<std::vector<Particle> > &p2D) {
	std::vector<Triangle*> tris;
	//For every particle in grid
	for (unsigned int i = 0; i < p2D.size(); i++) {
		for (unsigned int j = 0; j < p2D[i].size(); j++) {
			//Down Triangle
			if (i != p2D.size() - 1 && j != p2D.size() - 1) {
				Triangle *tri = new Triangle(&p2D[i][j], &p2D[i+1][j], &p2D[i][j+1]);
				//Add Wind force to Body
				Wind *w = new Wind(tri, 1.0f, 1.0f);
				p2D[i][j].addForce(w);
				p2D[i + 1][j].addForce(w);
				p2D[i][j + 1].addForce(w);
				tris.push_back(tri);
			}
			//Up Triangle
			if (i != p2D.size() - 1 && j != 0) {
				Triangle *tri = new Triangle(&p2D[i][j], &p2D[i + 1][j - 1], &p2D[i + 1][j]);
				//Add Wind force to Body
				Wind *w = new Wind(tri, 1.0f, 1.0f);
				p2D[i][j].addForce(w);
				p2D[i][j].addForce(w);
				p2D[i][j].addForce(w);
				tris.push_back(tri);
			}
		}
	}
	return tris;
}