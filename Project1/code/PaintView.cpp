//
// paintview.cpp
//
// The code maintaining the painting view of the input images
//

#include "impressionist.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "paintview.h"
#include "ImpBrush.h"
#include "math.h"


//#include <vector>
//#include <algorithm>


#define LEFT_MOUSE_DOWN		1
#define LEFT_MOUSE_DRAG		2
#define LEFT_MOUSE_UP		3
#define RIGHT_MOUSE_DOWN	4
#define RIGHT_MOUSE_DRAG	5
#define RIGHT_MOUSE_UP		6


#ifndef WIN32
#define min(a, b)	( ( (a)<(b) ) ? (a) : (b) )
#define max(a, b)	( ( (a)>(b) ) ? (a) : (b) )
#endif

static int		eventToDo;
static int		isAnEvent=0;
static Point	coord;

PaintView::PaintView(int			x, 
					 int			y, 
					 int			w, 
					 int			h, 
					 const char*	l) : Fl_Gl_Window(x,y,w,h,l)
{
	m_nWindowWidth	= w;
	m_nWindowHeight	= h;

}


void PaintView::draw()
{
	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_FRONT_AND_BACK);
	#endif // !MESA

	if(!valid()) // If valid is false/ Avoid unnecessarily initializing the OpenGL context
	{

		glClearColor(0.7f, 0.7f, 0.7f, 1.0); // Clear the color buffer

		// We're only using 2-D, so turn off depth 
		glDisable( GL_DEPTH_TEST );

		ortho();

		glClear( GL_COLOR_BUFFER_BIT );
	}

	Point scrollpos;// = GetScrollPosition();
	scrollpos.x = 0;
	scrollpos.y	= 0;


	m_nWindowWidth	= w(); // The whole width of the window
	m_nWindowHeight	= h(); // The whole height of the window

	int drawWidth, drawHeight;
	drawWidth = min( m_nWindowWidth, m_pDoc->m_nPaintWidth );
	drawHeight = min( m_nWindowHeight, m_pDoc->m_nPaintHeight );

	int startrow = m_pDoc->m_nPaintHeight - (scrollpos.y + drawHeight);
	if ( startrow < 0 ) startrow = 0;

	m_pPaintBitstart = m_pDoc->m_ucPainting + 3 * ((m_pDoc->m_nPaintWidth * startrow) + scrollpos.x);

	m_nDrawWidth	= drawWidth;
	m_nDrawHeight	= drawHeight;

	m_nStartRow		= startrow;
	m_nEndRow		= startrow + drawHeight;
	m_nStartCol		= scrollpos.x; // 0
	m_nEndCol		= m_nStartCol + drawWidth;
	



	if (m_pDoc->autoFill) {
		//redraw();
		
		autoPaint();
		SaveCurrentContent();
		refresh();
	}

	if ( m_pDoc->m_ucPainting && !isAnEvent )
	{
		RestoreContent();
	}


	if (m_pDoc->m_brushType == 6 && blur && m_pDoc->m_ucPainting) {
		applyBlurSharpen();
		blur = false;
	}else if (m_pDoc->m_pUI->getLevel() != currentBlurSharpenLevel) {
		applyBlurSharpen();
		blur = false;
	}
	
	if ( m_pDoc->m_ucPainting && isAnEvent)
	{
		// Clear it after processing.
		isAnEvent	= 0;	

		Point source( coord.x + m_nStartCol, m_nEndRow - coord.y );
		Point target( coord.x, m_nWindowHeight - coord.y );
		
		// This is the event handler
		switch (eventToDo) 
		{
		case LEFT_MOUSE_DOWN:
			// Save Current Content before doing anything on paintview
			savePreviousContent();
			if ((m_pDoc->m_brushType == 1 || m_pDoc->m_brushType == 4)) { // If the brush type is line or scattered line
				if (m_pDoc->m_direction == BRUSH_DIRECTION) { // If the 'brush direction' is selected
					start.x = target.x;
					start.y = target.y;
				}else if (m_pDoc->m_direction == GRADIENT) { // If the 'gradient' is selected
					m_pDoc->m_pCurrentBrush->gradient(source);
					m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
				}else if (m_pDoc->m_direction == SLIDER_RIGHTMOUSE) {
					m_pDoc->m_pCurrentBrush->BrushBegin(source, target);
				}
			} else {
				m_pDoc->m_pCurrentBrush->BrushBegin( source, target );
			}
			SaveCurrentContent();
			RestoreContent();
			break;
		case LEFT_MOUSE_DRAG:
			if ((m_pDoc->m_brushType == 1 || m_pDoc->m_brushType == 4)){
				if (m_pDoc->m_direction == BRUSH_DIRECTION) {
					m_pDoc->m_pCurrentBrush->cursorMovement(target, start);
				}
				else if (m_pDoc->m_direction == GRADIENT) {
					m_pDoc->m_pCurrentBrush->gradient(source);
				}
			}
			m_pDoc->m_pCurrentBrush->BrushMove( source, target );
			start = target;
			SaveCurrentContent();
			RestoreContent();
			break;

		case LEFT_MOUSE_UP:
			start.x = 0;
			start.y = 0;
			m_pDoc->m_pCurrentBrush->BrushEnd( source, target );
			SaveCurrentContent();
			RestoreContent();
			break;
		
		// Draw Line by dragging mouse while Right Mouse is being clicked
		case RIGHT_MOUSE_DOWN:
			if ( (m_pDoc->m_brushType == 1 || m_pDoc->m_brushType == 4) 
				&& m_pDoc->m_direction == SLIDER_RIGHTMOUSE ) 
			{
				m_pDoc->m_pCurrentBrush->startLine(target);
			}
			break;

		case RIGHT_MOUSE_DRAG:	
			RestoreContent();
			if ((m_pDoc->m_brushType == 1 || m_pDoc->m_brushType == 4) 
				&& m_pDoc->m_direction == SLIDER_RIGHTMOUSE)
			{
				m_pDoc->m_pCurrentBrush->drawLine(target);
			}
			break;
		
		case RIGHT_MOUSE_UP:
			RestoreContent();
			if ((m_pDoc->m_brushType == 1 || m_pDoc->m_brushType == 4) 
				&& m_pDoc->m_direction == SLIDER_RIGHTMOUSE)
			{
				m_pDoc->m_pCurrentBrush->finishLine(target);
			}
			break;

		default:
			printf("Unknown event!!\n");		
			break;
		}
	}

	glFlush();

	#ifndef MESA
	// To avoid flicker on some machines.
	glDrawBuffer(GL_BACK);
	#endif // !MESA

}

int PaintView::handle(int event)
{
	switch(event)
	{
	case FL_ENTER:
	    redraw();
		break;
	case FL_PUSH:
		coord.x = Fl::event_x(); // The location of Mouse
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo=RIGHT_MOUSE_DOWN;		
		else
			eventToDo=LEFT_MOUSE_DOWN;
		isAnEvent=1;
		redraw();
		break;
	case FL_DRAG:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button() > 1)
			eventToDo=RIGHT_MOUSE_DRAG;
		else
			eventToDo=LEFT_MOUSE_DRAG;
		isAnEvent=1;
		// Get location of cursor from Paint view
		m_pDoc->m_pUI->m_origView->getCursor(coord.x, coord.y);
		redraw();
		break;
	case FL_RELEASE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		if (Fl::event_button()>1)
			eventToDo=RIGHT_MOUSE_UP;
		else
			eventToDo=LEFT_MOUSE_UP;
		isAnEvent=1;
		redraw();
		break;
	case FL_MOVE:
		coord.x = Fl::event_x();
		coord.y = Fl::event_y();
		// Get location of cursor from Paint view
		m_pDoc->m_pUI->m_origView->getCursor(coord.x, coord.y); 
		break;
	default:
		return 0;
		break;

	}

	return 1;
}

void PaintView::refresh()
{
	redraw();
}

void PaintView::resizeWindow(int width, int height)
{
	resize(x(), y(), width, height);
}

void PaintView::SaveCurrentContent()
{
	// Tell openGL to read from the front buffer when capturing
	// out paint strokes
	glReadBuffer(GL_FRONT);

	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_PACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	
	glReadPixels( 0, 
				  m_nWindowHeight - m_nDrawHeight, 
				  m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart );
}

void PaintView::RestoreContent()
{
	glDrawBuffer(GL_BACK);

	glClear( GL_COLOR_BUFFER_BIT );

	glRasterPos2i( 0, m_nWindowHeight - m_nDrawHeight );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, m_pDoc->m_nPaintWidth );
	glDrawPixels( m_nDrawWidth, 
				  m_nDrawHeight, 
				  GL_RGB, 
				  GL_UNSIGNED_BYTE, 
				  m_pPaintBitstart);

//	glDrawBuffer(GL_FRONT);
}

void PaintView::autoPaint()
{
	m_pDoc->autoFill = false;

	int size = m_pDoc->getSize();
	
	std::vector< Point > randomBox;

	for (int i = 0; i < m_nDrawHeight; i += 1) {
		for (int j = 0; j < m_pDoc->m_nPaintWidth; j += 1) {
			randomBox.push_back(Point(j, i));
		}
	}

	std::random_shuffle(randomBox.begin(), randomBox.end());	


	for each (Point point in randomBox)
	{
		Point target;
		target.x = point.x;
		target.y = point.y + (h() - m_nDrawHeight); 
		m_pDoc->m_pCurrentBrush->BrushBegin(point, target);
		m_pDoc->m_pCurrentBrush->BrushMove(point, target);
	}
}

// Apply blur filter(mean filter) or Sharpen filter
void PaintView::applyBlurSharpen()
{	
	int type = m_pDoc->getBlurSharpenType();
	currentBlurSharpenLevel = m_pDoc->m_pUI->getLevel();
	int level = 2 * currentBlurSharpenLevel + 1;

	
	if (m_pDoc->m_ucBlurMap) {
		delete[] m_pDoc->m_ucBlurMap;
		m_pDoc->getBlur = false;
	}
	m_pDoc->m_ucBlurMap = new unsigned char[m_pDoc->m_nHeight * m_pDoc->m_nWidth * 3];


	GLubyte original[3];
	
	for (int i = 0; i < m_pDoc->m_nHeight; i++) {
		for (int j = 0; j < m_pDoc->m_nWidth; j++) {
			Point point = Point(j, i);

			GLubyte meanColor[3];
			
			if (type == 0) { // If blur is chosen
				memcpy(meanColor, applyBlurAtPixel(point, level), 3);
				m_pDoc->m_ucBlurMap[3 * (i * m_pDoc->m_nWidth + j)] = meanColor[0];
				m_pDoc->m_ucBlurMap[3 * (i * m_pDoc->m_nWidth + j) + 1] = meanColor[1];
				m_pDoc->m_ucBlurMap[3 * (i * m_pDoc->m_nWidth + j) + 2] = meanColor[2];
			}else { // If sharp is chosen
				memcpy(original, m_pDoc->GetOriginalPixel(point), 3);
				color = applySharpenAtPixel(point, level);

				m_pDoc->m_ucBlurMap[3 * (i * m_pDoc->m_nWidth + j)] = max(0, min(original[0] + color[0], 255));
				m_pDoc->m_ucBlurMap[3 * (i * m_pDoc->m_nWidth + j) + 1] = max(0, min(original[1] + color[1], 255));
				m_pDoc->m_ucBlurMap[3 * (i * m_pDoc->m_nWidth + j) + 2] = max(0, min(original[2] + color[2], 255));
			}
		}
	}

	m_pDoc->getBlur = true;
}

GLubyte* PaintView::applyBlurAtPixel(Point source, int level)
{	
	Point point;
	int temp = level * level;
	int num = 0;
	int type = m_pDoc->getBlurSharpenType();

	GLubyte* sumRed = new GLubyte[temp];
	GLubyte* sumGreen = new GLubyte[temp];
	GLubyte* sumBlue = new GLubyte[temp];

	memset(sumRed, 0, temp);
	memset(sumGreen, 0, temp);
	memset(sumBlue, 0, temp);

	for (int i = 0; i < level; i++) {
		for (int j = 0; j < level; j++) {
			point.x = source.x + i - level / 2;
			point.y = source.y + j - level / 2;

			GLubyte colors[3];
			memcpy(colors, m_pDoc->GetOriginalPixel(point), 3);
			
			sumRed[num] = colors[0];
			sumGreen[num] = colors[1];
			sumBlue[num] = colors[2];
			
			num++;
		}
	}
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;

	for (int i = 0; i < temp; i++) {
		sum1 += sumRed[i];
		sum2 += sumGreen[i];
		sum3 += sumBlue[i];
	}

	result[0] = sum1/temp;
	result[1] = sum2/temp;
	result[2] = sum3/temp;

	return result;
}

double* PaintView::applySharpenAtPixel(Point source, int level)
{
	Point point;
	int temp = level * level;
	int num = 0;
	//double sum[3] = {0, 0, 0};
	
	std::vector<float> kernel;
	std::vector<float> red;
	std::vector<float> blue;
	std::vector<float> green;

	for (int i = 0; i < temp; i++) {
		if (i == temp / 2) {
			kernel.push_back(temp);
		}

		kernel.push_back(-1);
	}

	for (int i = 0; i < level; i++) {
		for (int j = 0; j < level; j++) {
			point.x = source.x + i - level / 2;
			point.y = source.y + j - level / 2;

			GLubyte colors[3];
			memcpy(colors, m_pDoc->GetOriginalPixel(point), 3);
			float r, g, b;
			r = (float)colors[0]*kernel[num];
			g = (float)colors[1]*kernel[num];
			b = (float)colors[2]*kernel[num];
			red.push_back( r);
			green.push_back( g );
			blue.push_back( b );
			//gray[num] = 0.229 * red + 0.578 * green + 0.114 * blue;
			num++;
		}
	}

	double sumRed = 0;
	double sumGreen = 0;
	double sumBlue = 0;
	
	for (int i = 0; i < temp; i++) {
		sumRed += red[i];
		sumGreen += green[i];
		sumBlue += blue[i];
	}

	sharpen[0] = sumRed/temp;
	sharpen[1] = sumGreen/temp;
	sharpen[2] = sumBlue/temp;

	return sharpen;
}

void PaintView::savePreviousContent()
{
	
	memcpy(m_pDoc->m_ucPreviousMap, m_pDoc->m_ucPainting, m_nDrawHeight * m_nDrawWidth * 3);
}