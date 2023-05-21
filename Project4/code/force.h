#pragma once

#ifndef FORCE_H
#define FORCE_H

//#include "particle.h"
#include "vec.h"

class Particle;

class Force {

	public:
		virtual void applyForce(Particle* particle) = 0;
};

class Gravity : public Force {

	public:
		Gravity(Vec3d g) : gravity(g) {};
		virtual void applyForce(Particle* particle);
		
	protected:
		Vec3d gravity;
};

class Wind : public Force {

	public:
		Wind(Vec3d w) : wind(w) {};
		/*virtual void applyForce	*/	
		virtual void applyForce(Particle* particle);
	
	protected:
		Vec3d wind;
};

class Viscous : public Force {
	
	public:
		Viscous(double v) : viscous(v) {};
		virtual void applyForce(Particle* particle);
		
	protected:
		double viscous;
};

#endif