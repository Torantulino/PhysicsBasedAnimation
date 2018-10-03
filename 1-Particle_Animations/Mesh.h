#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "OBJloader.h"
#include "Shader.h"

/* 
** VERTEX CLASS 
*/
class Vertex {
public:
	Vertex(const glm::vec3& coord) {
		this->coord = coord;
	}
protected:
private:
	glm::vec3 coord;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

/* 
** MESH CLASS 
*/
class Mesh
{
public:

	/*
	** CONSTRUCTORS
	*/

	// default constructor creates a horizontal plane or dimensions 1 x 1 centered on the origin
	Mesh();
	// create mesh from a .obj file
	Mesh(const std::string& fileName);
	virtual ~Mesh();


	/*
	** GET AND SET METHODS
	*/

	// getModel computes the model matrix any time it is required
	glm::mat4 getModel() const{ return getTranslate() * getRotate() * getScale();}
	glm::mat4 getTranslate() const{ return m_translate; }
	glm::mat4 getRotate() const{ return m_rotate; }
	glm::mat4 getScale() const{ return m_scale; }
	Shader getShader() const { return m_shader; }
	GLuint getVertexArrayObject() const{ return m_vertexArrayObject; }
	unsigned int getNumIndices() const{ return m_numIndices; }

	// set position of mesh center to specified 3D position vector
	void setPos(const glm::vec3 &position) {
		m_translate[3][0] = position[0];
		m_translate[3][1] = position[1];
		m_translate[3][2] = position[2];
	}
	// set i_th coordinate of mesh center to float p (x: i=0, y: i=1, z: i=2)
	void setPos(int i, float p) { m_translate[3][i] = p; }

	// allocate shader to mesh
	void setShader(const Shader &shader) {
		m_shader = shader;
		m_shader.Use();
	}


	/* 
	** INITIALISATION AND UTILITY METHODS
	*/

	// initialise transform matrices to identity
	void initTransform();
	// create mesh from vertices
	void initMesh(Vertex* vertices, unsigned int numVertices);
	// create mesh from model (typically loaded from a file)
	void InitMesh(const IndexedModel& model);
	// load .obj file
	void loadOBJ(const char * path,
		std::vector < glm::vec3 > & out_vertices,
		std::vector < glm::vec2 > & out_uvs,
		std::vector < glm::vec3 > & out_normals
	);


	/*
	** TRANSFORMATION METHODS
	*/
	
	// translate mesh by a vector
	void translate(const glm::vec3 &vect);
	// rotate mesh by a vector
	void rotate(const float &angle, const glm::vec3 &vect);
	// scale mesh by a vector
	void scale(const glm::vec3 &vect);


private:
	enum {
		POSITION_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_numIndices;
	glm::mat4 m_translate;
	glm::mat4 m_rotate;
	glm::mat4 m_scale;

	Shader m_shader;
};


