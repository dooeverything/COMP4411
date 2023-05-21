#pragma once
#ifndef INCLUDED_C2_INTERPOLATING_H
#define INCLUDED_C2_INTERPOLATING_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"
#include "BezierCurveEvaluator.h"
#include "mat.h"
#include "vec.h"
//using namespace std;

class C2Interpolating : public BezierCurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
};

#endif