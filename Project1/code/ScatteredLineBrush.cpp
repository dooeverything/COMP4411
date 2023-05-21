//
// ScatteredLineBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLineBrush.h"
#include "math.h"

extern float frand();

ScatteredLineBrush::ScatteredLineBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void ScatteredLineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize(); // Get the Brush Size 

	glPointSize((float)size); // Set the Brush Size on the Paint

	BrushMove(source, target);
}

void ScatteredLineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) { // If the document is empty, user did not load the bitmap
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	int size = pDoc->getSize();
	int width = dlg->getWidth();
	double angle = (dlg->getAngle() * M_PI) / 180; // Convert degree to radius
	float alpha = dlg->getAlpha();

	// OpenGL starts 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(width);
	glBegin(GL_LINES);

	for (int i = 0; i < (rand()%4)+2; i++) // Generate random number between 2 and 5
	{
		// Generate random number between -size/2 and size/2
		double randomX = rand() % (size + 1) - (double)size/2;
		// Generate random number between -size/2 and size/2
		double randomY = rand() % (size + 1) - (double)size/2;

		Point point;
		point.x = ((double)size / 2) * cos(angle);
		point.y = ((double)size / 2) * sin(angle);

		Point center;
		center.x = target.x + randomX;
		center.y = target.y + randomY;

		SetColor(center, alpha); // Get color from original image and set color 
		
		// Half left side
		glVertex2d( (double)center.x - point.x, (double)center.y - point.y);
		// Half right side
		glVertex2d( (double)center.x + point.x, (double)center.y + point.y);

	}

	glEnd();
}

void ScatteredLineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

