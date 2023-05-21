#pragma once
#ifndef RAIN_PARTICLE_SYSTEM_H
#define RAIN_PARTICLE_SYSTEM_H


#include "particleSystem.h"


class RainParticleSystem : public ParticleSystem {

public:
	RainParticleSystem(int num_particle);

	virtual void startSimulation(float t);

	virtual void computeForcesAndUpdateParticles(float t);

};

#endif RAIN_PARTICLE_SYSTEM_H