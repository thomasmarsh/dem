#ifndef DEM_DISPLAY_H
#define DEM_DISPLAY_H

#include "common.h"
#include "thread.h"
#include "orbit.h"
#include "rect.h"

class Controller;

class GlutDriver {
private:
	Controller &controller;
	bool clear_screen;
	bool draw_orbit;
	Orbit orbit;
public:
	GlutDriver(Controller &c);

	void init(int c, char **v);
	void display();
	void clear() { clear_screen = true; }

	void process();
	void reshape(int w, int h);
	void handleMouse(int button, int state, int x, int y);
	void handleSpecial(int key);
	void handleKeyboard(unsigned char key);

	void run();
private:
	void pushSquares();
	void drawOrbits();
	void clearScreen();
	void swapBuffers();
};

#endif // DEM_DISPLAY_H
