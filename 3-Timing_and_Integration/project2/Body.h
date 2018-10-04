#pragma once

#include "Mesh.h"

class Body
{
public:
	Body();
	~Body();

	// - GETTERS -
	// Mesh
	Mesh & getMesh() { return m_mesh; }

	// Transform Matrices
	glm::mat3 getTranslate() const { return m_mesh.getTranslate(); }
	glm::mat3 getRotate() const { return m_mesh.getRotate(); }
	glm::mat3 getScale() const { return m_mesh.getScale(); }

	// Dynamic Variables
	glm::vec3& getAcc() { return m_acc; }
	glm::vec3& getVel() { return m_vel; }
	glm::vec3& getPos() { return m_pos; }

	// Physical Properties
	float getMass() const { return m_mass; }
	float getCor() const { return m_cor; }

	// - SETTERS -
	// Visual Properties
	void setMesh(Mesh m) { m_mesh = m; }

	//Dynamic Variables
	void setAcc(const glm::vec3 &vect) { m_acc = vect; }
	void setVel(const glm::vec3 &vect) { m_vel = vect; }
	void setVel(int i, float v) { m_vel[i] = v; }	// Set the ith dimension of the velocity vector
	void setPos(const glm::vec3 &vect) { m_pos = vect; m_mesh.setPos(vect); }
	void setPos(int i, float p) { m_pos[i] = p; m_mesh.setPos(i, p); }	// Set the ith coordinate or the position vector

	// Physical Properties
	void setMass(float mass) { m_mass = mass; }
	void setCor(float cor) { m_cor = cor; }

	// - TRANSFORMATIONS -
	void translate(const glm::vec3 &vect);
	void rotate(float angle, const glm::vec3 &vect);
	void scale(const glm::vec3 &vect);

private:
	// Visual Properties
	Mesh m_mesh;	// Associated Mesh

	// Physical Properties
	float m_mass;	// Mass
	float m_cor;	// Coefficient of Restitution (Elasticity)

	// Dynamic Variables
	glm::vec3 m_acc;	// Acceleration
	glm::vec3 m_vel;	// Veloctity
	glm::vec3 m_pos;	// Position (from mesh)
};

