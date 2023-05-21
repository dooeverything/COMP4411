//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"
#include "math.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source, const float alpha)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* pUI = pDoc->m_pUI;

	Fl_Color_Chooser* fcc = pUI->m_colorChooser;

	double red = fcc->r();
	double green = fcc->g();
	double blue = fcc->b();
	int mode = fcc->mode();

	if (mode == 1) 
	{
		// If the current mode is byte
		// Need to convert to rgb value
		red = red / 255;
		green = green / 255;
		blue = blue / 255;
	} else if (mode == 3) {
		// If the current mode is hsv
		// Need to convert to rgb value
		fcc->hsv2rgb(fcc->r(), fcc->g(), fcc->b(), red, green, blue);
	}

	GLubyte color[4];
	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 ); 
	// GetOriginalPixel - Returns the color of the pixel from the original paint (source)
	
	color[0] = color[0] * red;
	color[1] = color[1] * green;
	color[2] = color[2] * blue;
	
	color[3] = (GLubyte)(255.0*alpha); // convert '0.00-1.00' to '0-255' and assign as alpha value
	glColor4ubv( color ); // set color and alpha value

}

// Function for right mouse down
void ImpBrush::startLine(Point source)
{
	start = source;
	glColor3f(1.0, 0, 0);
	glPointSize(1.0);
}

// Function for right mouse drag
void ImpBrush::drawLine(Point source) 
{
	// Draw Red Line by dragging
	drag.x = source.x;
	drag.y = source.y;
	
	// openGL starts
	glBegin(GL_LINES);
	glVertex2d(start.x, start.y);
	glVertex2d(drag.x, drag.y);
	glEnd();
}

// Function for right mouse up - calculating length and angle of line
void ImpBrush::finishLine(Point source)
{
	finish = source;

	double x = (double)finish.x - start.x;
	double y = (double)finish.y - start.y;

	length = sqrt( x*x + y*y);

	//int temp = finish.x - start.x;
	double adj = (double)finish.x - start.x; //abs(temp);

	radian = acos(adj / length);
	double angle = radian * (180 / M_PI);

	if (x < 0 && y < 0)
	{
		angle += 90;
	}
	else if (x > 0 && y < 0) 
	{
		angle = 360 - angle;
	}

	// Get the length of the red line
	m_pDoc->m_pUI->setSize(length);
	m_pDoc->m_pUI->m_BrushSizeSlider->value(length);
	m_pDoc->m_pUI->setAngle(angle);
	//m_pDoc->m_pUI->m_AngleSizeSlider->value(angle);
}

// Function to calculate angle of line brush by cursor movement
void ImpBrush::cursorMovement(Point target, Point start) 
{
	// Calculate adjacent
	double x = (double)target.x - start.x;
	// Calculate opposite
	double y = (double)target.y - start.y;
	// Calculate Angle
	double radian = atan(y / x);
	double angle = radian * (180 / M_PI);

	if (x == 0) {
		angle = 90;
		if (y == 0) {
			angle = 0;
		}
	}
	else if (x < 0 && y > 0) {
		angle = -angle;
		angle += 90;
	}
	else if (x < 0 && y < 0) {
		angle += 180;
	}
	else if (x > 0 && y < 0) {
		angle = -angle;
		angle += 270;
	}
	m_pDoc->m_pUI->setAngle(angle);
}

// Function to calculate the gradient and angle that is perpendicular to the gradient
void ImpBrush::gradient(Point source)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			temp.x = source.x + i - 1;
			temp.y = source.y + j - 1;
			
			// Get the original color from temp
			memcpy(colors, m_pDoc->GetOriginalPixel(temp), 3);
			red = (int)colors[0];
			green = (int)colors[1];
			blue = (int)colors[2];
			color = 0.229*red + 0.578*green + 0.114*blue; // convert to grey-level image 


			// Calculate gradient along x
			//grdX += sobelX[i][j] * color;
			gradientX[i][j] = sobelX[i][j] * color;

			// Calculate gradient along y
			//grdY += sobelY[i][j] * color;
			gradientY[i][j] = sobelY[i][j] * color;
		

		}

		// Calculate a radian
		
	}

	double sum = 0;
	double sum2 = 0;
	for (int i = 0; i < 3; i++) {
		//grdY += testing2[0][i];
		for (int j = 0; j < 3; j++) {
			sum += gradientX[i][j];
			sum2 += gradientY[i][j];
		}
	}

	grdX = sum;
	grdY = sum2;

	double deg;

	if (grdX == 0) {
		deg = 90;
	} else {
		rad = atan(grdY / grdX);
		// Calculate a degree that is perpendicular to the gradient of image
		deg = rad * (180 / M_PI);
	}

	if (deg < 0) {
		deg += 180;
	}

	m_pDoc->m_pUI->setAngle(deg);
}

// Function to check whether the cursor is out of paintview or original view
bool ImpBrush::borderClip(Point source)
{
	if (source.x > m_pDoc->m_nWidth || source.y < 0)
	{
		return true;
	}

	return false;
}
