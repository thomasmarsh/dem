#ifndef DEM_FORMULA_H
#define DEM_FORMULA_H

#include <complex>
#include <cmath>

//-----------------------------------------------------------------------------
//
// 
//
//-----------------------------------------------------------------------------

struct RenderReactor {
	virtual ~RenderReactor() {}
	virtual void result(CalcParam &p, num_t dist, int iter) = 0;
};

//-----------------------------------------------------------------------------
//
// INLINE UTILITIES
//
//-----------------------------------------------------------------------------

static inline
num_t quick_mag(num_t r, num_t i) {
	return r*r + i*i;
}

static inline
num_t quick_mag(std::complex<num_t> &c) {
	return c.real()*c.real() + c.imag()*c.imag();
}

static inline
num_t calc_dist(num_t nz, num_t nzp) {
	num_t a = std::sqrt(nz);
	return 2*a*log(a)/std::sqrt(nzp);
}


//-----------------------------------------------------------------------------
//
// CORE FORMULAS
//
//-----------------------------------------------------------------------------

struct NullBailout {
	static bool bailout(const CalcParam &p) { return false; }
};


struct Mandelbrot {
	static bool bailout(const CalcParam &p) {
		if (p.mode == View::JULIA)
			return false;

		num_t x = p.z.x,
		       y = p.z.y;

		num_t tx = (x - 0.25);

		num_t q = tx*tx + y*y;
		
		if (((q*(q+tx)) < (.25*y*y)) || 
		    ((x+1)*(x+1) + y*y < .0625))
			return true;
		return false;
	}

	static void calc(CalcParam &p, RenderReactor &r) {
		num_t zx = p.z.x,
		       zy = p.z.y,
		       xp = 1,
		       yp = 0,
		       nz, nzp;

		unsigned int i;
		for (i=0; i <= p.maxiter; i++) {
			nz = 2 * (zx*xp - zy*yp);
			yp = 2 * (zx*yp + zy*xp);
			xp = nz;

			nz = zx*zx - zy*zy + p.c.x;
			zy = 2*zx*zy + p.c.y;
			zx = nz;

			nz = quick_mag(zx, zy);
			nzp = quick_mag(xp, yp);

			if (nzp > 1e62 || nz > 4) break;
		}
		r.result(p, calc_dist(nz, nzp), i);
	}
};

struct Mandelbrot3 : NullBailout {
	static void calc(CalcParam &p, RenderReactor &r) {
		num_t nz=0, nzp=0;
		std::complex<num_t> c(p.c.x, p.c.y),
				     z(p.z.x, p.z.y),
				     dz, tmp, c3(3,0);

		unsigned int i;
		for (i=0; i < p.maxiter; i++) {
			tmp = z*z;
			dz = c3 * tmp;
			z = z*tmp + c;
			nz = quick_mag(z);
			nzp = quick_mag(dz);
			if (nzp > 1e60 || nz > 16) break;
		}
		r.result(p, calc_dist(nz, nzp), i);
	}
};

struct Experiment1 : NullBailout {
	static void calc(CalcParam &p, RenderReactor &r) {
		num_t nz=0, nzp=0;
		std::complex<num_t> c(p.c.x, p.c.y),
				     z(p.z.x, p.z.y),
				     dz, tmp;
		unsigned int i;
		for (i=0; i < p.maxiter; i++) {
			tmp = pow(z, c);
			dz = c * tmp / z;
			z = tmp + c;

			nz = quick_mag(z);
			nzp = quick_mag(dz);
			if (nzp > 1e60 || nz > 1e60) break;
		}
		r.result(p, calc_dist(nz, nzp), i);
	}
};

struct Collatz : NullBailout {
	static void calc(CalcParam &p, RenderReactor &r) {
		num_t nz=0, nzp=0;
		std::complex<num_t> c(p.c.x, p.c.y),
				     z(p.z.x, p.z.y),
				     dz;
		const std::complex<num_t> n0_25(0.25,0),
					  nM_PI(M_PI,0),
					  n2(2,0),
				 	  n1(1,0),
					  n4(4,0);
		unsigned int i;
		for (i=0; i < p.maxiter; i++) {
			dz = n0_25 * nM_PI * (n2*z+n1) * sin(nM_PI * z) - n0_25 *cos(nM_PI*z)+n1;
			z = n0_25 * (n1+n4*z-(n1+n2*z)*cos(nM_PI*z));

			nz = quick_mag(z);
			nzp = quick_mag(dz);
			if (nzp > 1e60 || nz > 1e60) break;
		}
		r.result(p, calc_dist(nz, nzp), i);
	}
};

struct Multibrot : NullBailout {
	static void calc(CalcParam &p, RenderReactor &r) {
		num_t nz=0, nzp=0;
		std::complex<num_t> d(p.oc.x, p.oc.y),
				     c(p.c.x, p.c.y),
				     z(p.z.x, p.z.y),
				     dz, tmp;

		unsigned int i;
		for (i=0; i < p.maxiter; i++) {
			tmp = pow(z,d);
			dz = d*tmp/z;
			z = tmp+c;

			nz = quick_mag(z);
			nzp = quick_mag(dz);
			if (nzp > 1e60 || nz > 4) break;
		}
		r.result(p, calc_dist(nz, nzp), i);
	}
};

struct Newton : NullBailout {
	static void calc(CalcParam &p, RenderReactor &r) {
		num_t nz;
		std::complex<num_t> d(p.oc.x, p.oc.y),
				     c(p.c.x, p.c.y),
				     z(p.z.x, p.z.y),
				     tmp;
		const std::complex<num_t> n1(1,0),
					  n2(2,0),
					  n3(3,0);
		unsigned int i;
		for (i=0; i < p.maxiter; i++) {
			tmp = pow(z,n3);
			z -= (tmp-n1)/(n3*pow(z,n2));

			nz = quick_mag(z);
			if (nz > 1e60) break;
		}
		r.result(p, i < p.maxiter? 1e60: 0, i);
	}
};

struct Newton2 : NullBailout {
	static void calc(CalcParam &p, RenderReactor &r) {
		num_t nz;
		std::complex<num_t> d(p.oc.x, p.oc.y),
				     c(p.c.x, p.c.y),
				     z(p.z.x, p.z.y),
				     dz, pz8, pz4, pz3;
		const std::complex<num_t> n7(7,0),
				 	  n8(8,0),
					  n15(15,0),
					  n16(16,0),
					  n60(60,0);
		unsigned int i;
		for (i=0; i < p.maxiter; i++) {
			pz8 = pow(z, 8.0),
			pz4 = pow(z, 4.0),
			pz3 = pow(z, 3.0);
//			dz = ((56.0 * pow(z,6.0) - 180.0 * pow(z, 2.0)) * (pz8 + 15.0 * pz4 - 16.0)) / pow((8.0 * pow(z, 7.0) + 60.0 * pz3), 2.0) + 1.0;
			z -= (pz8+n15*pz4-n16) / (n8*pow(z,n7)+n60*pz3);

			nz = quick_mag(z);
			if (nz > 1e60) break;
//			nzp = quick_mag(dz);
//			if (nzp > 1e60 || nz > 1e60) break;
		}
		r.result(p, i <p.maxiter ? 1e60: 0/*calc_dist(nz, nzp)*/, i);
	}
};

struct Newton3 : NullBailout {
	static void calc(CalcParam &p, RenderReactor &r) {
		num_t nz;

		std::complex<num_t> d(p.oc.x, p.oc.y),
				     c(p.c.x, p.c.y),
				     z(p.z.x, p.z.y),
				     n(4.0,3.0);

		const std::complex<num_t> n2_1(2.1,0),
				 	  n1(1,0);

		unsigned int i;
		for (i=0; i < p.maxiter; i++) {
			z = z-(n2_1*z*pow(z,-n)*(pow(z,n)-n1))/n;
			nz = quick_mag(z);
			if (nz > 1e6) break;
		}
		r.result(p, (i < p.maxiter) ? 1e6 : 0, i);
	}
};

#endif // DEM_FORMULA_H
