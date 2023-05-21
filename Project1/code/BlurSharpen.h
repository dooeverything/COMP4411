//
// BlurSharpen.h
//
// The header file for Blur or Sharpen Brush. 
//

#ifndef BLURSHARPEN_H
#define BLURSHARPEN_H

#include "ImpBrush.h"

class BlurSharpen : public ImpBrush
{
public:
	BlurSharpen(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	void calculateSum(int x, int y, int level);
	GLubyte* applyBlurAtPixel(Point source, int level);
	void applyBlur();

	void setMeanFilter(int level);

	char* BrushName(void);
	int debug = 0;
	int getMedian(double* array, int size);
private:
	int blur[3];

};

#endif#pragma once
