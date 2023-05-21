#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	BRUSH_BLUR_OR_SHARPEN,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

enum
{
	SLIDER_RIGHTMOUSE = 0,
	GRADIENT,
	BRUSH_DIRECTION,
	NUM_DIRECTION_TYPE
};

enum
{
	BLUR = 0,
	SHARPEN,
};


class ImpressionistDoc; // Pre-declaring class

class Point 
{
public:
	Point() {};
	Point(int xx, int yy) { x = xx; y = yy; };

	int x, y;
};

class ImpBrush 
{
protected:
	ImpBrush::ImpBrush( ImpressionistDoc* pDoc = NULL, char* name = NULL );

public:

	// Boolean Function for clipping the stroke
	bool borderClip(Point source);

	// Functions for drawing red line when the right mouse button is clicked
	void drawLine(Point source);
	void startLine(Point source);
	void finishLine(Point source);

	// Calculate the angle of line brush, which is determined by direction of mouse cursor
	void cursorMovement(Point target, Point start);

	// Calculate an angle that is perpendicular to the gradient of the image
	void gradient(Point source);

	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin( const Point source, const Point target ) = 0;
	virtual void BrushMove( const Point source, const Point target ) = 0;
	virtual void BrushEnd( const Point source, const Point target ) = 0;

	// according to the source image and the position, determine the draw color
	void SetColor( const Point source, const float alpha );

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument( void );

	// Return the name of the brush (not used in this version).
	char* BrushName( void );
	
	static int			c_nBrushCount;	// How many brushes we have,
	static ImpBrush**	c_pBrushes;		// and what they are.

	Point start;
	Point drag;
	Point finish;

	// Length and Angle set by red line
	double length=0;
	double radian=0;

private:
	ImpressionistDoc*	m_pDoc;

	// Brush's name (not used in this version).
	char*				m_pBrushName;

	double sobelX[3][3] = { {-1, 0, 1},
						    {-2, 0, 2},
							{-1, 0, 1} };

	double sobelY[3][3] = { {1, 2, 1},
							{0, 0, 0},
							{-1,-2,-1} };

	Point temp;

	double grdX = 0;
	double grdY = 0;
	double rad;
	GLubyte colors[3];
	int red;
	int green;
	int blue;
	double color;

	double gradientX[3][3];
	double gradientY[3][3];

};

#endif