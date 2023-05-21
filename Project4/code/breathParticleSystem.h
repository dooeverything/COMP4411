#pragma once
#include "particleSystem.h"

class BreathParticleSystem : public ParticleSystem 
{


public:
	class BreathParticleSystem(int numParticle) : ParticleSystem(numParticle) {}


	virtual void createParticles(Vec3d pos);


	virtual void drawParticles(float t);

protected:

	vector<Breath> particles;
	vector<vector<Breath>> bakingList;


};