//
// ScatteredPointBrush.cpp
//


#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointbrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize(); // Get the Brush Size 

	glPointSize((float)1.0); // Set the Brush Size as small as possible

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) { // If the document is empty, user did not load the bitmap
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}


	int size = pDoc->getSize();
	float alpha = dlg->getAlpha();

	// OpenGL 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_POINTS);

	for (int i = 0; i < size*2; i++)
	{
		double randomX = (double)(rand() % (size + 1)) - size / 2;
		double randomY = (double)(rand() % (size + 1)) - size / 2;
		Point point;
		point.x = target.x + randomX;
		point.y = target.y + randomY;
		SetColor(point, alpha); // From ImpBrush.cpp 
		glVertex2d(point.x, point.y);
	}
	glEnd();
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

