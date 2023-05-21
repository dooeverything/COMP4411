//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>

#include "impressionistUI.h"
#include "impressionistDoc.h"

/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the 
project.  You can copy and paste these into your code and then change them to 
make them look how you want.  Descriptions for all of the widgets here can be found 
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------
	
	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions
		
		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
    m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v) 
	{	
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}
	

//------------Choice---------------------------------------
	
	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);
	
	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------
	
	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)	
{
	return ( (ImpressionistUI*)(o->parent()->user_data()) );
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}


//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	ImpressionistDoc *pDoc=whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp" );
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen/clicked
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_brushDialog->show();
}

// Brings up the color shader dialog
void ImpressionistUI::cb_colors(Fl_Menu_* o, void* v)
{
	whoami(o)->m_colorDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc=whoami(o)->getDocument();

	pDoc->clearCanvas();
}

// Swap the originalview canvas with paintview canvas
void ImpressionistUI::cb_swap_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	pDoc->swapCanvas();
}

// Manipulate the color of the image
void ImpressionistUI::cb_set_colors(Fl_Menu_* o, void* v)
{
	
}

// Auto Paint
void ImpressionistUI::cb_auto_paint(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	pDoc->autoPaint();
}

void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	pDoc->undo();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v) 
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}

void ImpressionistUI::cb_load_original_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	pDoc->loadOriginal();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("Impressionist FLTK version for COMP4411, Spring 2022");
}

// Testing
void ImpressionistUI::cb_creator(Fl_Menu_* o, void* v) 
{
	fl_message("DOO Dongkyu(20379251)");
}

void ImpressionistUI::cb_debug(Fl_Menu_* o, void* v)
{
	
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
	ImpressionistDoc* pDoc=pUI->getDocument();

	int type=(int)v;

	pDoc->setBrushType(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	// Get a document(pDoc-ImpressionistDoc Class) that will be cleared
	ImpressionistDoc * pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	// Clear the canvas
	pDoc->clearCanvas();
}

void ImpressionistUI::cb_apply_edgeDetection_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->applyEdgeDetection();
}

void ImpressionistUI::cb_auto_paint_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->autoPaint();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
}

//-----------------------------------------------------------
// Update the width of Line brush
// Slider
void ImpressionistUI::cb_widthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nWidth = int(((Fl_Slider*)o)->value());
}

//-----------------------------------------------------------
// Update the Angle of Line brush
// Slider
void ImpressionistUI::cb_angleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAngle = int(((Fl_Slider*)o)->value());
}

//-----------------------------------------------------------
// Update the Alpha of brush
// Slider
void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nAlpha = float(((Fl_Slider*)o)->value());
}

// Update the level of blur or sharpen
void ImpressionistUI::cb_blurSharpenSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nBlurSharpen = int(((Fl_Slider*)o)->value());
	((ImpressionistUI*)(o->user_data()))->m_paintView->refresh();
}

void ImpressionistUI::cb_edgeThresholdSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nThreshold = int(((Fl_Slider*)o)->value());
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w,h);
	m_origView->size(w,h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize( int size )
{
	m_nSize=size;

	if (size<=40) 
		m_BrushSizeSlider->value(m_nSize);
}

//------------------------------------------------
// Return the Line Brush Width
//------------------------------------------------
int ImpressionistUI::getWidth()
{
	return m_nWidth;
}

//------------------------------------------------
// Return the Line Brush Angle
//------------------------------------------------
int ImpressionistUI::getAngle()
{
	return m_nAngle;
}

//------------------------------------------------
// Set the Line Brush Angle
//------------------------------------------------
void ImpressionistUI::setAngle(int angle)
{
	m_nAngle = angle;
	m_AngleSizeSlider->value(m_nAngle);
}

//------------------------------------------------
// Return the Alpha
//------------------------------------------------
float ImpressionistUI::getAlpha()
{
	return m_nAlpha;
}

//------------------------------------------------
// Return the Brush Type
//------------------------------------------------
int ImpressionistUI::getBrushType()
{
	return m_directionType;
}

//------------------------------------------------
// Return the Brush Direction Type
//------------------------------------------------
int ImpressionistUI::getDirectionType()
{
	return m_directionType;
}

//------------------------------------------------
// Return the level of Blur or Sharpen
//------------------------------------------------
int ImpressionistUI::getLevel() {
	return m_nBlurSharpen;
}


void ImpressionistUI::setLevel(int level)
{
	m_nBlurSharpen = level;
	m_BlurSharpenSlider->value(level);
} 

int ImpressionistUI::getThreshold()
{
	return m_nThreshold;
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU }, // FL_SUBMENU gives following sub-menu
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load_image },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save_image },
		//{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes }, 
		// MENU_DIVIDER divides Menu by drawing a line between each menu
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
		{ 0 },

	{ "&Functions",		0, 0, 0, FL_SUBMENU },
		{ "&Brushes",	FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_brushes },
		{ "&Colors", FL_ALT + 'r', ((Fl_Callback*)ImpressionistUI::cb_colors), 0, FL_MENU_DIVIDER},

		{ "&Undo", FL_ALT +'z', (Fl_Callback*)ImpressionistUI::cb_undo},
		{ "&Auto Paint", FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_auto_paint},
		{ "&Swap Canvas", FL_ALT + 'w', (Fl_Callback*)ImpressionistUI::cb_swap_canvas},
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_clear_canvas},
		{ 0 },

	{ "&Canvas", 0, 0, 0, FL_SUBMENU},
		{ "&Original Image", FL_ALT +'o', (Fl_Callback*)ImpressionistUI::cb_load_original_image},
		{0},


	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
		{ "&Creator", FL_ALT + 't', (Fl_Callback *)ImpressionistUI::cb_creator},
		{ "&Debug", FL_ALT + 'd', (Fl_Callback*)ImpressionistUI::cb_debug},
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE+1] = {
  {"Points",			FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_POINTS},
  {"Lines",				FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_LINES},
  {"Circles",			FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT+'q', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT+'m', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT+'d', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_SCATTERED_CIRCLES},
  {"Blur or Sharpen",   FL_ALT+'s', (Fl_Callback *)ImpressionistUI::cb_brushChoice, (void *)BRUSH_BLUR_OR_SHARPEN},
  {0}
};

// Brush Direction menu Definition
Fl_Menu_Item ImpressionistUI::directionsTypeMenu[NUM_DIRECTION_TYPE + 1] = {
  {"Slider/Right Mouse",FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_directionChoice, (void*)SLIDER_RIGHTMOUSE},
  {"Gradient",FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_directionChoice, (void*)GRADIENT},
  {"Brush Direction",FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_directionChoice, (void*)BRUSH_DIRECTION},
  {0}
};

// Blur or Sharpen Menu definition
Fl_Menu_Item ImpressionistUI::blurSharpenMenu[3] = {
	{"Blur", FL_ALT + 'u', (Fl_Callback*)ImpressionistUI::cb_blurSharpenChoice, (void*)BLUR},
	{"Sharpen", FL_ALT + 'h', (Fl_Callback*)ImpressionistUI::cb_blurSharpenChoice, (void*)SHARPEN},
	{0}
};

//-----The choice callback-------------------------
void ImpressionistUI::cb_directionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setBrushDirection(type);
}

// Blur or Sharpen Choice callback
void ImpressionistUI::cb_blurSharpenChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;

	pDoc->setBlurSharpenType(type);

}

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
		m_menubar->menu(menuitems);

		// Create a group that will hold two sub windows inside the main
		// window
		Fl_Group* group = new Fl_Group(0, 25, 600, 275);

			// install paint view window
			m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
			m_paintView->box(FL_DOWN_FRAME);

			// install original view window
			m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
			m_origView->box(FL_DOWN_FRAME);
			m_origView->deactivate();

		group->end();
		Fl_Group::current()->resizable(group);
    m_mainWindow->end();

	// init values
	m_nSize = 10;
	m_nWidth = 1;
	m_nAngle = 0;
	m_nAlpha = 1.00;
	m_nBlurSharpen = 1;

	// brush dialog definition
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
	{
		// Add a brush type choice to the dialog
		m_BrushTypeChoice = new Fl_Choice(50,10,150,25,"&Brush");
		m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushTypeChoice->menu(brushTypeMenu);
		m_BrushTypeChoice->callback(cb_brushChoice);

		// Add a brush direction choice to the dialog
		m_BrushDirectionChoice = new Fl_Choice(114, 40, 150, 25, "&Stroke Direction");
		m_BrushDirectionChoice->user_data((void*)this);
		m_BrushDirectionChoice->menu(directionsTypeMenu);
		m_BrushDirectionChoice->callback(cb_directionChoice);

		m_AutoPaintButton = new Fl_Button(270, 40, 100, 25, "&Auto Paint");
		m_AutoPaintButton->user_data((void*)(this));
		m_AutoPaintButton->callback(cb_auto_paint_button);

		m_ClearCanvasButton = new Fl_Button(240,10,150,25,"&Clear Canvas");
		m_ClearCanvasButton->user_data((void*)(this));
		m_ClearCanvasButton->callback(cb_clear_canvas_button);


		// Add brush size slider to the dialog 
		m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
		m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
        m_BrushSizeSlider->labelfont(FL_COURIER);
        m_BrushSizeSlider->labelsize(12);
		m_BrushSizeSlider->minimum(1);
		m_BrushSizeSlider->maximum(40);
		m_BrushSizeSlider->step(1);
		m_BrushSizeSlider->value(m_nSize);
		m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
		m_BrushSizeSlider->callback(cb_sizeSlides);

		// Add Line Brush Size Slider
		m_LineBrushWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Width");
		m_LineBrushWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_LineBrushWidthSlider->type(FL_HOR_NICE_SLIDER);
		m_LineBrushWidthSlider->labelfont(FL_COURIER);
		m_LineBrushWidthSlider->labelsize(12);
		m_LineBrushWidthSlider->minimum(1);
		m_LineBrushWidthSlider->maximum(40);
		m_LineBrushWidthSlider->step(1);
		m_LineBrushWidthSlider->value(m_nWidth);
		m_LineBrushWidthSlider->align(FL_ALIGN_RIGHT);
		m_LineBrushWidthSlider->callback(cb_widthSlides);

		// Add Angle Brush Size Slider
		m_AngleSizeSlider = new Fl_Value_Slider(10, 140, 300, 20, "Angle");
		m_AngleSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_AngleSizeSlider->type(FL_HOR_NICE_SLIDER);
		m_AngleSizeSlider->labelfont(FL_COURIER);
		m_AngleSizeSlider->labelsize(12);
		m_AngleSizeSlider->minimum(0);
		m_AngleSizeSlider->maximum(359);
		m_AngleSizeSlider->step(1);
		m_AngleSizeSlider->value(m_nAngle);
		m_AngleSizeSlider->align(FL_ALIGN_RIGHT);
		m_AngleSizeSlider->callback(cb_angleSlides);

		// Add Alpha Brush Size Slider
		m_AlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
		m_AlphaSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_AlphaSlider->type(FL_HOR_NICE_SLIDER);
		m_AlphaSlider->labelfont(FL_COURIER);
		m_AlphaSlider->labelsize(12);
		m_AlphaSlider->minimum(0.00);
		m_AlphaSlider->maximum(1.00);
		m_AlphaSlider->step(0.01);
		m_AlphaSlider->value(m_nAlpha);
		m_AlphaSlider->align(FL_ALIGN_RIGHT);
		m_AlphaSlider->callback(cb_alphaSlides);

		// Add Blur or Sharpen choice to the dialog
		m_BlurSharpenChoice = new Fl_Choice(114, 200, 150, 25, "&Blur or Sharpen");
		m_BlurSharpenChoice->user_data((void*)this);
		m_BlurSharpenChoice->menu(blurSharpenMenu);
		m_BlurSharpenChoice->callback(cb_blurSharpenChoice);

		// Blur or Sharpen Level Slider
		m_BlurSharpenSlider = new Fl_Value_Slider(10, 230, 300, 20, "Level");
		m_BlurSharpenSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_BlurSharpenSlider->type(FL_HOR_NICE_SLIDER);
		m_BlurSharpenSlider->labelfont(FL_COURIER);
		m_BlurSharpenSlider->labelsize(12);
		m_BlurSharpenSlider->minimum(1);
		m_BlurSharpenSlider->maximum(5);
		m_BlurSharpenSlider->step(1);
		m_BlurSharpenSlider->value(m_nBlurSharpen);
		m_BlurSharpenSlider->align(FL_ALIGN_RIGHT);
		m_BlurSharpenSlider->callback(cb_blurSharpenSlides);
	
		// Edge Detection Threshold Level Slider
		m_EdgeThresholdSlider = new Fl_Value_Slider(10, 260, 200, 20, "Edge Threshold");
		m_EdgeThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_EdgeThresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_EdgeThresholdSlider->labelfont(FL_COURIER);
		m_EdgeThresholdSlider->labelsize(12);
		m_EdgeThresholdSlider->minimum(0);
		m_EdgeThresholdSlider->maximum(200);
		m_EdgeThresholdSlider->step(1);
		m_EdgeThresholdSlider->value(m_nThreshold);
		m_EdgeThresholdSlider->align(FL_ALIGN_RIGHT);
		m_EdgeThresholdSlider->callback(cb_edgeThresholdSlides);
		
		m_EdgeDetectionButton = new Fl_Button(315, 260, 70, 20, "&Apply");
		m_EdgeDetectionButton->user_data((void*)(this));
		m_EdgeDetectionButton->callback(cb_apply_edgeDetection_button);
	}
    m_brushDialog->end();	

	// Color Pallete dialog 
	m_colorDialog = new Fl_Window(300, 225, "Choose your color");
	{
		m_colorChooser = new Fl_Color_Chooser(0, 0, 300, 225, "Colors");
		m_colorChooser->rgb(1, 1, 1);
	}


}
