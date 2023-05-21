//
// LineBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "LineBrush.h"

#include "math.h"
extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize(); // Get the Brush Size 

	glPointSize((float)size); // Set the Brush Size on the Paint

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) { // If the document is empty, user did not load the bitmap
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	double radian = (dlg->getAngle()*M_PI)/180; // Convert degree to radian
	
	int width = dlg->getWidth();
	float alpha = dlg->getAlpha();

	// OpenGL starts 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(width);

	glBegin(GL_LINES);
	SetColor(source, alpha);

	double x = ((double)size / 2)*cos(radian);
	double y = ((double)size / 2)*sin(radian);

	glVertex2d(target.x - x , target.y - y); // Half left side
	glVertex2d(target.x + x , target.y + y); // Half right side

	glEnd();
}

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

