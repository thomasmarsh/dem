#include "control.h"
#include "views.h"


static Coord<int> DEFAULT_RESOLUTION = { 1040, 1040 };

Controller::Controller()
      : queue(DEFAULT_RESOLUTION),
	view(views[0]),
        res(DEFAULT_RESOLUTION),
        render(*this),
	display(*this)
{
	state.needsRecalc = true;
	state.saveRequest = false;
	state.nextView = false;
	state.prevView = false;
	state.viewIndex = 0;
	reset();
}


void Controller::init(int argc, char **argv) {
	display.init(argc, argv);
	if (argc == 2) {
		state.viewIndex = atoi(argv[1]);
		setView();
		write();
		::exit(0);
	}
	setView();
}

void Controller::reset() {
	state.dmul = view.dmul;
	state.imul = 1;
	state.plane = view.plane;
}

void Controller::write() {
	state.saveRequest = false;
	int x = res.x, y = res.y;
	res.x = 4096;
	res.y = 4096;
	render.reconfig();
	render.render(RenderEngine::SAVE);
	res.x = x;
	res.y = y;
	render.reconfig();
}

void Controller::draw() {
	display.clear();
	render.reconfig();
	render.render(RenderEngine::DRAW);
}

void Controller::run() {
	usleep(6000);
	while (1) {
		if (state.needsRecalc) { draw(); state.needsRecalc = false; }
		if (state.planeChangeRequest) { doChangePlane(); }
		if (state.saveRequest) write();
		if (state.nextView)    incView();
		if (state.prevView)    decView();
		usleep(20);
	}
}

void Controller::setView() {
	view = views[state.viewIndex];
	printf("view = %d\n", state.viewIndex);
	reset();
	recalc();
}

void Controller::doChangePlane() {
	render.stop();
	int plane = (int) state.plane;

	if (state.planeChangeReverse)
		plane = ((plane+View::NUM_PLANES)-1) % View::NUM_PLANES;
	else
		plane = (plane+1) % View::NUM_PLANES;

	state.plane = (View::Plane) plane;

	state.planeChangeRequest = false;

	recalc();
}

void Controller::incView() {
	render.stop();
	state.nextView = false;
	state.viewIndex = (state.viewIndex + 1) % NVIEWS;
	setView();
}

void Controller::decView() {
	render.stop();
	state.prevView = false;
	state.viewIndex = (state.viewIndex + NVIEWS - 1) % NVIEWS;
	setView();
}

void Controller::loop() {
	start();
	display.run();
}

void Controller::resize(Coord<int> sz) {
	queue.resize(sz);
	render.resize(sz);
}
