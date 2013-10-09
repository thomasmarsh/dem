
const int NVIEWS = 35;

View views[NVIEWS] = {
	{			// 0
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ 0, 0 },
		{ 3, 3 },
		{ 0, 0 },
		7000, 1
	},

	{			// 1
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.751878166, -0.034018858},
		{ 0.00083737688, 0.00083737688 },
		{ 0, 0 },
		7000, 1
	},

	{			// 2
		View::MANDELBROT,
		View::JULIA, View::STANDARD,
		{ 0, 0 },{ 3, 3 },
		{ -0.31317747870461659159, 0.64619028889236225970 },
		7000, 4.39805e+12
	},

	{			// 3
		View::MANDELBROT,
		View::JULIA, View::STANDARD,
		{ 0, 0 } , { 3, 3 },
		{ -0.1, 0.651 },
		10000, 6.87195e+10
	},

	{			// 4
		View::MANDELBROT,
		View::JULIA, View::STANDARD,
		{ 0, 0 }, { 3, 3 },
		{ -0.74543, 0.11301 },
		10000, 1.71799e+10
	},

	{			// 5
		View::MANDELBROT,
		View::JULIA, View::STANDARD,
		{0, 0 }, {3, 3},
		{ 0.285,  0.01 },
		10000, 2.19902e+12
	},

	{			// 6
		View::MANDELBROT,
		View::JULIA, View::STANDARD,
		{ 0, 0 }, { 3, 3 },
		{ -0.835, -0.2321 },
		10000, 1.75922e+13
	},

	{			// 7
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.769502838, -0.116273301 },
		{ 0.000078848835, 0.000078848835 },
		{ 0, 0 },
		10000, 1
	},

	{			// 8
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.777821233585, -0.120755672689 },
		{ 0.011377240534, 0.011377240534 },
		{ 0, 0 },
		10000, 1
	},

	{			// 9
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.759, 0 }, { 2.5, 2.5 },
		{ 0, 0 },
		200, 16777216 * 4
	},

	{			// 10
		View::MANDELBROT3,
		View::MANDEL, View::STANDARD,
		{ 0, 0 }, { 3, 3 },
		{ 0, 0 },
		10000, 1
	},

	{			// 11
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.745067, 0.118346 },
		{ 0.0007, 0.0007 },
		{ 0, 0 },
		10000, 1
	},

	{			// 12
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.74453985651, 0.12172377365 },
		{ 3.072e-8, 3.072e-8 },
		{ 0, 0 },
		100000L, 1.16415e-10
	},

	{			// 13
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ 0.2925755, -0.0149977 },
		{ 0.0004277, 0.0004277 },
		{ 0, 0 },
		10000L, 1
	},

	{			// 14
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.550769, 0.626543 },
		{ 0.005938, 0.005938 },
		{ 0, 0 },
		10000L, 1
	},

	{			// 15
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -1.74876455, 0.00000001 },
		{ 0.00002441, 0.00002441 },
		{ 0, 0 },
		1000000L, 1
	},

	{			// 16
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.7454291059302, 0.1130087933933 },
		{ 1.8e-10, 1.8e-10 },
		{ 0, 0 },
		1000000L, 1.38778e-17
	},

	{			// 17
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ 0.34369, -0.06625 },
		{ 0.0029296875, 0.0029296875 },
		{ 0, 0 },
		10000L, 1
	},

	{			// 18
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -1.039657910475, 0.348321555773 },
		{ 0.000000000266, 0.000000000266 },
		{ 0, 0 },
		10000L, 2.77556e-17
	},

	{			// 19
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -1.766526530883, 0 },
		{ 0.046604911802, 0.046604911802 },
		{ 0, 0 },
		10000L, 2.09715e+06
	},

	{			// 20 
		View::MANDELBROT,
		View::MANDEL, View::INVERSE,
		{ -0.87618, 0.299721 },
		{ 0.0564895, 0.0564895 },
		{ 0, 0 },
		10000L, 4.1943e+06
	},

	{			// 21
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -0.744371701858, -0.110071107252},
		{ 1.0e-13, 1.0e-13 },
		{ 0, 0 },
		10000, 4.03897e-28
	},

	{			// 22
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ 0.265699964380, 0.003654050018 },
		{ 0.000000000011, 0.000000000011 },
		{ 0, 0 },
		2000, 5.42101e-20
	},

	{			// 23
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -1.768509903809, 0.001271209112 },
		{ 0.000037717865, 0.000037717865 },
		{ 0, 0 },
		16000, 0.0078125
	},

	{			// 24
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{  -0.743643900055, 0.131825890901 },
		{ 0.000000049304, 0.000000049304 },
		{ 0, 0 },
		100000, 5.82077e-14
	},

	{ 			// 25
		View::MANDELBROT,
		View::MANDEL, View::STANDARD,
		{ -1.2095881121794871, -0.13821506410256415 },
		{ 0.00012, 0.00012 },
		{ 0, 0 },
		40000, 2.98023e-08
	},

	{ 			// 26
		View::EXPERIMENT1,
		View::MANDEL, View::STANDARD,
		{ 0, 0 },
		{ 3, 3 },
		{ 0.3, 0.6 },
		125, 1
	},

	{ 			// 27
		View::COLLATZ,
		View::MANDEL, View::STANDARD,
		{ 0, 0 },
		{ 10, 10 },
		{ 0.3, 0.6 },
		125, 1
	},

	{ 			// 28
		View::MULTIBROT,
		View::MANDEL, View::STANDARD,
		{ 0, 0 },
		{ 3, 3 },
		{ 3.1, -.1 },
		200, 1
	},

	{ 			// 29
		View::NEWTON,
		View::MANDEL, View::STANDARD,
		{ 0, 0 },
		{ 3, 3 },
		{ 3.1, -.1 },
		200, 1
	},

	{ 			// 30
		View::NEWTON2,
		View::MANDEL, View::STANDARD,
		{ 0, 0 },
		{ 3, 3 },
		{ 3.1, -.1 },
		200, 1
	},

	{ 			// 31
		View::NEWTON3,
		View::MANDEL, View::STANDARD,
		{ 0, 0 },
		{ 3, 3 },
		{ 3.1, -.1 },
		200, 1
	},

	{ 			// 32
		View::MANDELBROT,
		View::MANDEL, View::INVERSE_OFFSET2,
		{ -195000, 0 },
		{ 650000, 650000 },
		{ 0, 0, },
		6000, 1
	},


	{			// 33
		View::MANDELBROT3,
		View::JULIA, View::STANDARD,
		{ 0, 0 },
		{ 2.5, 2.5 },
		{ 0.457506, -0.546719 },
		400, 1
	},

	{			// 34
		View::EXPERIMENT1,
		View::MANDEL, View::STANDARD,
		{ 1.33635, 0.524615 },
		{ .03, .03 },
		{ 0, 0 },
		400, 1
	},
};
