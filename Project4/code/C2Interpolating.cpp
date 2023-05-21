#include <assert.h>
#include "C2Interpolating.h"
using namespace std;

void C2Interpolating::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
									std::vector<Point>& ptvEvaluatedCurvePts, 
									const float& fAniLength, const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();

	vector<float> rs; // Container for storing matrix during forward elimination
	vector<Point> ps; // Container for storing points during forward elimination
	vector<Point> ds; // Derivates
	
	float y1;
	float y2;
	vector<Point> pts;

	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts.back().x > 0.0f) {
			y2 = y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts.back().x) +
				ptvCtrlPts.back().y * ptvCtrlPts[0].x) /
				(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts.back().x);
		}
		else {
			y2 = y1 = ptvCtrlPts[0].y;
		}

		pts.push_back(Point(0, y1));

		pts.insert(pts.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());

		pts.push_back(Point(fAniLength, y2));
	}
	else {
		pts.insert(pts.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());
		
		y1 = ptvCtrlPts.front().y;
		y2 = ptvCtrlPts.back().y;
		ptvEvaluatedCurvePts.push_back(Point(0, y1));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));

	}

	int iCtrlPtCount = pts.size();

	if (iCtrlPtCount <= 2) {
		return;
	}

	//cout << "The last point is " << ptvCtrlPts[2] << endl;

	Point first = *(pts.begin());
	Point second = *(pts.begin()+1);
	
	float r0 = 0.5;
	rs.push_back(r0);

	float p0_X = 3.0 * ( (double)second.x - first.x) * r0;
	float p0_Y = 3.0 * ( (double)second.y - first.y) * r0;
	
	Point p0 = Point(p0_X, p0_Y);
	
	//cout << p0 << endl;

	ps.push_back(p0);

	double d = 0.0;
	Point old_p;
	

	for (int i = 1; i < iCtrlPtCount-1; i++) {
		float old_r = rs[i - 1];
		float r = 1.0 / (4.0 - old_r);
		rs.push_back(r);

		Point c0 = pts[i - 1];
		Point c2 = pts[i + 1];

		//cout << "c0: " << c0 << endl;
		//cout << "c2: " << c2 << endl;

		float p_x = (3 * (c2.x - c0.x) - ps[i - 1].x) * r;
		float p_y = (3 * (c2.y - c0.y) - ps[i - 1].y) * r;

		Point p = Point(p_x, p_y);
		ps.push_back(p);
	}
	
	float rM = 1.0 / (2.0 - rs.back());
	rs.push_back(rM);

	Point second_last = *(pts.end() - 2);
	Point last = *(pts.end() - 1);
	
	float pM_X = (3 * (last.x - second_last.x) - ps.back().x) * rM;
	float pM_Y = (3 * (last.y - second_last.y) - ps.back().y) * rM;

	Point pM = Point(pM_X, pM_Y);
	ps.push_back(pM);

	ds.resize(iCtrlPtCount);

	ds[iCtrlPtCount - 1] = ps[iCtrlPtCount - 1];
	
	for (int i = ds.size()-2; i >= 0; i-=1) {
		ds[i].x = ps[i].x - rs[i] * ds[i + 1].x;
		ds[i].y = ps[i].y - rs[i] * ds[i + 1].y;
		
		//cout << ds[i] << " at " << i << endl;
	}

	for (int i = 0; i < ds.size(); i++) {
	}

	for (int i = 0; i < iCtrlPtCount-1; i++) {
		
		Point v0 = pts[i];
		Point v3 = pts[i + 1];

		float v1_X = v0.x + (ds[i].x / 3.0);
		float v1_Y = v0.y + (ds[i].y / 3.0);
		Point v1 = Point(v1_X, v1_Y);

		float v2_X = v3.x - (ds[i+1].x / 3.0);
		float v2_Y = v3.y - (ds[i+1].y / 3.0);
		Point v2 = Point(v2_X, v2_Y);

		//cout << v0 << v1 << v2 << v3 << endl;

		displayBezierCurve(v0, v1, v2, v3, ptvEvaluatedCurvePts);
	}
}
