#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    vec3f trace( Scene *scene, double x, double y );
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth);


	void getBuffer( unsigned char *&buf, int &w, int &h );
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );

	void setBgObj();

	bool loadBackground(unsigned char* bg, int height, int width);

	vec3f adapSampling(double x, double y, double w, double h, int depth);

	bool loadScene( char* fn );

	bool sceneLoaded();

	void bgInitScene();

	void bgDeleteScene();
private:
	unsigned char *buffer;
	int buffer_width, buffer_height;
	int bufferSize;
	Scene *scene;

	bool m_bSceneLoaded;
	bool m_bgLoaded;
	MaterialSceneObject* bgObj = NULL;
	Material* bgMat = NULL;
};

#endif // __RAYTRACER_H__
