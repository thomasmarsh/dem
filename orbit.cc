#include <string.h>
#include "orbit.h"

void Orbit::calc(double zx, double zy, double cx, double cy) {
	memset(&escape, 0, sizeof(Point)*MAXORB);
	double x = zx, y = zy;

	len = 0;
	escape[len].x = x;
	escape[len].y = y;
	for (len=1; len <= MAXORB; len++) {
		x = x*x - y*y + cx;
		y = 2*x*y + cy;
		escape[len].x = x;
		escape[len].y = y;
		if (x*x + y*y > 4) break;
	}
}
