//
// ScatteredCircleBrush.cpp
//
// The implementation of Scattered Circle Brush. 
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCircleBrush.h"
#include <math.h>

extern float frand();

ScatteredCircleBrush::ScatteredCircleBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void ScatteredCircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize(); // Get the Brush Size 

	glPointSize((float)size); // Set the Brush Size on the Paint

	BrushMove(source, target);
}

void ScatteredCircleBrush::BrushMove(const Point source, const Point target)
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

	// Generate a number from 2 to 6 for the number of circle to draw
	for (int i = 0; i < rand()%5+2; i++)
	{
		// Generate a random number from -size to size for center point of a circle
		
		Point center;
		center.x = (double)target.x + rand()%(size + 1) - size;
		center.y = (double)target.y + rand()%(size + 1) - size;
		
		SetColor(center, alpha); // From ImpBrush.cpp 
		// Draw a circle with Triangles
		for (int j = 0; j <= 360; j++)
		{
			double angle = 2.0f * M_PI / 360; // Angle
			
			Point point;
			point.x = center.x + radius * cos(angle * j);
			point.y = center.y + radius * sin(angle * j);

			glVertex2d(point.x, point.y);
		}
	}

	glEnd();
}

void ScatteredCircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

