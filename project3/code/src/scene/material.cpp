#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.
	vec3f Q = r.at(i.t); // an intersection point
	vec3f N = i.N;
	vec3f V = -r.getDirection();

	vec3f opaque = vec3f(1, 1, 1) - kt;
	vec3f ambientLight = scene->ambient_light;

	vec3f ambient= prod(ka, ambientLight);

	
	if (i.obj) {
		vec3f emissionTexture = vec3f(0, 0, 0);
		if (bgTexture && showBackground) {

			// Get u,v of current ray
			vec3f uv = i.obj->getUV(r);
			if (uv[0] >=0 && uv[1] >= 0) {
				unsigned char* pixel = bgTexture + ((int)(uv[0] * width) + (int)(uv[1] * height) *width ) * 3;
				emissionTexture[0] = (double)pixel[0]/255.0;
				emissionTexture[1] = (double)pixel[1]/255.0;
				emissionTexture[2] = (double)pixel[2]/255.0;
				return emissionTexture + ambient;
			}
		}
		else if (bgTexture && !showBackground) {
			return vec3f(0, 0, 0);
		}
	}

 	vec3f color = prod(opaque, (ke + ambient) );

	list<Light*>::const_iterator it;
	int count = 0;
	for (it = scene->beginLights(); it != scene->endLights(); ++it) {
		Light* light = *it;

		//vec3f n = N;
		vec3f L = light->getDirection(Q);
		vec3f lightIntensity = light->getColor(Q);
		
		double distAtten = light->distanceAttenuation(Q);
		vec3f shadowAtten = light->shadowAttenuation(Q);
		double warnAtten = light->WarnLightAttenuation(Q);
		vec3f atten = distAtten * shadowAtten * warnAtten;

		if (N.dot(L) < RAY_EPSILON && opaque != vec3f(1,1,1) ) N = -N;

		double diffuse = max( N.dot(L), 0.0 );

		vec3f R = 2 * (N.dot(L)) * N - L;
		double specularAngle = max(R.dot(V), 0.0 );
		double specular = pow(specularAngle, 30);

		vec3f diffSpec = prod( prod((kd * diffuse), opaque) + (ks * specular), lightIntensity);
		
		color += prod( atten, diffSpec);
	}

	return color;
}
