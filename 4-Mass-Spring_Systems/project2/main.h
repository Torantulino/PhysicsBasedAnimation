void CheckCollisions(Particle &p, Mesh &cube);

void CreateCloth(std::vector<std::vector<Particle> >& p2D, float stiffness, float damping, float rest);

glm::vec3 calcConeForce(glm::vec3 pos);
