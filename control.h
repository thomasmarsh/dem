#ifndef DEM_CONTROL_H
#define DEM_CONTROL_H

#include "common.h"
#include "render.h"
#include "display.h"
#include "thread.h"

struct State {
        bool needsRecalc,
             saveRequest,
             nextView,
             prevView,
             planeChangeRequest,
             planeChangeReverse;

        State()
                : needsRecalc(true),
                  saveRequest(false),
                  nextView(false),
                  prevView(false),
                  planeChangeRequest(false),
                  planeChangeReverse(false)
        {}

        int viewIndex;

        num_t dmul, imul;

        View::Plane plane;
};

class Controller : public Thread {
public:
	Squarepusher queue;
	View        &view;
	Coord<int>   res;
        State        state;
private:
	RenderEngine render;
	GlutDriver   display;


	void incView();
	void decView();
public:
	Controller();

	void init(int argc, char **argv);

	void reset();
	void nextView() { stop(); state.nextView = true; }
	void prevView() { stop(); state.prevView = true; }
	void dmaxUp()   { stop(); state.dmul *= 2; restart(); printf("dmul=%Lf\n",state.dmul);}
	void dmaxDown() { stop(); state.dmul /= 2; restart(); printf("dmul=%Lf\n",state.dmul);}
	void iterUp()   { stop(); state.imul *= 2; restart(); printf("imul=%Lf\n",state.imul);}
	void iterDown() { stop(); state.imul /= 2; restart(); printf("imul=%Lf\n",state.imul);}
	void recalc()   { stop(); state.needsRecalc = true; }
	void save()     { stop(); state.saveRequest = true; }
	void stop()     { render.stop(); }
	void restart()  { stop(); recalc(); }

	void changePlane(bool reverse) { stop();
					 state.planeChangeRequest = true,
					 state.planeChangeReverse = reverse; }

	void doChangePlane();

	void resize(Coord<int> size);

	void draw();
	void write();

	void run();
	void loop();
	void setView();
};

#endif // DEM_CONTROL_H
