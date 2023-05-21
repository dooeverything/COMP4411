#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"
class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;

	bool flatEnough(Point v0, Point v1, Point v2, Point v3) const;

	virtual void displayBezierCurve(Point v0, Point v1, Point v2, Point v3, 
									std::vector<Point>& ptvEvaluatedCurvePts) const;

	int nPoints;
};

#endif