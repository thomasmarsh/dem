#include <stdlib.h>
#define GL_GLEXT_PROTOTYPES
#ifdef linux
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>

#include "orbit.h"
#include "display.h"
#include "rect.h"
#include "render.h"
#include "control.h"

static const int TIMER_DELAY = 10;


//----------------------------------------------------------------------------
//
// GLUT CALLBACKS
//
//----------------------------------------------------------------------------

GlutDriver *GLUT_FORWARD;

static void glut_display_cb() { GLUT_FORWARD->display(); }
static void glut_process_cb(int v) { GLUT_FORWARD->process(); }
static void glut_idle_cb(void) { GLUT_FORWARD->process(); }
static void glut_reshape_cb(int w, int h) { GLUT_FORWARD->reshape(w, h); }

static void glut_keyboard_cb(unsigned char key, int mx, int my) {
	GLUT_FORWARD->handleKeyboard(key);
}

static void glut_special_cb(int key, int x, int y) {
	GLUT_FORWARD->handleSpecial(key);
}

static void glut_mouse_cb(int button, int state, int x, int y) {
	GLUT_FORWARD->handleMouse(button, state, x, y);
}


//----------------------------------------------------------------------------
//
// CONSTRUCTOR / INITIALIZATION
//
//----------------------------------------------------------------------------

GlutDriver::GlutDriver(Controller &c)
	: controller(c)
{
	draw_orbit = false;
	GLUT_FORWARD = this;
}


void GlutDriver::init(int argc, char **argv) {
	TRACE();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(controller.res.x, controller.res.y);
	glutCreateWindow("DEM");

	reshape(controller.res.x, controller.res.y);

	// setup callback handlers
	glutDisplayFunc(glut_display_cb);
	glutReshapeFunc(glut_reshape_cb);
	glutTimerFunc(TIMER_DELAY, glut_process_cb, 0);
	glutIdleFunc(glut_idle_cb);
	glutKeyboardFunc(glut_keyboard_cb);
	glutMouseFunc(glut_mouse_cb);
	glutSpecialFunc(glut_special_cb);
	clear();
}


//----------------------------------------------------------------------------
//
// CALLBACK IMPLEMENTATIONS
//
//----------------------------------------------------------------------------

void GlutDriver::handleKeyboard(unsigned char key) {
	TRACE();
	switch (key) {
	case 'q':
		controller.stop();
		::exit(0);
		break;
	case 's':
		controller.save();
		break;
	case 'z':
		controller.stop();
		break;
	case 'r':
		controller.restart();
		clear();
		break;
	case 'p':
		controller.changePlane(false);
		clear();
		break;
	case 'P':
		controller.changePlane(true);
		clear();
		break;
	case '[':
		controller.prevView();
		clear();
		break;
	case ']':
		controller.nextView();
		clear();
		break;
	default:
		break;
	}
}


void GlutDriver::handleSpecial(int key) {
	TRACE();
	switch (key) {
	case GLUT_KEY_UP:
		controller.iterUp();
		clear();
		break;
	case GLUT_KEY_DOWN:
		controller.iterDown();
		clear();
		break;
	case GLUT_KEY_LEFT:
		controller.dmaxUp();
		clear();
		break;
	case GLUT_KEY_RIGHT:
		controller.dmaxDown();
		clear();
		break;
	default:
		break;
	}
}

Coord<double> zz;
void GlutDriver::handleMouse(int button, int state, int x, int y) {
	TRACE();
	if (button == GLUT_LEFT && state == GLUT_DOWN) {
		View &view = controller.view;

		double sx = view.offset.x / controller.res.x,
		       sy = view.offset.y / controller.res.y;
		zz.x = view.pos.x - (view.offset.x/2) + (sx*x),
		zz.y = view.pos.y + (view.offset.y/2) - (sy*y);
		printf("%g,%g\n", zz.x, zz.y);
		draw_orbit = true;
	} else if (button == GLUT_LEFT && state == GLUT_UP)
		draw_orbit = false;
}


void GlutDriver::process() {
	TRACE();
	glutPostRedisplay();
	usleep(50);
}


void GlutDriver::run() {
	TRACE();
	glutMainLoop();
}


void GlutDriver::pushSquares() {
	TRACE();
	Rect r;
	for (unsigned int i=0; i < controller.queue.numEntries(); i++) {
		controller.queue.pop(&r);
		glColor3d(r.c.r, r.c.g, r.c.b);
		glRectd(r.x, r.y, r.x+r.s, r.y+r.s);
	}
	glFlush();
}


void GlutDriver::drawOrbits() {
	TRACE();
	if (!draw_orbit) return;
	draw_orbit = false;

	View &view = controller.view;
	orbit.calc(zz.x, zz.y, view.c.x, view.c.y);

	double rx = view.pos.x - view.offset.x/2,
	       ry = view.pos.y + view.offset.y/2;

	glBegin(GL_LINE_STRIP);
	glColor3f(1, 1, 0);
	double x, y;
	for (int i=0; i < orbit.len; i++) {
		x = (orbit.escape[i].x - rx) / view.offset.x * controller.res.x,
		  y = -(orbit.escape[i].y - ry) / view.offset.y * controller.res.y;
		glVertex2f(x, y);
	}
	glEnd();
}


void GlutDriver::clearScreen() {
	TRACE();
	if (clear_screen) {
		clear_screen = false;

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glRectf(0, 0, controller.res.x, controller.res.y);
	}
}


void GlutDriver::swapBuffers() {
	// swap and setup timerfunc again
	TRACE();
	glFlush();
	glutSwapBuffers();
	glutTimerFunc(TIMER_DELAY, glut_process_cb, 0);
}


void GlutDriver::reshape(int w, int h) {
	TRACE();
	printf("resize: %d x %d\n", w, h);

	Coord<int> size = { w, h };
	controller.resize(size);

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, w-1, h-1, 0);


	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void GlutDriver::display() {
	TRACE();
	clearScreen();
	pushSquares();
	drawOrbits();
	swapBuffers();
}
