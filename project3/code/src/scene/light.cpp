#include <cmath>

#include "light.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation(const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.


	vec3f L = getDirection(P);

	vec3f point = P + L * RAY_EPSILON;
	
	ray light(point, L);
	isect i;
	
	vec3f shadow = { 1.0 , 1.0, 1.0 };

	// Loop until the light ray hits an object that is not transparent
	while (scene->intersect(light, i)) {
		
		if (i.getMaterial().kt.iszero()) {
			return vec3f(0, 0, 0);
		}

		shadow = prod(i.getMaterial().kt, shadow);
		
		point = light.at(i.t) + L * RAY_EPSILON;

		light = ray(point, L);
	}

	// distance to directional light is infinite
	// So, if there is no intersection point
	return shadow;
}

double DirectionalLight::WarnLightAttenuation(const vec3f& P) const
{
	return 1;
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	//return 1.0;

	// First calculated the distane between the source and point 

	double d = vec3f(P - position).length();
	double d_2 = pow(d, 2);
	
	//std::cout << "DisAtten " << constant << " " << linear << " " << quadratic << endl;
	//std::cout << "Distance from light is " << d_2 << std::endl;
 	double distance = 1 / (constant + linear * d + quadratic * d_2);

	distance = min(1.0,   distance);

	return distance;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    
	vec3f L = getDirection(P); // Get direction to light from intersection point
	vec3f point = P + L * RAY_EPSILON;
	ray light(P, L);
	isect i;
	vec3f shadow = { 1.0 , 1.0, 1.0 };

	// Loop until the light ray hits an object that is not transparent
	while (scene->intersect(light, i)) {
		// If there is an intersection (object or light)

		double t_intersect = i.t;
		vec3f intersect_P = light.at(t_intersect);
		
		double length_intersect = (intersect_P - P).length();
		double length_light = (position - P).length();
		// if there is an intersection with the object

		if (length_intersect < length_light) {
			if (i.getMaterial().kt.iszero()) {
				return vec3f(0, 0, 0);
			}
			shadow = prod(i.getMaterial().kt, shadow);
			
			point = light.at(i.t) + L * RAY_EPSILON;
			light = ray(point, L);
		}
	}

	return shadow;
}

double PointLight::WarnLightAttenuation(const vec3f& P) const
{
	return 1;
}


double WarnLight::WarnLightAttenuation(const vec3f& P) const
{
	// Calculate the angle of maximum spot light
	double pi = 3.14159265359;
	// convert radian to degree 
	//std::cout << "Angle is " << angle << std::endl;
	double radian = (double)angle * (pi / 180.0);
	//std::cout << "radian is " << radian << std::endl;
	double cos_A = cos(radian); // where r is the maximum angle of warn light

	vec3f L = -getDirection(P);
	//std::cout << "Light d: " << d;
	//std::cout << "  Ray to light source: " << L << std::endl;
	double cos_L = d.dot(L); 
	// where A is an angle between the direction of warn light 
	// and direction of ray from intersected object to light source
	
	//std::cout << "cos_L is " << cos_L << "  cos_A is " << cos_A << std::endl;

	if (cos_A >= cos_L ) {	
		return 0;
	}

	return pow(cos_L, p);
}
