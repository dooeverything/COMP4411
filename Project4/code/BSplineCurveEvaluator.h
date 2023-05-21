#ifndef INCLUDED_BSPLINE_CURVE_EVALUATOR_H
#define INCLUDED_BSPLINE_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"
#include "mat.h"
#include "vec.h"
class BSplineCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;

	bool flatEnough(Point v0, Point v1, Point v2, Point v3) const;

	void calculateQ(Point b0, Point b1, Point b2, Point b3, Mat4d& basis ,std::vector<Point>& ptvEvaluatedCurvePts) const;
};

#endif
