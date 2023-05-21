//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider*			m_thresholdSlider;
	Fl_Slider*			m_samplingSlider;
	
	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;
	Fl_Button*			m_supSampButton;
	Fl_Button*			m_adapSampButton;
	Fl_Button*			m_backgroundButton;
	Fl_Button*			m_glossyReflectionButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();
	double		getThreshold();
	int			getNumSampling();
	
	unsigned char* getBackgroundImage();
	int			getHeight();
	int			getWidth();

	bool		IsSampling();
	bool		IsAdapSampling();
	bool		isBackground();
	bool		glossy();
private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	double		m_nThreshold;
	int			m_nSampling;

	bool		isSampling;
	bool		isAdapSampling;
	bool		background;
	bool		isGlossy;
	
	bool		isSceneLoaded = false;

	unsigned char*	backGroundImage;
	int				height;
	int				width;
// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Widget* o, void* v);
	static void cb_load_backgroundscene(Fl_Widget* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);
	static void cb_thresholdSlides(Fl_Widget* o, void* v);
	static void cb_samplingSlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
	static void cb_sampling(Fl_Widget* o, void* v);
	static void cb_adapSampling(Fl_Widget* o, void* v);
	static void cb_background(Fl_Widget* o, void* v);
	static void cb_glossyReflection(Fl_Widget* o, void* v);
};

#endif
