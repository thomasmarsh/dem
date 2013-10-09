#ifndef DEM_PLANE_H
#define DEM_PLANE_H

#include "common.h"

// complex math helpers

#define c_sub(ar,ai,br,bi,ox,oy) ((ox)=(ar)-(br),(oy)=(ai)-(bi))

#define c_pow2_rp(ar,ai) ((ar)*(ar)-(ai)*(ai))
#define c_pow2_ip(ar,ai) (2*(ar)*(ai))
#define c_pow2(ar,ai,ox,oy) ((ox)=c_pow2_rp(ar,ai),(oy)=c_pow2_ip(ar,ai))

#define c_div_rp(ar,ai,br,bi) (((ar)*(br)+(ai)*(bi))/((bi)*(bi)+(br)*(br)))
#define c_div_ip(ar,ai,br,bi) ((-(ar)*(bi)+(ai)*(br))/((br)*(br)+(bi)*(bi)))
#define c_div(ar,ai,br,bi,ox,oy) ((ox)=c_div_rp(ar,ai,br,bi), \
                                  (oy)=c_div_ip(ar,ai,br,bi)) 

#define myabs(x) ((x)>0?(x):-(x)) 

#include <limits.h>

static inline
void CorrectPlane(Coord<num_t> &c, View::Plane plane) {
	switch (plane) {
	case View::STANDARD: break;
	case View::INVERSE:
		{ // 1/mu
			num_t t;
			if (myabs(c.x) + myabs(c.y) < 0.000001)
				t = INT_MAX, c.y = INT_MAX;
			else {
				c_div(1, 0, c.x, c.y, t, c.y);
			}
			c.x = t; 
		}
		break;
	case View::INVERSE_OFFSET:
		{ // 1/(mu + 0.25)
			num_t t;
			if (myabs(c.x) + myabs(c.y) < 0.000001)
				t = INT_MAX, c.y = INT_MAX;
			else {
				c_div(1, 0, c.x, c.y, t, c.y);
			}
			c.x = t;
			c.x += 0.25;
		}
		break;
	case View::LAMBDA:
		{ // lambda
			num_t tr, ti, mr, mi;

			mr = c.x, mi = c.y;
			c_pow2(c.x, c.y, tr, ti);
			c_div(tr, ti, 4, 0, c.x, c.y);
			c_div(mr, mi, 2, 0, tr, ti);
			c_sub(tr, ti, c.x, c.y, mr, mi);
			c.x = mr, c.y = mi;
		}
		break;
	case View::INVERSE_LAMBDA:
		{ // 1/lambda
			num_t tr, ti, mr, mi;

			c_div(1, 0, c.x, c.y, tr, c.y);
			c.x = tr;
			mr = c.x, mi = c.y;
			c_pow2(c.x, c.y, tr, ti);
			c_div(tr, ti, 4, 0, c.x, c.y);
			c_div(mr, mi, 2, 0, tr, ti);
			c_sub(tr, ti, c.x, c.y, mr, mi);
			c.x = mr, c.y = mi;
		}
		break;
	case View::INVERSE_LAMBDA_OFFSET:
		{ // 1/(lambda-1)
			num_t tr, ti, mr, mi;

			c_div(1, 0, c.x, c.y, tr, c.y);
			c.x = tr + 1;
			mr = c.x, mi = c.y;
			c_pow2(c.x, c.y, tr, ti);
			c_div(tr, ti, 4, 0, c.x, c.y);
			c_div(mr, mi, 2, 0, tr, ti);
			c_sub(tr, ti, c.x, c.y, mr, mi);
			c.x = mr, c.y = mi;
		}
		break;
	case View::INVERSE_OFFSET2:
		{ // 1/(mu - 1.40115)  [Myreburg point]
			num_t t;
			if (myabs(c.x) + myabs(c.y) < 0.000001)
				t = INT_MAX, c.y = INT_MAX;
			else {
				c_div(1, 0, c.x, c.y, t, c.y);
			}
			c.x = t;
			c.x -= 1.40115;
		}
		break; 
	case View::INVERSE_OFFSET3:
		{ // 1/(mu - 2.0)
			num_t t;
			if (myabs(c.x) + myabs(c.y) < 0.000001)
				t = INT_MAX, c.y = INT_MAX;
			else {
				c_div(1, 0, c.x, c.y, t, c.y);
			}
			c.x = t;
			c.x -= 2.0;
		}
		break; 
	default:
		break;
	}
}

#endif // DEM_PLANE_H
