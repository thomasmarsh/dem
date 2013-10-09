extern "C" {
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
}

#include "generator.h"


void loop(Generator &ri, int i) {
        Coord<int> c;
        while (!ri.done()) {
                ri.next(c, i);
                printf("  { %d, %d } - { %d, %d }\n", c.x, c.y, c.x + (1<< i)-1,
                                                                c.y + (1<< i)-1);
        }
}

int main() {
	Generator ri;
        Coord<int> size = { 8, 8 };
        ri.resize(size);

        for (int i=2; i > -1; i--) {
                ri.reset(4);
                printf(" -- %d --\n", i);
                loop(ri, i);
        }
}
