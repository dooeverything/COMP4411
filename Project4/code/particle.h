 // SAMPLE_SOLUTION
#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include "vec.h"
#include "modelerdraw.h"
#include "force.h"
#include "camera.h"

#include <random>
#include <vector>

using namespace std;

class Particle {

	public:
		Particle() {};
		virtual void drawParticle() = 0;
		virtual void update(float t) = 0;
		virtual void sumForce(vector<Force*> forces);

		virtual double getMass() { return mass; }
		virtual Vec3d getForce() { return forceAccum; }
		virtual Vec3d getVelocity() { return velocity;  }
		virtual Vec3d getPosition() { return position; }
		virtual double getTime() { return time; }
		virtual int	getLife() { return life; }

		virtual void setPosition(Vec3d pos) { position = pos; }
		virtual void setVelocity(Vec3d vel) { velocity = vel; }
		virtual void setForce(Vec3d force) { forceAccum = force; }
		virtual void setTime(float t) { time = t; }

	protected:
		//vector<Force*> forces;
		Vec3d forceAccum;
		Vec3d position;
		Vec3d velocity;
		double mass;
		float time;
		int life;
		Camera* camera;
};

class Rain : public Particle {

	public:
		Rain(double m, float t);

		virtual void drawParticle();

		//virtual void sumForce(vector<Force*> forces);

		virtual void update(float t);
};


class Breath : public Particle 
{
public:
	Breath(double m, Camera* camera);


	void billBoarding( Vec3d objPos, Vec3f camPos, Vec3f up);

	virtual void drawParticle();
	virtual void update(float t);
};

#endif	