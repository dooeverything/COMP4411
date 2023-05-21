//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;
								
	PaintView*			m_paintView;
	OriginalView*		m_origView;

// for brush dialog
	Fl_Window*			m_brushDialog;

	// For color dialog
	Fl_Window* m_colorDialog;
	Fl_Color_Chooser* m_colorChooser;

	// Choices
	Fl_Choice*			m_BrushTypeChoice;
	Fl_Choice*			m_BrushDirectionChoice;
	Fl_Choice*			m_BlurSharpenChoice;

	// Sliders
	Fl_Slider*			m_BrushSizeSlider;
	Fl_Slider*			m_LineBrushWidthSlider;
	Fl_Slider*			m_AngleSizeSlider;
	Fl_Slider*			m_AlphaSlider;
	Fl_Slider*			m_BlurSharpenSlider;
	Fl_Slider*		    m_EdgeThresholdSlider;


	// Buttons
	Fl_Button*          m_ClearCanvasButton;
	Fl_Button*			m_EdgeDetectionButton;
	Fl_Button*			m_AutoPaintButton;

	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc*	getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute
	int					getSize();
	void				setSize(int size);
	int					getWidth();
	int					getAngle();
	void				setAngle(int angle);
	float				getAlpha();
	int					getBrushType();
	int					getDirectionType();
	int					getLevel();
	void				setLevel(int level);
	int					getThreshold();
private:
	ImpressionistDoc*	m_pDoc;		// pointer to document to communicate with the document

	// All attributes here
	
	// Numbers
	int		m_nSize;
	int		m_nWidth;
	int		m_nAngle;
	float	m_nAlpha;
	int		m_nBlurSharpen=1;
	int		m_nThreshold = 100;
	
	// Types
	int		m_brushType = 0;
	int		m_directionType=0;


	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE+1];
	static Fl_Menu_Item		directionsTypeMenu[];
	static Fl_Menu_Item		blurSharpenMenu[];

	static ImpressionistUI*	whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	
	
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void cb_swap_canvas(Fl_Menu_* o, void* v);
	static void cb_auto_paint(Fl_Menu_* o, void* v);
	static void cb_set_colors(Fl_Menu_* o, void* v);
	static void cb_colors(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void cb_load_original_image(Fl_Menu_* o, void* v);
	static void cb_undo(Fl_Menu_* o, void* v);

	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void cb_directionChoice(Fl_Widget* o, void* v);
	static void cb_blurSharpenChoice(Fl_Widget* o, void* v);
	
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void cb_apply_edgeDetection_button(Fl_Widget* o, void* v);
	static void cb_auto_paint_button(Fl_Widget* o, void* v);

	// Callbacks for Sliders
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_widthSlides(Fl_Widget* o, void* v);
	static void cb_angleSlides(Fl_Widget* o, void* v);
	static void cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_blurSharpenSlides(Fl_Widget* o, void* v);
	static void cb_edgeThresholdSlides(Fl_Widget* o, void* v);

	
	// Testing
	static void cb_creator(Fl_Menu_* o, void* v);
	static void cb_debug(Fl_Menu_* o, void* v);
};

#endif
