//
// CircleBrush.cpp
//
// The implementation of Circle Brush. 
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"
#include <math.h>

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize(); // Get the Brush Size 

	glPointSize((float)size); // Set the Brush Size on the Paint

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) { // If the document is empty, user did not load the bitmap
		printf("CircleBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize(); // Radius
	double radius = size / 2;
	float alpha = dlg->getAlpha();

	// OpenGL starts
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLE_FAN);
	SetColor(source, alpha); // From ImpBrush.cpp 

	// Draw a circle with Triangles
	for (int i = 0; i <= 360; i++) 
	{	
		double angle = 2.0f * M_PI / 360; // Angle
		double x = target.x +  radius * cos( angle * i  );
		double y = target.y +  radius * sin( angle * i );
		glVertex2d(x, y); 
	}

	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

