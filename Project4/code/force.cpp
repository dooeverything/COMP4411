#include "Force.h"
#include "particle.h"

void Gravity::applyForce(Particle* particle) {
	Vec3d force = particle->getForce() + particle->getMass() * gravity;
	particle->setForce(force);
}

void Wind::applyForce(Particle* particle) {
	Vec3d force = particle->getForce() + wind;
	particle->setForce(force);
}

void Viscous::applyForce(Particle* particle)
{
	Vec3d force = particle->getForce() - particle->getVelocity() * viscous;
	particle->setForce(force);
}
