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
std::vector<RigidBody> rigidbodies;


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
bool frictionEnabled = false;

//Force
Gravity *grav = new Gravity(glm::vec3(0.0f, -9.8f, 0.0f));




// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 10.0f));
			
	// Create Shaders
	Shader blue = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/5-Rigid_Body_Dynamics/project2/resources/shaders/physics.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/5-Rigid_Body_Dynamics/project2/resources/shaders/physics.frag");
	Shader red = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/5-Rigid_Body_Dynamics/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/5-Rigid_Body_Dynamics/project2/resources/shaders/solid_red.frag");
	Shader transLambert = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/5-Rigid_Body_Dynamics/project2/resources/shaders/physics.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/5-Rigid_Body_Dynamics/project2/resources/shaders/physics_transparent.frag");

	// - Create Cube -
	Mesh cube = Mesh::Mesh(Mesh::CUBE);
	// scale up x5
	cube.scale(glm::vec3(10.0f, 10.0f, 10.0f));
	//Set Shader
	cube.setShader(transLambert);

	//// time
	const GLfloat timestep = 0.013f; 
	GLfloat initTime = (GLfloat)glfwGetTime();
	GLfloat simStartTime; 
	GLfloat timeAccumulated = 0.0f;
	 
	//flags  
	static bool firstShot;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		//Flags to insure single firing of setups
		static bool flag = true;
		static bool flag1 = true;
		static bool flag2 = true;
		static bool flag3 = true;
		static bool flag4 = true;
		static bool flag5 = true;
		// - MODE SWITCHING -
		// 0 - 2.1 Application of an impulse (1 & 2)
		if (glfwGetKey(app.getWindow(), GLFW_KEY_0) && flag) {
			flag = false;					//## Running this without flag sometimes causes infinite loop + memory leak of unknown cause##.
			flag1 = true;
			flag2 = true;
			flag3 = true;
			flag4 = true;
			flag5 = true;
 			mode = 0;
			particles.clear();
			particles2D.clear();
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			firstShot = true;

			//Create rigidbody
			RigidBody rbCube = RigidBody();
			Mesh m	= Mesh::Mesh(Mesh::CUBE);
			rbCube.setMesh(m);
			rbCube.getMesh().setShader(blue);
			
			//Set static properties
			rbCube.scale(glm::vec3(1.0f, 3.0f, 1.0f));
			rbCube.setMass(2.0f);
			rbCube.setCoM(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setCor(1.0f);
			rbCube.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
			//rbCube.rotate(1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

			std::cout << "Inverse inertia matrix: " << glm::to_string(rbCube.getInvInertia()) << std::endl;

			//Set dynamic properties
			rbCube.setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setVel(glm::vec3(2.0f, 0.0f, 0.0f));

			//Add forces
			//rbCube.addForce(grav);

			//Add to collection
			rigidbodies.push_back(rbCube);

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 1 - 2.2 Collision Detection
		if (glfwGetKey(app.getWindow(), GLFW_KEY_1) && flag1) {
			flag1 = false;					//## Running this without flag sometimes causes infinite loop + memory leak of unknown cause##.
			flag = true;
			flag2 = true;
			flag3 = true;
			flag4 = true;
			flag5 = true;
			mode = 1;
			particles.clear();
			particles2D.clear();
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			frictionEnabled = false;

			//Create rigidbody
			RigidBody rbCube = RigidBody();
			Mesh m = Mesh::Mesh(Mesh::CUBE);
			rbCube.setMesh(m);
			rbCube.getMesh().setShader(blue);

			//Set static properties
			rbCube.scale(glm::vec3(1.0f, 3.0f, 1.0f));
			rbCube.setMass(2.0f);
			rbCube.setCoM(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setCor(1.0f);
			rbCube.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
			//rbCube.rotate(1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

			//Set dynamic properties
			rbCube.setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));
			//rbCube.setVel(glm::vec3(2.0f, 0.0f, 0.0f));

			//Add gravity force
			rbCube.addForce(grav);

			//Add to collection
			rigidbodies.push_back(rbCube);

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 2 - 2.3 Collision Response (1)
		if (glfwGetKey(app.getWindow(), GLFW_KEY_2) && flag2) {
			flag2 = false;
			flag = true;
			flag1 = true;							//## Running this without flag sometimes causes infinite loop + memory leak of unknown cause##.
			flag3 = true;
			flag4 = true;
			flag5 = true;
			mode = 1;
			particles.clear();
			particles2D.clear();
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			frictionEnabled = false;

			//Create rigidbody
			RigidBody rbCube = RigidBody();
			Mesh m = Mesh::Mesh(Mesh::CUBE);
			rbCube.setMesh(m);
			rbCube.getMesh().setShader(blue);

			//Set static properties
			rbCube.scale(glm::vec3(1.0f, 3.0f, 1.0f));
			rbCube.setMass(2.0f);
			rbCube.setCoM(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setCor(1.0f);
			rbCube.setPos(glm::vec3(0.0f, 0.0f, 0.0f));
			//rbCube.rotate(1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

			//Set dynamic properties
			rbCube.setAngVel(glm::vec3(0.0f, 0.0f, 0.5f));
			rbCube.setVel(glm::vec3(0.0f, 0.0f, 0.0f));

			//Add gravity force
			rbCube.addForce(grav);

			//Add to collection
			rigidbodies.push_back(rbCube);

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 3 - 2.3 Collision Response (2)
		if (glfwGetKey(app.getWindow(), GLFW_KEY_3) && flag3) {
			flag3 = false;
			flag = true;
			flag1 = true;							//## Running this without flag sometimes causes infinite loop + memory leak of unknown cause##.
			flag2 = true;
			flag4 = true;
			flag5 = true;
			mode = 1;
			particles.clear();
			particles2D.clear();
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			frictionEnabled = false;


			//Create rigidbody
			RigidBody rbCube = RigidBody();
			Mesh m = Mesh::Mesh(Mesh::CUBE);
			rbCube.setMesh(m);
			rbCube.getMesh().setShader(blue);

			//Set static properties
			rbCube.scale(glm::vec3(1.0f, 3.0f, 1.0f));
			rbCube.setMass(2.0f);
			rbCube.setCoM(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setCor(0.7f);
			rbCube.setPos(glm::vec3(0.0f, -3.0f, 0.0f));
			//rbCube.rotate(1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

			//Set dynamic properties
			rbCube.setAngVel(glm::vec3(0.1f, 0.1f, 0.1f));
			rbCube.setVel(glm::vec3(0.0f, 0.0f, 0.0f));

			//Add gravity force
			rbCube.addForce(grav);

			//Add to collection
			rigidbodies.push_back(rbCube);

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 4 - 2.4 Friction
		if (glfwGetKey(app.getWindow(), GLFW_KEY_4) && flag4) {
			flag4 = false;
			flag = true;
			flag1 = true;							//## Running this without flag sometimes causes infinite loop + memory leak of unknown cause##.
			flag2 = true;
			flag3 = true;
			flag5 = true;
			mode = 1;
			particles.clear();
			particles2D.clear();
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			frictionEnabled = true;

			//Create rigidbody
			RigidBody rbCube = RigidBody();
			Mesh m = Mesh::Mesh(Mesh::CUBE);
			rbCube.setMesh(m);
			rbCube.getMesh().setShader(blue);

			//Set static properties
			rbCube.scale(glm::vec3(1.0f, 3.0f, 1.0f));
			rbCube.setMass(2.0f);
			rbCube.setCoM(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setCor(0.6f);
			rbCube.setPos(glm::vec3(0.0f, -2.0f, 0.0f));
			//rbCube.rotate(2.0f, glm::vec3(0.0f, 0.0f, 1.0f));

			//Set dynamic properties
			rbCube.setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setVel(glm::vec3(1.0f, 0.0f, 0.0f));

			//Add gravity force
			rbCube.addForce(grav);

			//Add to collection
			rigidbodies.push_back(rbCube);

			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 5 - 2.4 Friction 2
		if (glfwGetKey(app.getWindow(), GLFW_KEY_5) && flag5) {
			flag5 = false;
			flag = true;
			flag1 = true;							//## Running this without flag sometimes causes infinite loop + memory leak of unknown cause##.
			flag2 = true;
			flag3 = true;
			flag4 = true;
			mode = 1;
			particles.clear();
			particles2D.clear();
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			frictionEnabled = true;

			//Create rigidbody
			RigidBody rbCube = RigidBody();
			Mesh m = Mesh::Mesh(Mesh::CUBE);
			rbCube.setMesh(m);
			rbCube.getMesh().setShader(blue);

			//Set static properties
			rbCube.scale(glm::vec3(1.0f, 3.0f, 1.0f));
			rbCube.setMass(2.0f);
			rbCube.setCoM(glm::vec3(0.0f, 0.0f, 0.0f));
			rbCube.setCor(0.6f);
			rbCube.setPos(glm::vec3(0.0f, 0.0f, 0.5f));
			//rbCube.rotate(2.0f, glm::vec3(0.0f, 0.0f, 1.0f));

			//Set dynamic properties
			rbCube.setAngVel(glm::vec3(0.0f, 0.0f, 0.5f));
			rbCube.setVel(glm::vec3(0.0f, 0.0f, 0.0f));

			//Add gravity force
			rbCube.addForce(grav);

			//Add to collection
			rigidbodies.push_back(rbCube);

			//Reset Time
			timeAccumulated = 0.0f;
		}

		// - OTHER USER INTERACTION -
		//Start Simulation
		if (glfwGetKey(app.getWindow(), GLFW_KEY_SPACE)) {
			pause = false;
			simStartTime = (GLfloat)glfwGetTime();
			firstShot = true;
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
				for (unsigned int i = 0; i < rigidbodies.size(); i++) {


					if (currentTime - simStartTime > 2 && firstShot) {
						firstShot = false;
						Impulse imp (glm::vec3(-1.0f, 0.0f, 0.0f),4.0f, glm::vec3(0.0f, -1.0f, 0.0f));
						rigidbodies[i].impulses.push_back(imp);
					}

					// - ROTATIONAL DYNAMICS - 
					//Calculate current angular velocity
					rigidbodies[i].setAngVel(rigidbodies[i].getAngVel() + timestep * rigidbodies[i].getAngAcc() + sumImpulsesAng(rigidbodies[i]));
					//Create skew symmetric matrix for w
					glm::mat3 angVelSkew = glm::matrixCross3(rigidbodies[i].getAngVel());
					//Create 3x3 rotation matrix from rigidbody rotation matrix
					glm::mat3 R = glm::mat3(rigidbodies[i].getRotate());
					//Update rotation Matrix
					R += timestep * angVelSkew*R;
					R = glm::orthonormalize(R);
					rigidbodies[i].setRotate(glm::mat4(R));

					// - LINEAR DYNAMICS -
					//Calculate Accelleration
					rigidbodies[i].setAcc(rigidbodies[i].applyForces(rigidbodies[i].getPos(), rigidbodies[i].getVel(), 1, timestep));
					//Calculate Current Velocity
					rigidbodies[i].setVel(rigidbodies[i].getVel() + timestep * rigidbodies[i].getAcc() + sumImpulsesLin(rigidbodies[i]));
					//Calculate New Position
					rigidbodies[i].translate(timestep * rigidbodies[i].getVel());
					//Check for collisions with bounding cube
					CheckCollisions(rigidbodies[i], cube);
				}
			}
			// 1 - Impulses
			if (mode == 1 && !pause) {
				for (unsigned int i = 0; i < rigidbodies.size(); i++) {
					
					// - ROTATIONAL DYNAMICS - 
					//Calculate current angular velocity
					rigidbodies[i].setAngVel(rigidbodies[i].getAngVel() + timestep * rigidbodies[i].getAngAcc() + sumImpulsesAng(rigidbodies[i]));
					//Create skew symmetric matrix for w
					glm::mat3 angVelSkew = glm::matrixCross3(rigidbodies[i].getAngVel());
					//Create 3x3 rotation matrix from rigidbody rotation matrix
					glm::mat3 R = glm::mat3(rigidbodies[i].getRotate());
					//Update rotation Matrix
					R += timestep * angVelSkew*R;
					R = glm::orthonormalize(R);
					rigidbodies[i].setRotate(glm::mat4(R));
					
					// - LINEAR DYNAMICS -
					//Calculate Accelleration
					rigidbodies[i].setAcc(rigidbodies[i].applyForces(rigidbodies[i].getPos(), rigidbodies[i].getVel(), 1, timestep));
					//Calculate Current Velocity
					rigidbodies[i].setVel(rigidbodies[i].getVel() + timestep * rigidbodies[i].getAcc() + sumImpulsesLin(rigidbodies[i]));
					//Calculate New Position
					rigidbodies[i].translate(timestep * rigidbodies[i].getVel());
					//Check for collisions with bounding cube
					CheckCollisions(rigidbodies[i], cube);
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
		//Draw rigidbodies
		for each (RigidBody rb in rigidbodies) {
			app.draw(rb.getMesh());
		}

		//- Render Environment Last for transparency -		
		app.draw(cube);
		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

//Summate impulses returning resulting angular velocity (Must be called BEFORE linear)
glm::vec3 sumImpulsesAng(RigidBody &rb) {
	//Summate Impulses - I^-1(r X j)
	glm::vec3 rotImpSum = glm::vec3(0.0f, 0.0f, 0.0f);
	for each (Impulse imp in rb.impulses)
	{
		rotImpSum += imp.getMag() * rb.getInvInertia() * glm::cross((imp.getPoA() - rb.getPos()), imp.getDir());


		//std::cout << "r: " << glm::to_string(imp.getPoA() - rb.getPos()) << std::endl;
		//std::cout << "r magnitude: " << std::to_string(glm::length(imp.getPoA() - rb.getPos())) << std::endl << std::endl;
	}
	return rotImpSum;
}

//Summate impulses returning resulting linear velocity (Must be called AFTER angular as pops)
glm::vec3 sumImpulsesLin(RigidBody &rb) {
	//Summate Impulses - j/m
	glm::vec3 linImpSum = glm::vec3(0.0f, 0.0f, 0.0f);
	while (!rb.impulses.empty())
	{
		linImpSum += (rb.impulses.back().getMag() / rb.getMass()) * rb.impulses.back().getDir();
		rb.impulses.pop_back();
	}
	return linImpSum;
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

//Check for rigidbody collisions with a bouding cube.
void CheckCollisions(RigidBody &rb, Mesh &cube)
{
	std::vector<Vertex> localVertices = rb.getMesh().getVertices();
	std::vector<glm::vec3> vertexCoordsGlobal;
	glm::vec3 cubePos = cube.getPos();
	glm::mat4 cubeScale = cube.getScale();
	glm::vec3 overShoot;
	glm::vec3 planeNormal;

	//Obtain global vertex coordinates
	for each (Vertex v in localVertices)
	{
		glm::vec3 gV = rb.getMesh().getModel() * glm::vec4(v.getCoord(), 1.0f);
		vertexCoordsGlobal.push_back(gV);
	}

	bool collision = false;
	std::vector<glm::vec3> collisionsCoords;
	for each (glm::vec3 vCoord in vertexCoordsGlobal)
	{
		//Check for collision with ground
		//Right
		if (vCoord.x > cubePos.x + cubeScale[0][0]) {
			collision = true;
			collisionsCoords.push_back(vCoord);
			planeNormal = glm::vec3(-1.0f, 0.0f, 0.0f);

			//Print for task 2.2
			std::cout << "Colliding vertex: " << glm::to_string(vCoord) << std::endl;

			//Calculate collision overshoot
			overShoot = glm::vec3(cubeScale[0][0], vCoord.y, vCoord.z) - vCoord;
		}
		//Left
		if (vCoord.x < cubePos.x - cubeScale[0][0]) {
			collision = true;
			collisionsCoords.push_back(vCoord);
			planeNormal = glm::vec3(1.0f, 0.0f, 0.0f);

			//Print for task 2.2
			std::cout << "Colliding vertex: " << glm::to_string(vCoord) << std::endl;

			//Calculate collision overshoot
			overShoot = glm::vec3(-cubeScale[0][0], vCoord.y, vCoord.z) - vCoord;
		}
		//Up
		if (vCoord.y > cubePos.y + cubeScale[1][1]) {
			collision = true;
			collisionsCoords.push_back(vCoord);
			planeNormal = glm::vec3(0.0f, -1.0f, 0.0f);

			//Print for task 2.2
			std::cout << "Colliding vertex: " << glm::to_string(vCoord) << std::endl;

			//Calculate collision overshoot
			overShoot = glm::vec3(vCoord.x, cubeScale[1][1], vCoord.z) - vCoord;
		}
		//Down
		if (vCoord.y < cubePos.y - cubeScale[1][1]) {
			collision = true;
			collisionsCoords.push_back(vCoord);
			planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);

			//Print for task 2.2
			std::cout << "Colliding vertex: " << glm::to_string(vCoord) << std::endl;

			//Calculate collision overshoot
			overShoot = glm::vec3(vCoord.x, -cubeScale[1][1], vCoord.z) - vCoord;

			//Teared ground resting
			if (frictionEnabled) {
				//Slow vibration if stopped on the ground
				if (glm::length(rb.getVel()) < 1.0f) {
					//rb.setVel(rb.getVel() * 0.5f);
					rb.setAngVel(rb.getAngVel() * 0.5f);
				}
				//Stop
				if (glm::length(rb.getVel()) < 0.1f && glm::length(rb.getAngVel()) < 2.0f) {
					rb.setAngVel(rb.getAngVel() * 0.2f);
					rb.setVel(rb.getVel() * 0.2f);
				}
				//Rest
				if (glm::length(rb.getVel()) < 0.05f && glm::length(rb.getAngVel()) < 2.0f) {
					rb.setAngVel(rb.getAngVel() * 0.05f);
					rb.setVel(rb.getVel() * 0.05f);
				}
			}


		}
		//Front
		if (vCoord.z > cubePos.z + cubeScale[2][2]) {
			collision = true;
			collisionsCoords.push_back(vCoord);
			planeNormal = glm::vec3(0.0f, 0.0f, -1.0f);

			//Print for task 2.2
			std::cout << "Colliding vertex: " << glm::to_string(vCoord) << std::endl;

			//Calculate collision overshoot
			overShoot = glm::vec3(vCoord.x, vCoord.y, cubeScale[2][2]) - vCoord;
		}
		//Back
		if (vCoord.z < cubePos.z - cubeScale[2][2]) {
			collision = true;
			collisionsCoords.push_back(vCoord);
			planeNormal = glm::vec3(0.0f, 0.0f, 1.0f);

			//Print for task 2.2
			std::cout << "Colliding vertex: " << glm::to_string(vCoord) << std::endl;

			//Calculate collision overshoot
			overShoot = glm::vec3(vCoord.x, vCoord.y, -cubeScale[2][2]) - vCoord;
		}
	}
	if (collision) {
		//Get center point of all collided vertices
		glm::vec3 averageCoord = glm::vec3(0.0f, 0.0f, 0.0f);
		float j = 0.0f;
		for each (glm::vec3 colCoord in collisionsCoords) {
			averageCoord += colCoord;
			j++;
		}
		averageCoord /= j;

		//pause = true;
		std::cout << "Average of all colliding vertices: " << glm::to_string(averageCoord) << std::endl << std::endl;
		//std::cout << glm::to_string(rb.getRotate()) << std::endl;

		//Move rb back to collision plane
		rb.setPos(rb.getPos() + overShoot);

		//Apply plane based collision impulse
		Impulse imp;

		//Set point of application
		imp.setPoA(averageCoord + overShoot);

		//Calculate distance from CoM
		glm::vec3 r = imp.getPoA() - rb.getPos();

		//Calculate velocity of point
		glm::vec3 pointVel = glm::vec3(rb.getVel() + glm::cross(rb.getAngVel(),  r));

		//Calculate impulse magnitude
		float num = -(1.0f + rb.getCor()) * glm::dot(pointVel, planeNormal);
		float denom = 1.0f / rb.getMass() + glm::dot(planeNormal, (glm::cross(rb.getInvInertia() * glm::cross(r, planeNormal), r)));
		float impMag =	num/denom;

		//Set impulse magnitude and direction
		imp.setMag(impMag);
		imp.setDir(planeNormal);

		rb.impulses.push_back(imp);

		if (frictionEnabled) {
			//Calculate and apply friction
			Impulse Jf = calculateFriction(pointVel, planeNormal, rb, impMag * planeNormal, imp.getPoA() - rb.getPos());
			rb.impulses.push_back(Jf);
		}

		//std::cout << "Impulse magnitude: " << std::to_string(impMag) << std::endl;
		//std::cout << "Impulse Direction: " << glm::to_string(planeNormal) << std::endl;
		//std::cout << "PoA: " << glm::to_string(imp.getPoA()) << std::endl;

		return;
	}
}

Impulse calculateFriction(glm::vec3 vRel, glm::vec3 planeNormal, RigidBody &rb, glm::vec3 jn, glm::vec3 r) {
	//Calculate Velocity Tangential to the plane
	glm::vec3 Vn = glm::proj(vRel, planeNormal);
	glm::vec3 Vt = vRel - Vn;
	//normalize
	glm::vec3 t;
	if (Vt != glm::vec3(0.0f, 0.0f, 0.0f))
		t = glm::normalize(Vt);
	else {
		Impulse k(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
		return k;
	}

	//Calculate Point of application
	glm::vec3 PoA = r + rb.getPos();

	//Method 1 - Gaffer's Method
	//Calculate impluse magnitude
	float numerator = glm::dot(vRel, t);
	float denom = (1.0f / rb.getMass()) + glm::dot(rb.getInvInertia() *  glm::cross(glm::cross(r, t), r), t);
	float jMag = numerator / denom;
	//Create and return impulse
	Impulse J(-t, jMag, PoA);
	return J;
	
	////Method 2 
	////Calculate friction impulse
	//glm::vec3 jMin = rb.getCoF() * abs(glm::length(jn)) * (Vt / glm::length(Vt));
	//float jMag = glm::length(Vt) * rb.getMass() + glm::length(rb.getAngVel()) / glm::length(rb.getInvInertia() * glm::cross(r, glm::normalize(jMin)));
	////Create and return impulse with cap
	//Impulse J(-t, jMag <= glm::length(jMin) ? jMag : glm::length(jMin), PoA);
	//return J;

	////Method 3
	////Calculate friction impulse
	//glm::vec3 jMin = rb.getCoF() * abs(glm::length(jn)) * (Vt / glm::length(Vt));
	//float jMax = glm::length(rb.getAngVel()) / glm::length(rb.getInvInertia() * glm::cross(r, glm::normalize(jMin)));
	//float jMag = glm::length(Vt) * rb.getMass() + glm::length(rb.getAngVel()) / glm::length(rb.getInvInertia() * glm::cross(r, glm::normalize(jMin)));
	////Create and return impulse with cap
	//Impulse J(-t, jMag <= jMax ? jMag : jMax, PoA);
	//return J;
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