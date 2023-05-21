//
// BlurSharpen.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "BlurSharpen.h"
#include <vector>
#include <algorithm>    // std::sort

extern float frand();

BlurSharpen::BlurSharpen(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void BlurSharpen::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	int size = pDoc->getSize(); // Get the Brush Size 

	glPointSize(size); // Set the Brush Size on the Paint

	BrushMove(source, target);
}

void BlurSharpen::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	
	debug = pDoc->getBlurSharpenType();
	
	if (pDoc == NULL) { // If the document is empty, user did not load the bitmap
		printf("PointBrush::BrushMove  document is NULL\n");
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
	glBegin(GL_POINTS);
	SetColor(source, alpha);
	glVertex2d(target.x, target.y);

	glEnd();
}

void BlurSharpen::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

/*
void BlurSharpen::applyBlur()
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	
	int type = pDoc->getBlurSharpen();
	int level =2*dlg->getLevel() + 1;

	if (pDoc->m_ucBlurMap) delete[] pDoc->m_ucBlurMap;

	pDoc->m_ucBlurMap = new unsigned char[pDoc->m_nHeight * pDoc->m_nWidth * 3];
	memset(pDoc->m_ucBlurMap, 0, pDoc->m_nHeight * pDoc->m_nWidth * 3);
	

	for (int i = 0; i < pDoc->m_nHeight; i++) {
		for (int j = 0; j < pDoc->m_nWidth; j++) {
			Point point = Point(i, j);

			GLubyte colors[3];

			memcpy(colors, applyBlurAtPixel(point, level), 3);

			pDoc->m_ucBlurMap[ 3*(i * pDoc->m_nWidth + j) ] = colors[0]; //= sum[0];
			pDoc->m_ucBlurMap[ 3*(i * pDoc->m_nWidth + j) + 1 ] = colors[1];
			pDoc->m_ucBlurMap[ 3*(i * pDoc->m_nWidth + j) + 2 ] = colors[2];
		}
	}

	pDoc->blur = true;
}
*/

/*
GLubyte* BlurSharpen::applyBlurAtPixel(Point source, int level) 
{
	ImpressionistDoc* pDoc = GetDocument();
	Point point;

	int temp = level * level;
	//double* gray = new double[temp];
	std::vector< std::pair<double, int> > gray;
	std::vector<GLubyte*> temp2;
	int testing = temp / 2;
	int num = 0;
	int test[3] = {0,0,0};
	for (int i = 0; i < level; i++) {
		for (int j = 0; j < level; j++) {
			point.x = source.x + i - level/2;
			point.y = source.y + j - level/2;
			
			GLubyte colors[3];

			memcpy(colors, pDoc->GetOriginalPixel(point), 3);

			double red = (double)colors[0];
			double green = (double)colors[1];
			double blue = (double)colors[2];

			gray.push_back( std::make_pair(0.229*red + 0.578*green + 0.114*blue, num)) ;
			
			temp2.push_back(colors);
			num++;
		}
	}


	std::sort(gray.begin(), gray.end());

	int medianLocation = temp / 2;

	int location = gray[testing].second;
	debug = location;

	//GLubyte medianColor[3];
	
	//memcpy(medianColor, temp2[location], 3);

	return temp2[location];

}
*/


