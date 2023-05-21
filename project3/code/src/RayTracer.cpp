// The main ray tracer.

#include <Fl/fl_ask.h>

#include <random>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "SceneObjects/Square.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"

#include "ui/TraceUI.h"
extern TraceUI* traceUI;

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
	ray r( vec3f(0,0,0), vec3f(0,0,0) );
	scene->getCamera()->rayThrough( x,y,r );
	vec3f thresholdMax = scene->getMaxThreshold();
	return traceRay( scene, r, thresholdMax, 0).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth)
{	
	isect i;
	int depthMax = scene->getMaxDepth();

	if( scene->intersect( r, i ) && depth <= depthMax) {
		// YOUR CODE HERE 
		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.
		vec3f color = { 0,0,0 };
		vec3f Q = r.at(i.t); // Intersection Point
		/*Q[0] = Q[0] - RAY_EPSILON;
		Q[1] = Q[1] - RAY_EPSILON;
		Q[2] = Q[2] - RAY_EPSILON;*/
		const Material& m = i.getMaterial();
		
		vec3f phong = m.shade(scene, r, i);
		
		// Adaptive Termination
		// Check whether a color of intersecting point is less than threshold
		if ( phong.length() - thresh[0] <= RAY_EPSILON) {
			return phong;
		}

		color = phong;
	
		vec3f N = i.N; // normal light
		vec3f V = -r.getDirection(); // opposite direction of eye direction

		double u_i = 0;
		double u_t = 0;
		double refracted_index = m.index;
		
		if (N.dot(V) > RAY_EPSILON) {
			// If ray is entering the object.. 
			// direction of light: air(outside) -> inside of object
			u_i = 1.0;
			u_t = refracted_index;
		}
		else
		{
			// if ray is leaving from inside of the object
			// direction of light: inside of object -> air(outside)
			u_i = refracted_index;
			u_t = 1.0;
			N = -N; // Flip the sign of the normal vector
		}

		// Reflection Light Model
		double cos_i = N.dot(V);
		vec3f R = 2 * cos_i * N - V; // Reflected direction
		ray reflectedRay(Q, R);
		
		vec3f opacity = vec3f(1, 1, 1) - m.kt;
		
		// Glossy Reflection
		if (traceUI->glossy()) {
			vec3f glColor = {0,0,0};

			random_device rd;  
			mt19937 gen(rd()); 

			for (int i = 0; i < 40; i++) {
				uniform_real_distribution<double> dis(0.0, 1.0);
				double a = 0.05;
				double random1 = dis(gen);
				double random2 = dis(gen);
				
				double u = -a / 2 + random1 * a;
				double v = -a / 2 + random2 * a;
				
				vec3f gR = R + vec3f(1, 0, 0) * u + vec3f(0, 1, 0) * v;
				
				ray glossyReflectedRay(Q, gR);
				vec3f glossyReflected = prod(m.kr, traceRay(scene, glossyReflectedRay, thresh, depth + 2));
				glColor += prod(opacity, glossyReflected);
			}
			glColor = glColor /20.0;
			color += glColor;
		}
		else {
			vec3f reflected = prod(m.kr, traceRay(scene, reflectedRay, thresh, depth + 1));
			color += prod(opacity, reflected);
		}


		if ( m.kt == vec3f(0, 0, 0)) {
			// if the object is not transmissive
			// then no need to calculate refraction color
			return color;
		}

		
		// Refracted Light Model
		// Calculate a Direction of Refracted Ray
		double u = (u_i / u_t);
		double a = 1 - pow(u, 2) * (1 - pow(cos_i, 2));
		
		if (a < RAY_EPSILON) {
			// Total Internal Reflection
			return color;
		}

		double cos_t = sqrt(a);
		vec3f t = -V*u + N*cos_i*u - N*cos_t;
		
		ray refractedRay(Q, t); // Refracted Ray
		vec3f refractedColor = traceRay(scene, refractedRay, thresh, depth+1);
		vec3f refracted = prod(m.kt, refractedColor);
		color += refracted;

		return color;
	
	} 

	// No intersection with any object.  
	// This ray travels to infinity, so we color
	// it according to the background color, which in this (simple) case
	// is just black.	
	return vec3f( 0.0, 0.0, 0.0 );
}

bool RayTracer::loadBackground(unsigned char* bg, int height, int width) 
{
	if (bg) {
		if (bgMat) {
			delete[] bgMat->bgTexture;
			bgMat->bgTexture = NULL;
			
		}
		else {
			bgMat = new Material();
		}

		bgMat->bgTexture = bg;
		bgMat->height = height;
		bgMat->width = width;

		if (bgObj) {

		}
		bgObj = new Square(scene, bgMat);

		return true;
	}
	return false;
}

void RayTracer::setBgObj()
{
	// Background image will be loaded only after the scene is loaded
	if (m_bSceneLoaded) {
		mat3f m = scene->getCamera()->getRotationMatrix();
	
		mat4f rotM = { vec4f(m[0][0], m[0][1], m[0][2], 0),
					   vec4f(m[1][0], m[1][1], m[1][2], 0),
					   vec4f(m[2][0], m[2][1], m[2][2], 0),
					   vec4f(0, 0, 0, 1) };

		TransformNode* bgTransform = &scene->transformRoot;		
		bgTransform->children.clear();
		vec3f eye = scene->getCamera()->getEye();
		vec3f look = scene->getCamera()->getViewDir();

		double eyeX = abs(eye[0]);
		double eyeY = abs(eye[1]);
		double eyeZ = abs(eye[2]);
		double z = -10;
		double d = sqrt(pow((z - eyeX) * look[0], 2) + pow((z - eyeY) * look[1], 2) + pow((z - eyeZ) * look[2], 2));

		bgObj->setTransform
				(
					bgTransform->createChild(rotM)
					->createChild(mat4f::translate(vec3f(0, 0, z)))
					->createChild(mat4f::scale(vec3f(d,d,1) ))
				);
		
		scene->add(bgObj);
		m_bgLoaded = true;
	}
}

void RayTracer::bgInitScene()
{
	bgMat->showBackground = true;
	scene->initScene();
}

void RayTracer::bgDeleteScene()
{
	if (bgObj) {
		bgMat->showBackground = false;
	}
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
	m_bgLoaded = false;
}

RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	

	if (bgObj) {
		if (m_bgLoaded == false) {
			setBgObj();
		}
	}

	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for (int i = 0; i < buffer_width; ++i) {
			tracePixel(i,j);
		}
}

vec3f RayTracer::adapSampling(double x, double y, double w, double h, int depth)
{
	if (depth >= 4) {
		return trace(scene, x, y);
	}

	vec3f bottomLeft = trace(scene, x, y);
	vec3f col = bottomLeft;
	
	int num = 1;
	
	double newW = (double)w * 2.0;
	double newH = (double)h * 2.0;
	depth += 1;

	double sampX = x + (1.0 / newW);
	double sampY = y; //+ (0 / ((double)h * (double)2));
	vec3f bottomRight = trace(scene, sampX, sampY);

	if ((bottomRight - bottomLeft).length() > 0.1) {
		col += adapSampling(sampX, sampY, newW, newH, depth);
		num++;
	}

	sampX = x; // +(1.0 / newW);
	sampY = y + (1.0 / ((double)h * (double)2));
	vec3f topLeft = trace(scene, sampX, sampY);

	if ( (bottomRight - topLeft).length() > 0.1) {
		col += adapSampling(sampX, sampY, newW, newH, depth);
		num++;
	}

	sampX = x + (1.0 / newW);
	sampY = y + (1.0 / newH);
	vec3f topRight = trace(scene, sampX, sampY);

	if ((bottomRight - topRight).length() > 0.1) {
		col += adapSampling(sampX, sampY, newW, newH, depth);
		num++;
	}

	return col / num;
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;
	vec3f sum = vec3f(0,0,0);

	if( !scene )
		return;

	double numSamp = scene->getNumSampling();
	bool isSupSampling = scene->IsSampling();
	bool isAdapSampling = scene->IsAdapSampling();
	double samp = numSamp * numSamp;
	double x = double(i) / double(buffer_width);
	double y = double(j) / double(buffer_height);
	
	if (isSupSampling) {

		for (int i = -1; i < 2; i++) {
			for (auto j = -1; j < 2; j++) {
				if (i == 0 || j == 0) continue;
				//cout << "i and j: " << i << " " << j << endl;
				double sampX = x + ((double)i / ((double)buffer_width  * samp));
				double sampY = y + ((double)j / ((double)buffer_height * samp));
				sum += trace(scene, sampX, sampY);
			}
		}

		col = sum / (numSamp*numSamp);
	}else if(isAdapSampling){
		//col = trace(scene, x, y);
	
		col = adapSampling(x, y, buffer_width, buffer_height, 0);
	}
	else {
		col = trace(scene, x, y);
	}

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}