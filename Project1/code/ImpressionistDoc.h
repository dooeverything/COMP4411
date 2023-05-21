// 
// impressionistDoc.h
//
// header file for Doc 
//

#ifndef ImpressionistDoc_h
#define ImpressionistDoc_h

#include "impressionist.h"
#include "bitmap.h"

class ImpressionistUI;

class ImpressionistDoc 
{
public:
	ImpressionistDoc();

	void	setUI(ImpressionistUI* ui);		// Assign the UI to use

	int		loadImage(char *iname);			// called by the UI to load image
	int		saveImage(char *iname);			// called by the UI to save image

	void	autoPaint();
	void	swapCanvas();
	int     clearCanvas();     // called by the UI to clear the drawing canvas4
	void	loadOriginal();

	void applyEdgeDetection();
	GLubyte	edgeDetectionAtPixel(Point source);
	
	void undo();

	// Set functions
	void	setBrushType(int type);			// called by the UI to set the brushType
	void	setBrushDirection(int direction); // called by the UI to set the Brush Direction
	void	setBlurSharpenType(int type);
	void	setSize(int size);				// set the UI size
	
	// Get functions
	int		getSize();						// get the UI size
	int		getBlurSharpenType();
	char*	getImageName();					// get the current image name

// Attributes
public:
	// Dimensions of original window.
	int				m_nWidth, 
					m_nHeight;
	// Dimensions of the paint window.
	int				m_nPaintWidth, 
					m_nPaintHeight;	

	// Bitmaps for original image and painting.
	unsigned char*	m_ucBitmap;
	unsigned char*	m_ucPainting;
	
	// Bitmaps for backup
	unsigned char* m_ucBackUP;
	
	// Bitmaps for bluring 
	unsigned char* m_ucBlurMap;

	// Bitmaps for edge Detection
	unsigned char* m_ucEdgeMap;

	// Bitmaps for saving previous content
	unsigned char* m_ucPreviousMap;

	// The current active brush.
	ImpBrush*			m_pCurrentBrush;	
	
	// Size of the brush.
	int m_nSize;	
	int m_brushType=0;
	int m_direction=0;
	int m_blurSharpenType = 0;

	bool autoFill = false;
	bool getBlur = false;
	bool swap = false;
	
	bool swapFirst = true;
	bool edgeFirst = true;
	ImpressionistUI*	m_pUI;

// Operations
public:
	// Get the color of the original picture at the specified coord
	GLubyte* GetOriginalPixel( int x, int y );   
	
	// Get the color of the original picture at the specified point	
	GLubyte* GetOriginalPixel( const Point p );  


private:
	char			m_imageName[256];

};

extern void MessageBox(char *message);

#endif
