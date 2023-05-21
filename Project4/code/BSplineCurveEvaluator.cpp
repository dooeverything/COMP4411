#include <assert.h>
#include "BSplineCurveEvaluator.h"
using namespace std;

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
										  std::vector<Point>& ptvEvaluatedCurvePts, 
										  const float& fAniLength, const bool& bWrap) const
{


	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();
	//cout << "HIHI" << endl;
	Mat4d b = {-1, 3, -3, 1,
				3, -6, 3, 0,
				-3, 3, 0, 0,
				1, 0, 0, 0};

	Mat4d mb = {1, 4, 1, 0,
				0, 4, 2, 0,
				0, 2, 4, 0,
				0, 1, 4, 1 };

	Point first;
	Point last;

	if (bWrap) {
		first = Point(0- ptvCtrlPts[0].x, ptvCtrlPts[0].y-0.01);
		last = Point(fAniLength+ ptvCtrlPts[iCtrlPtCount - 1].x, ptvCtrlPts[iCtrlPtCount - 1].y-0.01);
	}
	else {
		first = Point(ptvCtrlPts[0].x-0.01, ptvCtrlPts[0].y - 0.01);
		last = Point(ptvCtrlPts[iCtrlPtCount - 1].x-0.01, ptvCtrlPts[iCtrlPtCount - 1].y - 0.01);
	}
	vector<Point> pts;
	for (int i = 0; i < 2; i++) {
		pts.push_back(first );
	}
	pts.insert(pts.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());
	
	for (int i = 0; i < 2; i++) {
		pts.push_back(last);
	}


	Mat4d basis = 0.166667 * b * mb;
	std::vector<Point>::const_iterator it;
	for (it = pts.begin(); it != pts.end() - 3; it++) {
		
		/*if ((it+3) == ptvCtrlPts.end()) {
			cout << "hehe" << endl;
			break;
		}*/
		Point b0 = *it;
		Point b1 = *(it + 1);
		Point b2 = *(it + 2);
		Point b3 = *(it + 3);

		if (flatEnough(b0, b1, b2, b3)) {
			continue;
		}
		// Convert B_0, B_1, B_2, B_3 to V_0, V_1, V_2, V_3
		// Calculate Q(t) for each t
		calculateQ(b0, b1, b2, b3, basis, ptvEvaluatedCurvePts);
	}

	if (!bWrap) {
		ptvEvaluatedCurvePts.push_back( Point(0, ptvCtrlPts[0].y) );
		ptvEvaluatedCurvePts.push_back( Point(fAniLength, ptvCtrlPts[iCtrlPtCount-1].y) );
	}
}




bool BSplineCurveEvaluator::flatEnough(Point v0, Point v1, Point v2, Point v3) const
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

void BSplineCurveEvaluator::calculateQ(Point b0, Point b1, Point b2, Point b3, Mat4d& basis, std::vector<Point>& ptvEvaluatedCurvePts) const
{
	int temp = 0;
	for (double i = 0; i < 40; i++) {
		//cout << ++temp << endl;
		double t = i / 40;

		/*if (i < 4) {
			t = 0.0;
		}
		else if (i > 15) {
			t = 1.0;
		}*/
		
		Vec4d vX = { b0.x,b1.x,b2.x,b3.x };
		Vec4d vY = { b0.y,b1.y,b2.y,b3.y };

		//Vec4d knots = { pow(t, 3), pow(t, 2), pow(t, 1), 1 };
		////Vec4f temp = basis[0];
		//
		//double x = knots * (basis * vX);
		//double y = knots * (basis * vY);
		
		double tX_3 = pow(t, 3) * (basis[0][0] * vX[0] + basis[0][1] * vX[1] + basis[0][2] * vX[2] + basis[0][3] * vX[3]);
		double tX_2 = pow(t, 2) * (basis[1][0] * vX[0] + basis[1][1] * vX[1] + basis[1][2] * vX[2] + basis[1][3] * vX[3]);
		double tX_1 = t * (basis[2][0] * vX[0] + basis[2][1] * vX[1] + basis[2][2] * vX[2] + basis[2][3] * vX[3]);
		double tX_0 = basis[3][0] * vX[0] + basis[3][1] * vX[1] + basis[3][2] * vX[2] + basis[3][3] * vX[3];

		double tY_3 = pow(t, 3) * (basis[0][0] * vY[0] + basis[0][1] * vY[1] + basis[0][2] * vY[2] + basis[0][3] * vY[3]);
		double tY_2 = pow(t, 2) * (basis[1][0] * vY[0] + basis[1][1] * vY[1] + basis[1][2] * vY[2] + basis[1][3] * vY[3]);
		double tY_1 = t * (basis[2][0] * vY[0] + basis[2][1] * vY[1] + basis[2][2] * vY[2] + basis[2][3] * vY[3]);
		double tY_0 = basis[3][0] * vY[0] + basis[3][1] * vY[1] + basis[3][2] * vY[2] + basis[3][3] * vY[3];

		Point q = Point(tX_3 + tX_2 + tX_1 + tX_0, tY_3 + tY_2 + tY_1 + tY_0);
		ptvEvaluatedCurvePts.push_back(q);

	}

	//cout << temp << endl;

}
