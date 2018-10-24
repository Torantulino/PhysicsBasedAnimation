#pragma once

#include "Mesh.h"
#include "Force.h"

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
	glm::vec3& getPrevPos() { return m_PrevPos; }
	std::vector<Force*> getForces() { return m_forces; }
	glm::vec3& getPrevAcc() { return m_prevAcc; }

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
	void setPrevPos() { m_PrevPos = m_pos; }		// Set previous position

	void setPrevAcc(const glm::vec3&vect) { m_prevAcc = vect; }

	// Physical Properties
	void setMass(float mass) { m_mass = mass; }
	void setCor(float cor) { m_cor = cor; }

	// - TRANSFORMATIONS -
	void translate(const glm::vec3 &vect);
	void rotate(float angle, const glm::vec3 &vect);
	void scale(const glm::vec3 &vect);

	// - PHYSICS -
	//Add force
	void addForce(Force *f) { m_forces.push_back(f); }
	//Apply all forces - RETURNS ACCELERATION
	glm::vec3 applyForces(glm::vec3 pos, glm::vec3 vel, float t, float dt) {
		glm::vec3 fAccumulator = glm::vec3(0.0f, 0.0f, 0.0f);

		for (auto &f : m_forces) {
			fAccumulator += f->apply(getMass(), pos, vel);
		}
		return fAccumulator / getMass();
	}

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
	glm::vec3 m_PrevPos;// Previous Position
	std::vector<Force*> m_forces; //All forces to be applied to this body

	glm::vec3 m_prevAcc;
};

