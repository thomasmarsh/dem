#include "control.h"

int main(int argc, char **argv) {
	Controller c;

	c.init(argc, argv);
	c.loop();
}
