#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;
	virtual double WarnLightAttenuation(const vec3f& P) const = 0;

protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual double WarnLightAttenuation(const vec3f& P) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
public:
	PointLight( Scene *scene, const vec3f& pos, const vec3f& color)
		: Light( scene, color ), position( pos ){}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual double WarnLightAttenuation(const vec3f& P) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;
	
	void setConstant(double constant) {
		this->constant = constant;
	}

	void setLinear(double linear) {
		this->linear = linear;
	}
	
	void setQuad(double quadratic) {
		this->quadratic = quadratic;
	}


protected:
	vec3f position;

	double constant=0;
	double linear=1;
	double quadratic=0;
};

class WarnLight : public PointLight
{
public:
	WarnLight(Scene* scene, const vec3f& pos, const vec3f& color, 
			const vec3f& dir, const double& angle, const double& param)
		: PointLight(scene, pos, color), d(dir), angle(angle), p(param) {}
	//virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double WarnLightAttenuation(const vec3f& P) const;
	//virtual vec3f getColor(const vec3f& P) const;
	//virtual vec3f getDirection(const vec3f& P) const;

protected:
	vec3f d;
	double angle;
	double p;
};

#endif // __LIGHT_H__
