#include <cmath>

#include "Square.h"

bool Square::intersectLocal( const ray& r, isect& i ) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if( d[2] == 0.0 ) {
		return false;
	}

	double t = -p[2]/d[2];

	if( t <= RAY_EPSILON ) {
		return false;
	}

	vec3f P = r.at( t );

	if( P[0] < -0.5 || P[0] > 0.5 ) {	
		return false;
	}

	if( P[1] < -0.5 || P[1] > 0.5 ) {	
		return false;
	}

	i.obj = this;
	i.t = t;
	if( d[2] > 0.0 ) {
		i.N = vec3f( 0.0, 0.0, -1.0 );
	} else {
		i.N = vec3f( 0.0, 0.0, 1.0 );
	}

	return true;
}

vec3f Square::getUV(const ray& r) const 
{
	// Transform the ray into the object's local coordinate space
	vec3f pos = transform->globalToLocalCoords(r.getPosition());
	vec3f dir = transform->globalToLocalCoords(r.getPosition() + r.getDirection()) - pos;
	double length = dir.length();
	dir /= length;

	vec3f uv = { -1, -1, -1 };

	if (dir[2] == 0.0) {
		return uv;
	}

	double t = -pos[2] / dir[2];

	if (t <= RAY_EPSILON) {
		return uv;
	}

	ray localRay(pos, dir);
	
	vec3f P = localRay.at(t);

	if (P[0] < -0.5 || P[0] > 0.5) {
		return uv;
	}

	if (P[1] < -0.5 || P[1] > 0.5) {
		return uv;
	}

	double u = maximum(0.0, minimum(P[0] + 0.5, 1));
	double v = maximum(0.0, minimum(P[1] + 0.5, 1));
	uv[0] = u;
	uv[1] = v;

	return uv;
}
