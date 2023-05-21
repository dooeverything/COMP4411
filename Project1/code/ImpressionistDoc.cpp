// 
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>
#include <vector>
#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "ImpBrush.h"


// Include individual brush headers here.
#include "PointBrush.h"
#include "CircleBrush.h"
#include "LineBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredCircleBrush.h"
#include "BlurSharpen.h"

#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucBitmap		= NULL;
	m_ucPainting	= NULL;
	m_ucBackUP = NULL;
	m_ucBlurMap = NULL;
	m_ucEdgeMap = NULL;
	m_ucPreviousMap = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES] = new LineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]	= new CircleBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS] = new ScatteredPointBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]	= new ScatteredLineBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES] = new ScatteredCircleBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_BLUR_OR_SHARPEN] = new BlurSharpen(this, "Blur or Sharpen");
	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}

//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_brushType = type;
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
	if (type == 6) {
		m_pUI->setSize(1);
		m_pUI->m_paintView->refresh();
		m_pUI->m_paintView->blur = true;
	}else {
		if (m_ucBlurMap) {
			delete[] m_ucBlurMap;
			m_ucBlurMap = NULL;
			getBlur = false;
		}
	}
}

void ImpressionistDoc::setBrushDirection(int direction)
{
	m_direction = direction;
}

// Set the type blur brush or sharpen brush
void ImpressionistDoc::setBlurSharpenType(int type)
{
	m_blurSharpenType = type;
	m_pUI->m_paintView->refresh();
	m_pUI->m_paintView->blur = true;
}

// Return the type of brush ( blur or sharpen )
int ImpressionistDoc::getBlurSharpenType()
{
	return m_blurSharpenType; 
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=readBMP(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	if ( m_ucBitmap ) delete [] m_ucBitmap;
	if ( m_ucPainting ) delete [] m_ucPainting;
	
	if (m_ucBackUP) {
	
		delete[] m_ucBackUP;
		m_ucBackUP = NULL;
		swap = false;
	}	
	
	if (m_ucBlurMap) {
		delete[] m_ucBlurMap;
		m_ucBlurMap = NULL;
		getBlur = false;
	}

	if(m_brushType == 6) 
		m_pUI->m_paintView->blur = true;

	if (m_ucEdgeMap) {
		delete[] m_ucEdgeMap;
		m_ucEdgeMap = NULL;
	} 
	swapFirst = true;
	edgeFirst = true;

	m_ucBitmap		= data;
	m_ucPreviousMap = new unsigned char [width * height * 3];
	//memcpy(m_ucBlurMap, m_ucBitmap, width * height * 3); // copying, Not pointing

	// allocate space for draw view
	m_ucPainting	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);
	memcpy(m_ucPreviousMap, m_ucPainting, width * height * 3);
	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char *iname) 
{

	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

// Swap originalview canvas with paintview canvas
void ImpressionistDoc::swapCanvas()
{
	if (swapFirst) 
	{
		if (m_ucBackUP) delete[] m_ucBackUP;
		m_ucBackUP = new unsigned char [m_nWidth * m_nHeight * 3];
		memcpy(m_ucBackUP, m_ucBitmap, m_nWidth * m_nHeight * 3); // copying, Not pointing
		swapFirst = false;
		swap = true;
	}

	if (m_ucBitmap && m_ucPainting) 
	{
		unsigned char* originalView = m_ucBitmap;
		
		// Swap
		m_ucBitmap = m_ucPainting;
		m_ucPainting = originalView;

		m_pUI->m_origView->refresh();
		m_pUI->m_paintView->refresh();	
	}
}

// Auto Paint
void ImpressionistDoc::autoPaint() 
{ 
	//m_pUI->m_paintView->autoPaint();
	m_pUI->m_paintView->refresh();
	//m_pUI->m_paintView->SaveCurrentContent();
	autoFill = true;
}

void ImpressionistDoc::applyEdgeDetection() 
{	
	m_blurSharpenType = 0;
	m_pUI->setLevel(1);
	m_pUI->m_paintView->applyBlurSharpen(); // Apply Blur Filter on the image to reduce noise

	m_ucEdgeMap = new unsigned char[m_nHeight * m_nWidth * 3];

	for (int i = 0; i < m_nHeight; i++) {
		for (int j = 0; j < m_nWidth; j++) {
			Point point = Point(j, i);
			
			GLubyte color = edgeDetectionAtPixel(point);

			m_ucEdgeMap[3 * (i * m_nWidth + j)] = color;
			m_ucEdgeMap[3 * (i * m_nWidth + j) + 1] = color;
			m_ucEdgeMap[3 * (i * m_nWidth + j) + 2] = color;
		}
	}

	if (edgeFirst && swapFirst) { // If edge detection is called first time, then save the original image to the back up space
		if (m_ucBackUP) delete[] m_ucBackUP;
		m_ucBackUP = new unsigned char[m_nWidth * m_nHeight * 3];
		memcpy(m_ucBackUP, m_ucBitmap, m_nWidth * m_nHeight * 3); // copying, Not changing a pointer
		edgeFirst = false;
		swapFirst = false;
		swap = true;
	}

	// Delete the image that is pointed by m_ucBitmap(it could be original or previous )
	delete[] m_ucBitmap;
	//memcpy(m_ucBitmap, m_ucEdgeMap, m_nWidth * m_nHeight * 3);
	
	m_ucBitmap = m_ucEdgeMap;
	m_ucEdgeMap = NULL;
	m_pUI->m_origView->refresh();
		
	delete[] m_ucBlurMap;
	m_ucBlurMap = NULL;
	getBlur = false;
}

GLubyte ImpressionistDoc::edgeDetectionAtPixel(Point source) 
{

	double sobelX[3][3] = { {-1, 0, 1},
							{-2, 0, 2},
							{-1, 0, 1} };

	double sobelY[3][3] = { {1, 2, 1},
							{0, 0, 0},
							{-1,-2,-1} };

	GLubyte gray[3][3];
	GLubyte red;
	GLubyte green;
	GLubyte blue;


	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Point temp;
			
			temp.x = source.x + i - 1;
			temp.y = source.y + j - 1;

			GLubyte colors[3];
			// Get the original color from temp
			memcpy(colors, GetOriginalPixel(temp), 3);
			red = (int)colors[0];
			green = (int)colors[1];
			blue = (int)colors[2];
			//GLubyte gray = 0.229 * red + 0.578 * green + 0.114 * blue; // convert to grey-level image 

			gray[i][j] = 0.229 * red + 0.578 * green + 0.114 * blue;

		}
	}

	double Gx = 0;
	double Gy = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Gx += gray[i][j] * sobelX[i][j];
			Gy += gray[i][j] * sobelY[i][j];
		}
	}

	int threshold = m_pUI->getThreshold();

	GLubyte magnitude = sqrt( (Gx * Gx) + (Gy * Gy) );

	if (magnitude < threshold) {
		magnitude = 0;
	}

	return magnitude;

}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;
	
	if(getBlur)
		return (GLubyte*)(m_ucBlurMap + 3 * (y * m_nWidth + x));
	
	if(swap)
		return (GLubyte*)(m_ucBackUP + 3 * (y*m_nWidth + x));

	return (GLubyte*)(m_ucBitmap + 3 * (y * m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const Point p )
{
	return GetOriginalPixel( p.x, p.y );
}

void ImpressionistDoc::loadOriginal()
{
	if (m_ucBackUP) {
		if (m_ucBitmap) delete[] m_ucBitmap;
		m_ucBitmap = new unsigned char[m_nWidth * m_nHeight * 3];
		memcpy(m_ucBitmap, m_ucBackUP, m_nWidth * m_nHeight * 3);
		m_pUI->m_origView->refresh();
	}
}

void ImpressionistDoc::undo()
{
	memcpy(m_ucPainting, m_ucPreviousMap, m_nPaintHeight * m_nPaintWidth * 3);
	m_pUI->m_paintView->refresh();
}