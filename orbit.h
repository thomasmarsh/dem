#ifndef DEM_ORBIT_H
#define DEM_ORBIT_H

#include "common.h"
#include <string.h>

const int MAXORB = 64;

struct Orbit {
	Coord<num_t> escape[MAXORB];
	int len;

	inline void calc(num_t zx, num_t zy, num_t cx, num_t cy) {
		memset(&escape, 0, sizeof(Coord<num_t>)*MAXORB);
		num_t x = zx, y = zy;

		len = 0;
		escape[len].x = x;
		escape[len].y = y;
		for (len=1; len < MAXORB; len++) {
			x = x*x - y*y + cx;
			y = 2*x*y + cy;
			escape[len].x = x;
			escape[len].y = y;
			if (x*x + y*y > 4) break;
		}
	}
};

#endif // DEM_ORBIT_H
