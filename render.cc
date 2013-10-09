#include "render.h"
#include "control.h"
#include "plane.h"
#include "formula.h"

#include <complex>

RenderEngine::RenderEngine(Controller &c)
	: controller(c)
{
	reconfig();
	for (int i=0; i < NUM_THREADS; i++)
		worker[i] = new Worker(gen, *this);
	stopped = true;
}


RenderEngine::~RenderEngine() {
	for (int i=0; i < NUM_THREADS; i++)
		delete worker[i];
}


void RenderEngine::stop() {
	for (int i=0; i < NUM_THREADS; i++)
		worker[i]->setDone(true);
	for (int i=0; i < NUM_THREADS; i++)
		while (worker[i]->isRunning())
	stopped = true;
}


num_t RenderEngine::distance_max(num_t range, num_t thick) {
	// reference point is -1.5 to 1.5 (3.0)
	return std::sqrt((controller.state.dmul * thick * DMAX_FUDGE)
				/ (1000 * (range/3)));
}


class RenderContext {
private:
	RenderBuf<Color> buf;
	Coord<num_t> step,initial;
	Generator gen;
	View *view;
	num_t dmax;
public:
	void set(View *v, Coord<int> &res, State &state) {
		view = v;
		buf.resize(res);
		gen.resize(res);
		dmax = std::sqrt((state.dmul * res.x * DMAX_FUDGE) / (1000 * (view->offset.x/3)));
		step.x = view->offset.x / res.x;
		step.y = view->offset.y / res.y;
		initial.x = view->pos.x - view->offset.x / 2;
		initial.y = view->pos.y - view->offset.y / 2;
	}
};

void RenderEngine::reconfig() {
	View &view = controller.view;

	buf.resize(controller.res);
	gen.resize(controller.res);

	dmax = distance_max(view.offset.x, controller.res.x);
	printf("dmax=%Lf\n", dmax);

	step.x = view.offset.x / controller.res.x;
	step.y = view.offset.y / controller.res.y;

	initial.x = view.pos.x - view.offset.x / 2;
	initial.y = view.pos.y + view.offset.y / 2;
}


//----------------------------------------------------------------------------
//
// CORE RENDERING ALGORITHM
//
//----------------------------------------------------------------------------

void RenderEngine::render(Mode m) {
	stopped = false;
	mode = m;

	if (mode == DRAW) {
		for (chunkLevel=5; (!stopped) && (chunkLevel > -1) ; chunkLevel--) {
			gen.reset(5);
			for (int i=0; i < NUM_THREADS; i++) {
				worker[i]->setDone(false);
				worker[i]->start();
			}
			for (int i=0; i < NUM_THREADS; i++)
				worker[i]->join();
		}
	} else {
		gen.reset(0);
		chunkLevel = 0;
		for (int i=0; i < NUM_THREADS; i++) {
			worker[i]->setDone(false);
			worker[i]->start();
		}
		for (int i=0; i < NUM_THREADS; i++)
			worker[i]->join();
		buf.print();
	}
}


inline
void RenderEngine::initParam(CalcParam &p) {
	p.z.x = (initial.x + step.x * (num_t) p.coord.x);
	p.z.y = (initial.y - step.y * (num_t) p.coord.y);

	if (controller.state.plane != View::STANDARD) {
		CorrectPlane(p.z, controller.state.plane);
		CorrectPlane(p.c, controller.state.plane);
	}
}


inline
void RenderEngine::setParamStart(CalcParam &p) {
	View &view = controller.view;

	p.oc.x = view.c.x;
	p.oc.y = view.c.y;

	if (view.mode == View::MANDEL)
		p.c.x = p.z.x,     p.c.y = p.z.y;
	else    p.c.x = view.c.x,  p.c.y = view.c.y;
}
	


template <typename F>
inline
void RenderEngine::render(CalcParam &p) {

	initParam(p);
	if (F::bailout(p)) {
		result(p, 0, 0);
		return;
	}
	setParamStart(p);

	F::calc(p, *this);
}


void RenderEngine::Worker::run() {
	// is the point in the set
	running = true;

	// screen/image coordinate
	CalcParam param;

	param.mode    = r.controller.view.mode;
	param.maxiter = (int) ((num_t) r.controller.view.maxiter
				      * r.controller.state.imul);

	// get first index
	r.gen.next(param.coord, r.chunkLevel);

#define FORMULA(F1,F2) case (View::F1): r.render<F2>(param); break

	while (!isDone() && hasData()) {
		// expensive calculation
		switch (r.controller.view.formula) {
                FORMULA(MANDELBROT, Mandelbrot);
                FORMULA(MANDELBROT3, Mandelbrot3);
                FORMULA(COLLATZ, Collatz);
                FORMULA(EXPERIMENT1, Experiment1);
                FORMULA(MULTIBROT, Multibrot);
                FORMULA(NEWTON, Newton);
                FORMULA(NEWTON2, Newton2);
                FORMULA(NEWTON3, Newton3);
#undef FORMULA
		default:
                        fprintf(stderr, "ERROR: unhandled formula: %d\n", r.controller.view.formula);
                        ::exit(-1);
			break;
		}
                r.gen.next(param.coord, r.chunkLevel);
	}
	running = false;
}


void RenderEngine::result(CalcParam &p, num_t dist, int iter) {
	bool in_set = dist > dmax;
	Color c;
	if (!in_set) c.set(0, 0, 0);
	else { 
#if 0
		num_t x = (num_t)((int)(1.0/dist)%1000)/1000.0;
		num_t y = (num_t) iter / (num_t) controller.view.maxiter;
		c.b = 1-x*y;
		c.g = x*(1-y);
		c.r = (c.r + c.g) / 2.;
#else
		c.set(1, 1,1);
#endif
	}

	if (mode == DRAW)
		controller.queue.push(
			c,
			p.coord.x,
			p.coord.y,
			1<<chunkLevel);
	else { // mode == SAVE
		controller.queue.push(
			c,
			p.coord.x/4.0,
			p.coord.y/4.0,
			(1<<chunkLevel)/4.0);
		if (chunkLevel == 0)
			buf.set(p.coord, c);
	}
}
