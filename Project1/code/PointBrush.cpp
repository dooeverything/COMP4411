//
// PointBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "pointbrush.h"

extern float frand();

PointBrush::PointBrush( ImpressionistDoc* pDoc, char* name ) : ImpBrush(pDoc,name)
{
}

void PointBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int size = pDoc->getSize(); // Get the Brush Size 

	glPointSize( (float)size ); // Set the Brush Size on the Paint

	BrushMove( source, target );
}

void PointBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) { // If the document is empty, user did not load the bitmap
		printf( "PointBrush::BrushMove  document is NULL\n" );
		return;
	}

	// If the location of cursor is out of originalview(or paint view), Do nothing
	if (borderClip(source)) 
	{
		return;
	}

	float alpha = dlg->getAlpha();
	
	// To use alpha value
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// OpenGL 
	glBegin( GL_POINTS );

	SetColor( source, alpha ); // From ImpBrush.cpp 
	//GLubyte colors[3] = { 255, 0, 0 };
	//glColor3ubv(colors);
	glVertex2d( target.x, target.y ); 

	glEnd();

	
}

void PointBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

