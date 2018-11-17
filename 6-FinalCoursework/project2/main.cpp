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
std::vector<Sphere> rigidbodies;


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
	Shader lambert = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/6-FinalCoursework/project2/resources/shaders/physics.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/6-FinalCoursework/project2/resources/shaders/physics.frag");
	Shader red = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/6-FinalCoursework/project2/resources/shaders/solid.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/6-FinalCoursework/project2/resources/shaders/solid_red.frag");
	Shader transLambert = Shader("E:/University/Year3_Tri1/Physics_Based_Animation/6-FinalCoursework/project2/resources/shaders/physics.vert", "E:/University/Year3_Tri1/Physics_Based_Animation/6-FinalCoursework/project2/resources/shaders/physics_transparent.frag");

	// - Create Cube -
	Mesh cube = Mesh::Mesh(Mesh::CUBE);
	// scale up x5
	cube.scale(glm::vec3(10.0f, 10.0f, 10.0f));
	//Set Shader
	cube.setShader(transLambert); 
	   
	//// time
	const GLfloat timestep = 0.003f; 
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
		// - MODE SWITCHING -
		// 1 - 2.2 Collision Detection
		if (glfwGetKey(app.getWindow(), GLFW_KEY_1) && flag1) {
			flag1 = false;
			flag = true;
			mode = 1;
			particles.clear();
			particles2D.clear(); 
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			frictionEnabled = true;

			for (unsigned int i = 0; i < 5; i++) {
				//Create sphere
				Sphere sphere = Sphere();
				Mesh m = Mesh::Mesh("./resources/models/sphere.obj");
				sphere.setMesh(m);
				sphere.getMesh().setShader(lambert);

				//Set static properties    
				sphere.scale(glm::vec3(1.0f, 1.0f, 1.0f));
				sphere.setMass(2.0f);
				sphere.setCoM(glm::vec3(0.0f, 0.0f, 0.0f));
				sphere.setCor(1.0f);
				sphere.setPos(glm::vec3(-9.0f + i*3, -9.0f, 0.0f));
				//rbCube.rotate(1.0f, glm::vec3(0.0f, 0.0f, 1.0f));

				//Set dynamic properties
				sphere.setAngVel(glm::vec3(30.0f * i, 0.0f, i*15.0f));
				sphere.setVel(glm::vec3(0.0f, 0.0f, 0.0f));

				//Add gravity force
				sphere.addForce(grav);

				//Add to collection
				rigidbodies.push_back(sphere);
			}
			//Reset Time
			timeAccumulated = 0.0f;
		}
		// 2 - 2.3 Collision Response (1)
		if (glfwGetKey(app.getWindow(), GLFW_KEY_2) && flag) {
			flag = false;
			flag1 = true;
			mode = 1;
			particles.clear();
			particles2D.clear();
			triangles.clear();
 			rigidbodies.clear();		
			pause = true;
			frictionEnabled = true;
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

					//Check for intersphere collisions
					CheckCollisions(rigidbodies);
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

//Create Hooke force connections for the given 2D particle array
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

//Check for Cube collisions with a bouding cube.
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

//Check for Shpere collisions with a bouding cube.
void CheckCollisions(Sphere &rb, Mesh &cube)
{
	glm::vec3 cubePos = cube.getPos();
	glm::mat4 cubeScale = cube.getScale();
	glm::vec3 overShoot;
	glm::vec3 planeNormal;
	glm::vec3 spherePos = rb.getPos();
	bool collision = false;
	float distance;

	//##DOESN'T WORK WHEN THERE ARE TWO COLLISIONS AT ONCE!##
	//Check for collision with ground
	//Right
	if (spherePos.x + rb.getRadius() > cubePos.x + cubeScale[0][0]) { //TODO: Convert rest to this method to prevent tunneling
		collision = true;
		planeNormal = glm::vec3(-1.0f, 0.0f, 0.0f);

		CollisionResponse(rb, overShoot, planeNormal);

		//Calculate collision overshoot
		overShoot = glm::vec3(cubePos.x + cubeScale[0][0], spherePos.y, spherePos.z) - glm::vec3(spherePos.x + rb.getRadius(), spherePos.y, spherePos.z);
	}

	//Left
	distance = abs(cubePos.x - cubeScale[0][0] - spherePos.x);
	if (distance < rb.getRadius()) {
		collision = true;
		planeNormal = glm::vec3(1.0f, 0.0f, 0.0f);

		CollisionResponse(rb, overShoot, planeNormal);

		//Calculate collision overshoot
		overShoot = glm::vec3(cubePos.x - cubeScale[0][0], spherePos.y, spherePos.z) - glm::vec3(spherePos.x - rb.getRadius(), spherePos.y, spherePos.z);
	}

	//Up
	distance = abs(cubePos.y + cubeScale[1][1] - spherePos.y);
	if (distance < rb.getRadius()) {
		collision = true;
		planeNormal = glm::vec3(0.0f, -1.0f, 0.0f);

		CollisionResponse(rb, overShoot, planeNormal);

		//Calculate collision overshoot
		overShoot = glm::vec3(spherePos.x, cubePos.y + cubeScale[1][1], spherePos.z) - glm::vec3(spherePos.x, spherePos.y + rb.getRadius(), spherePos.z);
	}

	//Down
	distance = abs(cubePos.y - cubeScale[1][1] - spherePos.y);
	if (distance < rb.getRadius()) {
		collision = true;
		planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);

		//Calculate collision overshoot
		overShoot = glm::vec3(spherePos.x, cubePos.y - cubeScale[1][1], spherePos.z) - glm::vec3(spherePos.x, spherePos.y - rb.getRadius(), spherePos.z);

		CollisionResponse(rb, overShoot, planeNormal);

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
	distance = abs(cubePos.z + cubeScale[2][2] - spherePos.z);
	if (distance < rb.getRadius()) {
		collision = true;
		planeNormal = glm::vec3(0.0f, 0.0f, -1.0f);

		//Calculate collision overshoot
		overShoot = glm::vec3(spherePos.x, spherePos.y, cubePos.z + cubeScale[2][2]) - glm::vec3(spherePos.x, spherePos.y, spherePos.z + rb.getRadius());
		return CollisionResponse(rb, overShoot, planeNormal);

	}

	//Back
	distance = abs(cubePos.z - cubeScale[2][2] - spherePos.z);
	if (distance < rb.getRadius()) {
		collision = true;
		planeNormal = glm::vec3(0.0f, 0.0f, 1.0f);

		//Calculate collision overshoot
		overShoot = glm::vec3(spherePos.x, spherePos.y, cubePos.z - cubeScale[2][2]) - glm::vec3(spherePos.x, spherePos.y, spherePos.z - rb.getRadius());
		return CollisionResponse(rb, overShoot, planeNormal);

	}
}

void CheckCollisions(std::vector<Sphere> &spheres) {
	//iterate through collection
	for (unsigned int i = 0; i < spheres.size(); i++) {
		for (unsigned int j = 0; j < spheres.size(); j++) {
			//If the spheres are not the same object
			if (i != j) {
				float combiedRadius = spheres[i].getRadius() + spheres[j].getRadius();
				float distance = glm::length(spheres[i].getPos() - spheres[j].getPos());
				if (distance < combiedRadius) {
					float overshoot = combiedRadius - distance;
					CollisionResponse(spheres[i], spheres[j], overshoot);
				}
			}
		}
	}
}

void CollisionResponse(Sphere & sp1, Sphere & sp2, float overshoot) {
	glm::vec3 collisionNormal = glm::normalize(sp2.getPos() - sp1.getPos());	//Towards sp2
	glm::vec3 r1 = collisionNormal * sp1.getRadius();
	glm::vec3 r2 = -collisionNormal * sp2.getRadius();
	glm::vec3 PoA = sp1.getPos() + r1;
	
	//Move back against collision
	sp1.setPos(sp1.getPos() - (overshoot / 2) * collisionNormal);
	sp2.setPos(sp2.getPos() + (overshoot / 2) * collisionNormal);

	//Calculate velocity of collision points
	glm::vec3 pointVel1 = glm::vec3(sp1.getVel() + glm::cross(sp1.getAngVel(), r1));
	glm::vec3 pointVel2 = glm::vec3(sp2.getVel() + glm::cross(sp2.getAngVel(), r2));
	glm::vec3 vRel = pointVel2 - pointVel1;

	//Calculate collision impulse magnitude
	float e = (sp1.getCor() + sp2.getCor()) / 2;
	float num = -(1.0f + e) * glm::dot(vRel, collisionNormal);
	float denom = 1.0f / sp1.getMass() + 1.0f / sp2.getMass() + glm::dot(collisionNormal, 
		glm::cross(sp1.getInvInertia() * glm::cross(r1, collisionNormal), r1) +
		glm::cross(sp2.getInvInertia() * glm::cross(r2, collisionNormal), r2));
	float impMag = abs(num / denom);

	//Create collision Impulses
	Impulse imp1;
	imp1.setPoA(PoA);
	imp1.setDir(-collisionNormal);
	imp1.setMag(impMag);

	Impulse imp2;
	imp2.setPoA(PoA);
	imp2.setDir(collisionNormal);
	imp2.setMag(impMag);
	
	//Add Impulses to collection to be applied
	sp1.impulses.push_back(imp1);
	sp2.impulses.push_back(imp2);
}

void CollisionResponse(Sphere & rb, glm::vec3 &overShoot, glm::vec3 &planeNormal)
{
	//Move rb back to collision plane
	rb.setPos(rb.getPos() + overShoot);
	//pause = true;

	//Apply plane based collision impulse
	Impulse imp;

	//Calculate distance from CoM
	glm::vec3 r = rb.getRadius() * -planeNormal;

	//Set point of application
	imp.setPoA(rb.getPos() + r);

	//Calculate velocity of point
	glm::vec3 pointVel = glm::vec3(rb.getVel() + glm::cross(rb.getAngVel(), r));

	//Calculate impulse magnitude
	float num = -(1.0f + rb.getCor()) * glm::dot(pointVel, planeNormal);
	float denom = 1.0f / rb.getMass() + glm::dot(planeNormal, (glm::cross(rb.getInvInertia() * glm::cross(r, planeNormal), r)));
	float impMag = abs(num / denom);

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
}



//Calculate Friction
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