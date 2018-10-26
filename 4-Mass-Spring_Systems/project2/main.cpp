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

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 1 - Spring Test
		if (glfwGetKey(app.getWindow(), GLFW_KEY_1)) {
			mode = 1;
			particles.clear();
			particles2D.clear();
			triangles.clear();
			pause = true;
			hooke.setRest(1.0f);
			hooke.setStiffness(1.0f);
			hooke.setDamping(0.5f);

			// Create particles
			Particle p1 = Particle::Particle();
			Particle p2 = Particle::Particle();
			// Set Shader
			p1.getMesh().setShader(red);
			p2.getMesh().setShader(blue);

			//Set initial position
			p1.setPos(glm::vec3(-2.0f, 10.0f, 0.0f));
			p2.setPos(glm::vec3(2.0f, 10.0f, 0.0f));
			//Add particle to collection
			particles.push_back(p1);
			particles.push_back(p2);
		}
		// 2 - Spring Chain of 5 Particles
		if (glfwGetKey(app.getWindow(), GLFW_KEY_2)) {
			//Set properties and reset
			mode = 2;
			particles.clear();
			particles2D.clear();
			triangles.clear();
			pause = true;
			hooke.setRest(1.0f);
			hooke.setStiffness(1.0f);
			hooke.setDamping(.3f);

			// Create particles
			for (unsigned int i = 0; i < 2; i++) {
				// Create new particle
				Particle p = Particle::Particle();
				//First particle
				if (i == 0) {
					// Set blue Shader
					p.getMesh().setShader(blue);
				}
				//Other Particles
				else {
					//Add Forces
					p.addForce(&gravity);
					p.addForce(&hooke);
					// Set red Shader
					p.getMesh().setShader(red);
				}
				//Set initial position
				p.setPos(glm::vec3(0.0f, 10.0f - i*1.5f, 0.0f));
				//Add particle to collection
				particles.push_back(p);
			}
		}
		// 3 - Chain of 10 particles fixed at either end
		if (glfwGetKey(app.getWindow(), GLFW_KEY_3)) {
			//Set properties and reset
			mode = 3;
			particles.clear();
			particles2D.clear();
			triangles.clear();
			pause = true;
			hooke.setRest(1.0f);
			hooke.setStiffness(2.0f);
			hooke.setDamping(0.2f);

			// Create particles
			for (unsigned int i = 0; i < 10; i++) {
				// Create new particle
				Particle p = Particle::Particle();
				//First and last particles
				if (i == 0 || i == 9) {
					// Set blue Shader
					p.getMesh().setShader(blue);
				}
				//Other Particles
				else {
					// Set red Shader
					p.getMesh().setShader(red);
				}
				//Set initial position
				p.setPos(glm::vec3(-4.0f + i, 10.0f, 0.0f));
				//Add particle to collection
				particles.push_back(p);
			}
		}
		// 4 - Chain of 10 particles fixed at either end with plane collision (Uses same simulator as 3)
		if (glfwGetKey(app.getWindow(), GLFW_KEY_4)) {
			//Set properties and reset
			mode = 3;
			particles.clear();
			particles2D.clear();
			triangles.clear();
			pause = true;
			hooke.setRest(1.0f);
			hooke.setStiffness(2.0f);
			hooke.setDamping(0.2f);

			// Create particles
			for (unsigned int i = 0; i < 10; i++) {
				// Create new particle
				Particle p = Particle::Particle();
				//First and last particles
				if (i == 0 || i == 9) {
					// Set blue Shader
					p.getMesh().setShader(blue);
				}
				//Other Particles
				else {
					// Set red Shader
					p.getMesh().setShader(red);
				}
				//Set initial position
				p.setPos(glm::vec3(-4.0f + i, -4.0f, 0.0f));
				//Add particle to collection
				particles.push_back(p);
			}
		}
		// 5 - Cloth Simulation 1 - Square with fixed corners
		if (glfwGetKey(app.getWindow(), GLFW_KEY_5)) {
			//Set properties and reset
			mode = 5;
			particles.clear();
			particles2D.clear();
			triangles.clear();
			pause = true;
			hooke.setRest(0.5f);
			hooke.setStiffness(0.5f);
			hooke.setDamping(0.5f);

			//Create 2d Particle grid
			for (unsigned int i = 0; i < 10; i++) {
				std::vector<Particle> ps;
				for (unsigned int j = 0; j < 10; j++) {
					// Create new particle
					Particle p = Particle::Particle();
					//Set corners to blue
					if (i == 0 && j == 0 || i == 9 && j == 0 || i == 0 && j == 9 || i == 9 && j == 9) {
						p.getMesh().setShader(blue);
					}
					//Set rest to red
					else {
						p.getMesh().setShader(red);
					}
					//Set position
					p.setPos(glm::vec3(5.0f-i, 0.0f, 5.0f-j));
					p.setMass(0.01f);
					//Add to collection
					ps.push_back(p);
				}
				particles2D.push_back(ps);
			}
		}
		// 6 - Cloth Simulation 1 - Square with fixed corners (VELOCITY VERLET)
		if (glfwGetKey(app.getWindow(), GLFW_KEY_6)) {
			//Set properties and reset
			mode = 6;
			particles.clear();
			particles2D.clear();
			triangles.clear();
			pause = true;
			float rest(1.0f);
			float stiffness(1.0f);
			float damping(0.5f);

			//Create 2d Particle grid
			for (unsigned int i = 0; i < 10; i++) {
				std::vector<Particle> ps;
				for (unsigned int j = 0; j < 10; j++) {
					// Create new particle
					Particle p = Particle::Particle();
					//Set corners to blue
					if (i == 0 && j == 0 || i == 9 && j == 0 || i == 0 && j == 9 || i == 9 && j == 9) {
						p.getMesh().setShader(blue);
					}
					//Set rest to red
					else {
						//Colour current particle red
						p.getMesh().setShader(red);
					}
					//Set position
					p.setPos(glm::vec3(5.0f - i, 0.0f, 5.0f - j));
					p.setMass(0.01f);
					//Add to collection
					ps.push_back(p);
				}
				particles2D.push_back(ps);
			}

			//Make cloth connections
			CreateCloth(particles2D, stiffness, damping, rest);

		}
		// 7 - Flag Simulation - Square fixed at two corners with wind.
		if (glfwGetKey(app.getWindow(), GLFW_KEY_7)) {
			//Set properties and reset
			mode = 7;
			particles.clear();
			particles2D.clear();
			triangles.clear();
			pause = true;
			float rest(1.0f);
			float stiffness(1.0f);
			float damping(0.5f); 

			//Create 2d Particle grid
			for (unsigned int i = 0; i < 10; i++) {
				std::vector<Particle> ps;
				for (unsigned int j = 0; j < 10; j++) {
					// Create new particle
					Particle p = Particle::Particle();
					//Set two corners to blue
					if (i == 0 && j == 0 ||  i == 0 && j == 9) {
						p.getMesh().setShader(blue);
					}
					//Set rest to red
					else {
						//Colour current particle red
						p.getMesh().setShader(red);
					}
					//Set position
					p.setPos(glm::vec3(6.5f, 10.0f-i, 5.0f - j));
					p.setMass(0.01f);
					p.setCor(0.3f);
					//Add to collection
					ps.push_back(p);
				}
				particles2D.push_back(ps);
			}
			//Triangulate
			triangles = TriangulateGrid(particles2D);

			//Make cloth connections
			CreateCloth(particles2D, stiffness, damping, rest);
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

			// 1 - Particle Pair Spring Test -
			if (mode == 1 && !pause) {
				//Calculate Forces
				glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
				//Get references to bodies of each particle and set for calculation
				Body b1 = (Body)particles[0];
				Body b2 = (Body)particles[1];
				hooke.setB1(&b1);
				hooke.setB2(&b2);
				//Calculate spring force
				force += hooke.apply(particles[0].getMass(), particles[0].getPos(), particles[0].getVel());
				//Calculate Gravity
				force += gravity.apply(particles[0].getMass(), particles[0].getPos(), particles[0].getVel());
				//Calculate Accelleration
				particles[0].setAcc(force / particles[0].getMass());
				//Calculate Current Velocity
				particles[0].setVel(particles[0].getVel() + timestep * particles[0].getAcc());
				//Calculate New Position
				particles[0].translate(timestep * particles[0].getVel());
				//Check for collisions with the bounding box
				CheckCollisions(particles[0], cube);
			}

			// 2 - Chain of 5 Particles connected by Springs -
			if (mode == 2 && !pause) {
				//For each particle in collection
				for (unsigned int i = 0; i < particles.size(); i++) {
					//Simulate all but the first particle
					if (i != 0) {
						//Calculate Forces
						glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
						//Get references to bodies of the current and previous particles and set for calculation
						Body b1 = (Body)particles[i];
						Body b2 = (Body)particles[i - 1];
						hooke.setB1(&b1);
						hooke.setB2(&b2);

						//Calculate spring force
						force += hooke.apply(particles[i].getMass(), particles[i].getPos(), particles[i].getVel());
						//Calculate Gravity
						force += gravity.apply(particles[i].getMass(), particles[i].getPos(), particles[i].getVel());

						// - APPLY FORCES THEN INTEGRATE -

						//Calculate Accelleration
						particles[i].setAcc(force / particles[i].getMass());
						particles[i - 1].setAcc(-force/ particles[i - 1].getMass());
						//Calculate Current Velocity
						particles[i].setVel(particles[i].getVel() + timestep * particles[i].getAcc());
						particles[i - 1].setVel(particles[i - 1].getVel() + timestep * particles[i - 1].getAcc());
						//Calculate New Position
						particles[i].translate(timestep * particles[i].getVel());
						//Check for collisions with the bounding box
						CheckCollisions(particles[i], cube);
					}
				}
			}

			// 3 - Chain of 10 particles fixed at either end -
			if (mode == 3 && !pause) {
				for (unsigned int i = 0; i < particles.size(); i++) {
					//For all but the extremities
					if (i > 0 && i < particles.size()-1) {
						//Calculate Forces
						glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
						//Calculate Gravity
						force += gravity.apply(particles[i].getMass(), particles[i].getPos(), particles[i].getVel());

						// - HOOKE WITH PREVIOUS -
						//Get references to bodies of the current and previous particles and set for calculation
						Body b1 = (Body)particles[i];
						Body b2 = (Body)particles[i - 1];
						hooke.setB1(&b1);
						hooke.setB2(&b2);

						//Calculate spring force
						force += hooke.apply(particles[i].getMass(), particles[i].getPos(), particles[i].getVel());

						// - HOOKE WITH NEXT -
						//Get references to bodies of the current and previous particles and set for calculation
						Body b3 = (Body)particles[i + 1];
						hooke.setB2(&b3);

						//Calculate spring force
						force += hooke.apply(particles[i].getMass(), particles[i].getPos(), particles[i].getVel());

						//Calculate Accelleration
						particles[i].setAcc(force / particles[i].getMass());

						//Calculate Current Velocity
						particles[i].setVel(particles[i].getVel() + timestep * particles[i].getAcc());

						//Calculate New Position
						particles[i].translate(timestep * particles[i].getVel());

						//Check for collisions with the bounding box
						CheckCollisions(particles[i], cube);
					}
				}
			}

			// 5 - Cloth of 100 particles fixed at each corner -
			if (mode == 5 && !pause) {
				for (unsigned int i = 0; i < particles2D.size(); i++) {
					for (unsigned int j = 0; j < particles2D[i].size(); j++) {
						//For all but the corners
						if (!(i == 0 && j == 0 ) && !(i == particles2D.size() -1 && j == 0 ) && !( i == 0 && j == particles2D.size() -1) && !( i == particles2D.size() -1 && j == particles2D.size() - 1 ) ) {
							//Calculate Forces
							glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
							//Calculate Gravity
							force += gravity.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());
							
							//Set references to current particle's body
							Body b1 = (Body)particles2D[i][j];
							hooke.setB1(&b1);

							// - SIDE CONNECTIONS -

							// - ALL BUT LEFT EDGE -
							if (i != 0) {
								//Set references to other body
								Body b2 = (Body)particles2D[i - 1][j];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());

							}

							// - ALL BUT RIGHT EDGE -
							if (i != particles2D.size() - 1) {
								//Set references to other body
								Body b2 = (Body)particles2D[i + 1][j];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());
							}

							// - ALL BUT FRONT EDGE - 
							if (j != 0) {
								//Set references to other body
								Body b2 = (Body)particles2D[i][j - 1];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());
							}

							// - ALL BUT BACK EDGE -
							if (j != particles2D[i].size() - 1) {
								//Set references to other body
								Body b2 = (Body)particles2D[i][j + 1];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());
							}

							// - DIAGONAL CONNECTIONS - (CAUSING EXPLOSION)

							// - ALL BUT BACK OR RIGHT EDGES- 
							if (j != particles2D[i].size() - 1 && i != particles2D.size() - 1) {
								//Set references to other body
								Body b2 = (Body)particles2D[i + 1][j + 1];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());

							}

							// - ALL BUT FRONT OR RIGHT EDGES- 
							if (j != 0 && i != particles2D.size() - 1) {
								//Set references to other body
								Body b2 = (Body)particles2D[i + 1][j - 1];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());

							}

							// - ALL BUT FRONT OR LEFT EDGES- 
							if (j != 0 && i != 0) {
								//Set references to other body
								Body b2 = (Body)particles2D[i - 1][j - 1];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());

							}

							// - ALL BUT BACK OR LEFT EDGES- 
							if (j != particles2D[i].size() - 1 && i != 0) {
								//Set references to other body
								Body b2 = (Body)particles2D[i - 1][j + 1];
								hooke.setB2(&b2);

								//Calculate spring force
								force += hooke.apply(particles2D[i][j].getMass(), particles2D[i][j].getPos(), particles2D[i][j].getVel());
							}



							//Apply Drag force
							force += particles2D[i][j].getVel()* -0.01f;

							//Calculate Accelleration
							particles2D[i][j].setAcc(force / particles2D[i][j].getMass());

							//Calculate Current Velocity
							particles2D[i][j].setVel(particles2D[i][j].getVel() + timestep * particles2D[i][j].getAcc());

							//Calculate New Position
							particles2D[i][j].translate(timestep * particles2D[i][j].getVel());

							//Check for collisions with the bounding box
							CheckCollisions(particles2D[i][j], cube);

						}
					}
				}
			}

			// 6 - Cloth of 100 particles fixed at each corner - (Velocity Verlet)
			if (mode == 6 && !pause) {
				for (unsigned int i = 0; i < particles2D.size(); i++) {
					for (unsigned int j = 0; j < particles2D[i].size(); j++) {
						//For all but the corners
						if (!(i == 0 && j == 0) && !(i == particles2D.size() - 1 && j == 0) && !(i == 0 && j == particles2D.size() - 1) && !(i == particles2D.size() - 1 && j == particles2D.size() - 1)) {
							//Apply Drag force
							//force += particles2D[i][j].getVel()* -0.01f;

							//// - Velocity Verlet Integration -
							////Set Previous accelleration
							//particles2D[i][j].setPrevAcc(particles2D[i][j].getAcc());

							////Calculate New Position
							//particles2D[i][j].translate(particles2D[i][j].getVel() * timestep + (0.5 * particles2D[i][j].getPrevAcc() * pow(timestep, 2)));

							////Calculate New Accelleration
							//particles2D[i][j].setAcc(particles2D[i][j].applyForces(particles2D[i][j].getPos(), particles2D[i][j].getVel(), 1, timestep));

							////Calculate Average Accelleration between old and new
							//glm::vec3 Aavg = (particles2D[i][j].getPrevAcc() + particles2D[i][j].getAcc()) / 2;

							////Calculate Current Velocity
							//particles2D[i][j].setVel(particles2D[i][j].getVel() + timestep * Aavg);

							////Check for collisions with the bounding box
							//CheckCollisions(particles2D[i][j], cube);

							//Calculate Accelleration
							particles2D[i][j].setAcc(particles2D[i][j].applyForces(particles2D[i][j].getPos(), particles2D[i][j].getVel(), 1, timestep));

							//Calculate Current Velocity
							particles2D[i][j].setVel(particles2D[i][j].getVel() + timestep * particles2D[i][j].getAcc());

							//Apply Drag force
							particles2D[i][j].setVel(particles2D[i][j].getVel()*0.997);

							//Calculate New Position
							particles2D[i][j].translate(timestep * particles2D[i][j].getVel());

							//Check for collisions with the bounding box
							CheckCollisions(particles2D[i][j], cube);

						}
					}
				}
			}

			// 7 - Cloth of 100 particles fixed at each corner - (Velocity Verlet)
			if (mode == 7 && !pause) {
				//Particle Forces
				for (unsigned int i = 0; i < particles2D.size(); i++) {
					for (unsigned int j = 0; j < particles2D[i].size(); j++) {
						//For all but the corners
						if (!(i == 0 && j == 0) && !(i == 0 && j == particles2D.size() - 1)) {

							//Calculate Accelleration
							particles2D[i][j].setAcc(particles2D[i][j].applyForces(particles2D[i][j].getPos(), particles2D[i][j].getVel(), 1, timestep));

							//Calculate Current Velocity
							particles2D[i][j].setVel(particles2D[i][j].getVel() + timestep * particles2D[i][j].getAcc());

							//Apply Drag force
							particles2D[i][j].setVel(particles2D[i][j].getVel()*0.997);

							//Calculate New Position
							particles2D[i][j].translate(timestep * particles2D[i][j].getVel());

							//Check for collisions with the bounding box
							CheckCollisions(particles2D[i][j], cube);
						}
					}

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

		////Draw Triangles
		//for each (Triangle *tri in triangles) {
 		//	Mesh triMesh = Mesh::Mesh();
		//	triMesh.initMesh(tri->getVertices(), tri->getNormals());
		//	triMesh.setShader(blue);
		//	app.draw(triMesh);
		//}

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