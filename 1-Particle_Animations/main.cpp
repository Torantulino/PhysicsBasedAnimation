// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"


// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"


// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera  camera(glm::vec3(0.0f, 5.0f, 20.0f));
double lastX = WIDTH / 2.0;
double lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// view and projection matrices
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

// time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// window
GLFWwindow* window = NULL;

//Physics
glm::vec3 initVelocity = glm::vec3(1.0f, 15.0f, 0.0f);
glm::vec3 velocity = glm::vec3(1.0f, 15.0f, 0.0f);
glm::vec3 acceleration = glm::vec3(0.0f, -9.8f, 0.0f);

//Objects
std::vector<Mesh> particles;
std::vector<glm::vec3> velocities;

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	double xOffset = xPos - lastX;
	double yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement((GLfloat) xOffset, (GLfloat) yOffset);
}


void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll((GLfloat)yOffset);
}


// Renderer initialisation
int initRender() {
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Physics-Based Animation", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// remove the mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// moder GLEW approach
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 1;
}

// draw mesh
void draw(const Mesh &mesh)
{
	mesh.getShader().Use();

	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(mesh.getShader().Program, "model");
	GLint viewLoc = glGetUniformLocation(mesh.getShader().Program, "view");
	GLint projLoc = glGetUniformLocation(mesh.getShader().Program, "projection");

	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.getModel()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(mesh.getVertexArrayObject());
	glDrawArrays(GL_TRIANGLES, 0, mesh.getNumIndices());
	glBindVertexArray(0);
}



// main function
int main()
{
	// init renderer
	initRender();	
			
	// create ground plane
	Mesh plane = Mesh::Mesh();
	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));

	// create particle
	Mesh particle1 = Mesh::Mesh();
	//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
	particle1.translate(glm::vec3(0.0f, 2.5f, 0.0f));
	particle1.scale(glm::vec3(.1f, .1f, .1f));
	particle1.rotate((GLfloat) M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
	// allocate shader
	particle1.setShader(Shader("resources/shaders/core.vert", "resources/shaders/core_blue.frag"));

	/*
	CREATE THE PARTICLE(S) YOU NEED TO COMPLETE THE TASKS HERE
	*/
	
	for (int i = 0; i < 10; i++) {
		Mesh p = Mesh::Mesh();
		//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
		p.translate(glm::vec3(0.0f, 2.5f, 0.0f));
		p.scale(glm::vec3(.1f, .1f, .1f));
		p.rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
		// allocate shader
		p.setShader(Shader("resources/shaders/core.vert", "resources/shaders/core_blue.frag"));

		particles.push_back(p);

		velocities.push_back(glm::vec3(i, i, i));
	}


	GLfloat firstFrame = (GLfloat) glfwGetTime();
	float acc = 1.1f;
	// Game loop
	while (!glfwWindowShouldClose(window))
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

		// Check and call events
		glfwPollEvents();
		DoMovement();

		// view and projection matrices
		projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
		view = camera.GetViewMatrix();

		/*
		**	ANIMATIONS
		*/

		// 1 - make particle fall at constant speed using the translate method
		/*
		float velocity = -0.1f;
		particle1.translate(glm::vec3(0.0f, velocity, 0.0f));
		*/

		// 2 - same as above using the setPos method
		/*
		float velocity = -0.1f;
		glm::vec3 curPos = particle1.getTranslate()[3];
		particle1.setPos(glm::vec3(curPos.x, curPos.y + velocity, curPos.z));
		*/

		// 3 - make particle oscillate above the ground plance
		/*
		float yPos = sin(currentFrame);
		glm::vec3 curPos = particle1.getTranslate()[3];
		particle1.setPos(glm::vec3(curPos.x, yPos + 2, curPos.z));
		*/
		// 4 - particle animation from initial velocity and acceleration
		/*
		particle1.setPos(initVelocity * currentFrame + (acceleration * (currentFrame * currentFrame)/2));
		*/

		// 5 - add collision with plane
		/*
		glm::vec3 groundPos = plane.getTranslate()[3];
		glm::vec3 particlePos = particle1.getTranslate()[3];

		velocity += acceleration * deltaTime;

		//Apply equation of motion
		particle1.translate(velocity * deltaTime);

		
		//Check for collision with ground
		if (particlePos.y < groundPos.y) {
			//Move particle back up to ground pos
			particle1.setPos(glm::vec3(particlePos.x, groundPos.y, particlePos.z));
			//Invert velocity in the y axis and apply damping
			velocity = glm::vec3(velocity.x * 0.9, -velocity.y* 0.9, velocity.z);
		}
		*/

		// 6 - Same as above but for a collection of particles
		glm::vec3 groundPos = plane.getTranslate()[3];
		int i = 0;
		for each (Mesh p in particles)
		{
			glm::vec3 particlePos = p.getTranslate()[3];

			velocities[i] += acceleration * deltaTime;

			//Apply equation of motion
			p.translate(velocities[i] * deltaTime);

			//Check for collision with ground
			if (particlePos.y < groundPos.y) {
				//Move particle back up to ground pos
				p.setPos(glm::vec3(particlePos.x, groundPos.y, particlePos.z));
				//Invert velocity in the y axis and apply damping
				velocities[i] = glm::vec3(velocities[i].x * 0.9, -velocities[i].y* 0.9, velocities[i].z);
			}
			i++;
		}

		/*
		**	RENDER 
		*/

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw groud plane
		draw(plane);
		// draw particles
		draw(particle1);
		
				

		glBindVertexArray(0);
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
