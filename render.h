#ifndef DEM_RENDER_H
#define DEM_RENDER_H

#include "common.h"
#include "rect.h"
#include "thread.h"
#include "buffer.h"
#include "generator.h"
#include "formula.h"

#include <cmath>

static const int NUM_THREADS = 8;
static const num_t DMAX_FUDGE = 1e-18;

class Controller;

class RenderEngine : public RenderReactor {
private:
	Controller   &controller;
	Coord<int>    res;
	RenderBuf<Color> buf;
	Generator     gen;

	num_t        dmax;
	Coord<num_t> step;
	Coord<num_t> initial;
	int 	      chunkLevel;

public:
	enum Mode { DRAW, SAVE };

	Mode mode;
	bool 	      stopped;
	RenderEngine(Controller &c);
	~RenderEngine();

	void resize(Coord<int> &r) { res = r;  reconfig(); }
	void stop();
	void render(Mode mode);
	void reconfig();
private:

	void result(CalcParam &p, num_t dist, int iter);

	num_t distance_max(num_t range, num_t thick);
	void initParam(CalcParam &p);
	void setParamStart(CalcParam &p);

	template <typename F> void render(CalcParam &p);

	void correctPlane(Coord<num_t> &p);


	// ---------------------
	// WORKER
  	// ---------------------

	class Worker : public Thread {
	private:
		RenderEngine &r;
//		SpinLock mutex;
		SyncVar<bool> done, running;
	public:
		bool isRunning() { return running; }
		Worker(Generator &g, RenderEngine &_r) : r(_r), done(false), running(false) {}

		void setDone(bool val) { done = val; }
		bool isDone() { return done; }

		bool hasData() { return !r.gen.done(); }

	private:
		void run();
	};

	Worker *worker[NUM_THREADS];
};

#endif // DEM_RENDER_H
