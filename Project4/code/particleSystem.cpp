#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <random>

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem(int n_Particle) 
{
	cout << "Constructor of particles" << endl;
	// TODO

	bake_fps = 30;
	bake_start_time = 0;
	bake_end_time = 20;

	this->n_Particles = n_Particle;	

	simulate = false;
	dirty = false;
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	// TODO
	rainParticles.clear();
	breathParticles.clear();

	for each (Force* f in forces)
	{
		delete f;
	}

	for each (Force * f in breathForces)
	{
		delete f;
	}
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{    
	// TODO
	random_device rd;  // Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> disY(8.0, 11.0);

	// Create Rain Particles
	uniform_real_distribution<> disX( (-n_Particles/2.0 - 2), n_Particles / 2.0);
	uniform_real_distribution<> disZ( -5, 5);


	uniform_real_distribution<> vel(-5.0, 5.0);

	for (double i = 0.0; i < n_Particles; i++) {
		for (double j = 0.0; j < n_Particles; j++) {
			Rain r(1, t);
			r.setPosition( { disX(gen), (double)(disY(gen)), disZ(gen) } );  //i - n_Particles/2.0; //10;  // x
			r.setVelocity({ 0, vel(gen), 0 });

			r.sumForce(forces);
			rainParticles.push_back(r);
		}
	}

	bakeRainParticles(t);

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation **/
void ParticleSystem::stopSimulation(float t)
{
	// TODO
	bake_end_time = t;
	// These values are used by the UI
	simulate = false;
	dirty = true;
}

/** Reset the simulation **/
void ParticleSystem::resetSimulation(float t)
{
    
	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{

	if (simulate) 
	{
		vector<Rain>::iterator it;
		for (it = rainParticles.begin(); it != rainParticles.end(); ++it) 
		{
			(it)->setTime(t);
			(it)->update(t);
		}
		bakeRainParticles(t);


		vector<Breath>::iterator it2;
		vector<Breath>::iterator deathParticle = breathParticles.begin();

		for (it2 = breathParticles.begin(); it2 != breathParticles.end(); ++it2) 
		{
			it2->setTime(t);
			it2->update(t);

			if (it2->getLife() <= 0) deathParticle = it2;
		}
		breathParticles.erase(breathParticles.begin(), deathParticle);
		bakeBreathParticles(t);
	}
}

/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if (t == 0) return;
	// TODO	
	vector<Rain> drawParticles;
	if (!simulate) {
		if (rainBakingList.size() > 0) 
		{
			for each (vector<Rain> bake in rainBakingList) 
			{
				double time = bake.front().getTime();

				if ( abs(time -t) < 0.1) 
				{
					drawParticles = bake;
					break;
				}
			}

			for each (Rain p in drawParticles)
			{
				p.drawParticle();
			}

			return;
		}else 
			return;
	}


	if (t == bake_end_time) {
		return;
	}

	for each ( Rain p in rainParticles /*bake_particles.back()*/ )
	{
		p.drawParticle();
	}

	return;

}

void ParticleSystem::drawBreathParticles(float t) 
{
	if (t == 0) return;

	// TODO	
	vector<Breath> drawParticles;
	if (!simulate)
	{
		if (breathBakingList.size() > 0) 
		{
			for each (vector<Breath> bake in breathBakingList)
			{
				if (bake.size() == 0) continue;

				double time = bake.front().getTime();

				if (abs(time - t) < 0.1)
				{
					drawParticles = bake;
					break;
				}
			}

			for each (Breath p in drawParticles)
			{
				p.drawParticle();
			}

			return;
		}
		else
			return;
	}

	// If the time is end, Do not draw any particles
	if (t == bake_end_time) {
		return;
	}

	if (breathParticles.size() == 0) {
		return;
	}

	for each (Breath p in breathParticles)
	{
		if ( p.getTime() != t) continue;

		if (p.getLife() <= 0) continue;

		p.drawParticle();
	}

	return;
}


void ParticleSystem::createBreathParticles(Vec3d pos, int n_Breath)
{
	random_device rd;  // Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

	uniform_real_distribution<> mass(1, 2);
	uniform_real_distribution<> velX(0.0, 0.5);
	uniform_real_distribution<> velY(0.0, 0.2);
	if (simulate) {
		for (int i = 0; i < n_Breath; ++i) {
			//cout << "Create breath particles " << endl;	
			Breath b(mass(gen), camera);

			Vec3d velocity = { velX(gen), velY(gen), 0 };
			
			b.setPosition(pos);
			b.setVelocity(velocity);
			b.sumForce(breathForces);
			
			breathParticles.push_back(b);
		}
	}
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeRainParticles(float t) 
{
	// TODO
	rainBakingList.push_back(rainParticles);
}

void ParticleSystem::bakeBreathParticles(float t)
{
	breathBakingList.push_back(breathParticles);
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	// TODO
	rainBakingList.clear();
	breathBakingList.clear();
}





