 // SAMPLE_SOLUTION
#include "particle.h"

#include <FL/gl.h>
#include <FL/glut.h>
#include <GL/glu.h>
#include <cstdio>


Rain::Rain(double m, float t)
{
	mass = m;
	time = t;
	life = 20;
}

void Rain::drawParticle() 
{
	drawRain(position[0], position[1], position[2]);
}

void Particle::sumForce(vector<Force*> forces)
{
	vector<Force*>::const_iterator it;
	for (it = forces.begin(); it != forces.end(); ++it)
	{
		(*it)->applyForce(this);
	}
}

void Rain::update(float t)
{

	random_device rd;  // Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> restartX(-7.5, 7.5);
	uniform_real_distribution<> disZ(-5, 3);
	
	uniform_real_distribution<> velY(0.0, 1.0);

	position += t * velocity * 0.01;
	velocity += t * (forceAccum / mass) * 0.5;
	life -= 1;
	//cout << "After update: " << position[2] << endl;

	if (position[1] <= 0 || position[0] > 7.5) 
	{
		position = { restartX(gen), 10.0, position[2] };
		velocity = { 0,velY(gen),0 };
	}

}


Breath::Breath(double m, Camera* c) 
{
	mass = m;
	life = 20;
	camera = c;
}


void Breath::drawParticle() 
{
	//cout << "Draw PartcleS!" << endl;
	
	Vec3d scale = { 0.25, 0.25, 1 };

	Vec3f up = camera->getUp();
	Vec3f camPos = camera->getPos();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		billBoarding(position, camPos, up);
		glScaled(scale[0], scale[1], scale[2]);
		drawBreathParticle(position[0], position[1], position[2]);
	glPopMatrix();
	
	glDisable(GL_BLEND);
	
}


void Breath::update(float t)
{
	position += sqrt(t) * velocity * 0.05;
	velocity += (t/2.0) * (forceAccum / mass) * 0.1;
	life -= 1;
}

void Breath::billBoarding(Vec3d objPos, Vec3f camPos, Vec3f up)
{
	glTranslatef(objPos[0], objPos[1], objPos[2]);
	
	Vec3f objToCam = { (float)objPos[0]-camPos[0], 
					   (float)objPos[1]-camPos[1], 
					   (float)objPos[2]-camPos[2] };
	
	objToCam.normalize();
	
	Vec3f rightVec = objToCam ^ up;
	up = objToCam ^ rightVec;

	GLfloat billBoard[16];
	billBoard[0] = rightVec[0];
	billBoard[1] = rightVec[1];
	billBoard[2] = rightVec[2];
	billBoard[3] = 0.0;
	
	billBoard[4] = up[0];
	billBoard[5] = up[1];
	billBoard[6] = up[2];
	billBoard[7] = 0.0;
	
	billBoard[8] = -objToCam[0];
	billBoard[9] = -objToCam[1];
	billBoard[10] = -objToCam[2];
	billBoard[11] = 0.0;

	billBoard[12] = 0.0;
	billBoard[13] = 0.0;
	billBoard[14] = 0.0;
	billBoard[15] = 1.0;
	
	glMultMatrixf(billBoard);
}
