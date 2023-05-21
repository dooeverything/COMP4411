#include <assert.h>
#include "CatmullRomCurveEvaluator.h"
using namespace std;

void CatmullromCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
											 std::vector<Point>& ptvEvaluatedCurvePts, 
											 const float& fAniLength, const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();

	Mat4d basis = { -1,  3, -3,  1,
					 2, -5,  4, -1,
					-1,  0,  1,  0,
					 0,  2,  0,  0 };

	//cout << basis << endl;
	float y1;
	float y2;
	vector<Point> pts;
	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
			y2 = y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
				ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
				(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
		}
		else {	
			y2 = y1 = ptvCtrlPts[0].y;
		}
	
		pts.push_back(Point(0, y1));
		pts.push_back(Point(0, y1));

		pts.insert(pts.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());

		pts.push_back(Point(fAniLength, y2));
		pts.push_back(Point(fAniLength, y2));
	
	}
	else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.

		y1 = ptvCtrlPts[0].y;
		y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

		pts.push_back(Point(0, y1));

		pts.insert(pts.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());

		pts.push_back(Point(fAniLength, y2));

		ptvEvaluatedCurvePts.push_back(Point(0, y1));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));
	}


	vector<Point>::const_iterator it;
	for (it = pts.begin(); it != pts.end() - 3; it++) 
	{
		Point c0 = *it;
		Point c1 = *(it + 1);
		Point c2 = *(it + 2);
		Point c3 = *(it + 3);
	

		if (flatEnough(c0, c1, c2, c3)) {
			ptvEvaluatedCurvePts.push_back(c0);
			ptvEvaluatedCurvePts.push_back(c1);
			ptvEvaluatedCurvePts.push_back(c2);
			ptvEvaluatedCurvePts.push_back(c3);
			continue;
		}

		// Convert B_0, B_1, B_2, B_3 to V_0, V_1, V_2, V_3
		// The, calculate Q(t) for each t
		for (double t = 0.0; t <= 1; t+=0.01) {
			//double t = i;/// 40;
			Vec4d x = { c0.x, c1.x, c2.x, c3.x };
			Vec4d y = { c0.y, c1.y, c2.y, c3.y };

			Vec4d ts = { pow(t,3), pow(t,2), t, 1 };

			double Qx = 0.5 * ts * basis * x;
			double Qy = 0.5 * ts * basis * y;

			Point Q = Point(Qx, Qy);

			ptvEvaluatedCurvePts.push_back(Q);
		}
	}
}

bool CatmullromCurveEvaluator::flatEnough(Point v0, Point v1, Point v2, Point v3) const
{
	float d01 = v0.distance(v1); //sqrt(pow(v0.x - v1.x, 2) + pow(v0.y - v1.y, 2));
	float d12 = v1.distance(v2); // sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
	float d23 = v2.distance(v3); // sqrt(pow(v2.x - v3.x, 2) + pow(v2.y - v3.y, 2));

	float d03 = v0.distance(v3); // sqrt(pow(v0.x - v3.x, 2) + pow(v0.y - v3.y, 2));

	float d = (d01 + d12 + d23); /// v03;

	if (abs(d - d03) < 1e-5) {
		//cout << "Flat enough!" << endl;
		//cout << d << endl;
		return true;
	}

	return false;
}

