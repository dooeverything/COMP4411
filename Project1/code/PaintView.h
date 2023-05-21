//
// paintview.h
//
// The header file for painting view of the input images
//

#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
class ImpressionistDoc;

class PaintView : public Fl_Gl_Window
{
public:
	PaintView(int x, int y, int w, int h, const char* l);
	void draw();
	int handle(int event);

	void refresh();
	
	void resizeWindow(int width, int height);

	void SaveCurrentContent();

	void RestoreContent();

	void autoPaint();
	
	void applyBlurSharpen();
	GLubyte* applyBlurAtPixel(Point source, int level);

	double* applySharpenAtPixel(Point source, int level);

	void savePreviousContent();

	ImpressionistDoc *m_pDoc;
	bool blur = false;
	int currentBlurSharpenLevel=1;

private:
	GLvoid* m_pPaintBitstart;
	int		m_nDrawWidth,
			m_nDrawHeight,
			m_nStartRow, 
			m_nEndRow,
			m_nStartCol, 
			m_nEndCol,
			m_nWindowWidth, 
			m_nWindowHeight;
	
	int		sobelX[3][3] = { {-1, 0, 1},
							 {-2, 0, 2},
							 {-1, 0, 1} };
	
	int		sobelY[3][3] = { {1, 2, 1},
							 {0, 0, 0},
							 {-1,-2,-1} };

	Point	start;
	GLubyte result[3];
		
	double* color = NULL;
	double sharpen[3] = {0, 0, 0};
};

#endif