#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal(const ray& r, isect& i) const
{
	// YOUR CODE HERE:
	// Add box intersection code here.
	// it currently ignores all boxes and just returns false.

	BoundingBox box = ComputeLocalBoundingBox();
	double tMax;
	double tMin;

	if (box.intersect(r, tMin, tMax)) {
		// If the ray intersect with bounding box
		if (tMin < RAY_EPSILON || tMax < RAY_EPSILON || (tMax-tMin) < RAY_EPSILON) {
			return false;
		}

		i.t = tMin;

		vec3f Q = r.at(tMin);
		vec3f max = box.max;
		vec3f min = box.min;
		
		if (abs(Q[0] - max[0]) < RAY_EPSILON) {
			i.N = vec3f(1, 0, 0);
		}
		else if (abs(Q[0] - min[0]) < RAY_EPSILON) {
			i.N = vec3f(-1, 0, 0);
		}
		else if (abs(Q[1] - max[1]) < RAY_EPSILON) {
			i.N = vec3f(0, 1, 0);
		}
		else if (abs(Q[1] - min[1]) < RAY_EPSILON) {
			i.N = vec3f(0, -1, 0);
		}
		else if (abs(Q[2] - max[2]) < RAY_EPSILON) {
			i.N = vec3f(0, 0, 1);
		}
		else if (abs(Q[2] - min[2]) < RAY_EPSILON) {
			i.N = vec3f(0, 0, -1);
		}
		else {
			i.N = vec3f(0, 0, 0);
		}

		i.obj = this;
	
		return true;
	}


	return false;
}
