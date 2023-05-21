//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"
#include "../fileio/bitmap.h"
static bool done;

//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Widget* o, void* v)
{
	TraceUI* pUI = whoami((Fl_Menu_*)o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
			pUI->raytracer->bgDeleteScene();
			pUI->m_backgroundButton->value(0);
			pUI->m_backgroundButton->deactivate();
			pUI->isSceneLoaded = true;
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}
}


void TraceUI::cb_load_backgroundscene(Fl_Widget* o, void* v)
{
	TraceUI* pUI = whoami((Fl_Menu_*)o);

	char* bgName = fl_file_chooser("Open Background Image?", "*.bmp", NULL);

	if (bgName) {
		pUI->backGroundImage = readBMP(bgName, pUI->width, pUI->height);

		if (pUI->isSceneLoaded) {
		
			if (pUI->background) {
				pUI->m_backgroundButton->value(0);
				pUI->raytracer->bgDeleteScene();
			}

			bool temp = pUI->raytracer->loadBackground(pUI->backGroundImage, pUI->height, pUI->width);

			if (temp) {
				pUI->raytracer->setBgObj();
				pUI->m_backgroundButton->activate();
				pUI->background = true;
			}
		}
		else {
			fl_alert("Scene is not loaded, Please load scene first");
		}

	}
}


void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}

void TraceUI::cb_thresholdSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nThreshold = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_samplingSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nSampling = double(((Fl_Slider*)o)->value());
}

void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );

		pUI->m_traceGlWindow->show();

		pUI->raytracer->traceSetup(width, height);
		
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}
				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::cb_sampling(Fl_Widget* o, void* v)
{
	int temp = int(((Fl_Light_Button*)o)->value());
	
	//((TraceUI*)(o->user_data()))->isSampling = (int(((Fl_Light_Button*)o)->value()) == 0) ? false : true;

	if (int(((Fl_Light_Button*)o)->value()) == 0) {
		((TraceUI*)(o->user_data()))->isSampling = false;
		((TraceUI*)(o->user_data()))->m_adapSampButton->activate();
	}
	else {
		((TraceUI*)(o->user_data()))->isSampling = true;
		((TraceUI*)(o->user_data()))->m_adapSampButton->deactivate();
	}

}

void TraceUI::cb_adapSampling(Fl_Widget* o, void* v)
{
	int temp = int(((Fl_Light_Button*)o)->value());


	//((TraceUI*)(o->user_data()))->isAdapSampling = (int(((Fl_Light_Button*)o)->value()) == 0) ? false : true;

	if (int(((Fl_Light_Button*)o)->value()) == 0) {
		((TraceUI*)(o->user_data()))->isAdapSampling = false;
		((TraceUI*)(o->user_data()))->m_supSampButton->activate();
	}
	else {
		((TraceUI*)(o->user_data()))->isAdapSampling = true;
		((TraceUI*)(o->user_data()))->m_supSampButton->deactivate();
	}
}

void TraceUI::cb_background(Fl_Widget* o, void* v)
{
	TraceUI* pUI = ((TraceUI*)(o->user_data()));

	if (int(((Fl_Light_Button*)o)->value()) == 0) {
		((TraceUI*)(o->user_data()))->background = false;
		if(done) pUI->raytracer->bgDeleteScene();
	}else {
		//((TraceUI*)(o->user_data()))->cb_load_scene(o, v);
		pUI->raytracer->bgInitScene();
		((TraceUI*)(o->user_data()))->background = true;
	}

}

void TraceUI::cb_glossyReflection(Fl_Widget* o, void* v) 
{
	TraceUI* pUI = ((TraceUI*)(o->user_data()));

	if (int(((Fl_Light_Button*)o)->value()) == 0) {
		((TraceUI*)(o->user_data()))->isGlossy = false;
	}
	else {
		((TraceUI*)(o->user_data()))->isGlossy = true;
	}
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}

double TraceUI::getThreshold()
{
	return m_nThreshold;
}

int TraceUI::getNumSampling()
{
	return m_nSampling;
}

unsigned char* TraceUI::getBackgroundImage()
{
	return backGroundImage;
}

int TraceUI::getHeight() 
{
	return height;
}

int TraceUI::getWidth()
{
	return width;
}

bool TraceUI::glossy() 
{
	return isGlossy;
}

bool TraceUI::isBackground() 
{
	return background;
}

bool TraceUI::IsSampling()
{
	return isSampling;
}

bool TraceUI::IsAdapSampling()
{
	return isAdapSampling;
}

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Load Background Image...", FL_ALT + 'b', (Fl_Callback*)TraceUI::cb_load_backgroundscene},
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	m_nSize = 150;
	m_nThreshold = 0;
	m_nSampling = 1;

	isSampling = false;
	isAdapSampling = false;
	background = false;
	isGlossy = false;

	m_mainWindow = new Fl_Window(100, 40, 320, 200, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 320, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(1024);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);


		m_thresholdSlider = new Fl_Value_Slider(10, 80, 180, 20, "Thresh");
		m_thresholdSlider->user_data((void*)(this));
		m_thresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_thresholdSlider->labelfont(FL_COURIER);
		m_thresholdSlider->labelsize(12);
		m_thresholdSlider->minimum(0);
		m_thresholdSlider->maximum(1);
		m_thresholdSlider->step(0.01);
		m_thresholdSlider->value(m_nThreshold);
		m_thresholdSlider->align(FL_ALIGN_RIGHT);
		m_thresholdSlider->callback(cb_thresholdSlides);
		

		m_samplingSlider = new Fl_Value_Slider(10, 135, 160, 20, "Num of Sup-Sampling");
		m_samplingSlider->user_data((void*)(this));
		m_samplingSlider->type(FL_HOR_NICE_SLIDER);
		m_samplingSlider->labelfont(FL_COURIER);
		m_samplingSlider->labelsize(12);
		m_samplingSlider->minimum(1);
		m_samplingSlider->maximum(5);
		m_samplingSlider->step(1);
		m_samplingSlider->value(m_nSampling);
		m_samplingSlider->align(FL_ALIGN_RIGHT);
		m_samplingSlider->callback(cb_samplingSlides);


		m_renderButton = new Fl_Button(240, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(240, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_supSampButton = new Fl_Light_Button(10, 105, 100, 25, "&Super Samp");
		m_supSampButton->user_data((void*)(this));
		m_supSampButton->callback(cb_sampling);

		m_adapSampButton = new Fl_Light_Button(115, 105, 100, 25, "&Adap Samp");
		m_adapSampButton->user_data((void*)(this));
		m_adapSampButton->callback(cb_adapSampling);


		m_backgroundButton = new Fl_Light_Button(10, 160, 100, 25, "&Background");
		m_backgroundButton->user_data((void*)(this));
		m_backgroundButton->deactivate();
		m_backgroundButton->callback(cb_background);

		m_glossyReflectionButton = new Fl_Light_Button(115, 160, 100, 25, "&Glossy");
		m_glossyReflectionButton->user_data((void*)(this));
		m_glossyReflectionButton->callback(cb_glossyReflection);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);
    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}