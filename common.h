#ifndef DEM_COMMON_H
#define DEM_COMMON_H

typedef long double num_t;

template <typename T>
struct Coord {
	T x, y;
};


struct Color {
	num_t r, g, b;;

	void set(num_t _r,
		 num_t _g,
		 num_t _b) { r = _r, g = _g, b = _b; }
};


struct View {
	enum Formula {
		MANDELBROT,
		MANDELBROT3,
		EXPERIMENT1,
		COLLATZ,
		MULTIBROT,
		NEWTON,
		NEWTON2,
		NEWTON3
	 };

	enum Mode { MANDEL, JULIA };

	enum Plane {
		STANDARD, 		// mu
		INVERSE,		// 1/mu
		INVERSE_OFFSET, 	// 1/(mu+0.25)
		LAMBDA,			// lambda
		INVERSE_LAMBDA,		// 1/lambda
		INVERSE_LAMBDA_OFFSET, 	// 1/(lamda-1)
		INVERSE_OFFSET2, 	// 1/(mu-1.40115)
		INVERSE_OFFSET3, 	// 1/(mu-2.0)
		NUM_PLANES
	};

	Formula	      formula;
	Mode          mode;
	Plane	      plane;

	Coord<double> pos,
                      offset,
		      c;
	unsigned int  maxiter;
	num_t        dmul;
};

struct CalcParam {
	View::Mode mode;
	Coord<int> coord;
	Coord<num_t> z, c, oc;
	unsigned int maxiter;
};

#define TRACE() //printf("%s\n", __FUNCTION__)


#endif // DEM_COMMON_H
