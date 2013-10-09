#ifndef DEM_RECT_H
#define DEM_RECT_H

#include "common.h"
#include "thread.h"

#include <string.h>

struct Rect {
	Color c;
	num_t x, y, s;

	void set(Color _c, num_t _x, num_t _y, num_t _s) {
		c=_c, x=_x, y=_y, s=_s;
	}
};

class Squarepusher {
private:
	unsigned int wpos, rpos;
	unsigned int entries;
	unsigned int size;
	Rect **queue;
	SpinLock mutex;

	void alloc() {
		queue = new Rect*[size];
		for (unsigned int i=0; i < size; i++)
			queue[i] = new Rect;
	}
public:
	Squarepusher(Coord<int> &r) : wpos(0), rpos(0), entries(0), size(r.x*r.y) {
		alloc();
	}

	void dealloc() {
		for (unsigned int i=0; i < size; i++)
			delete queue[i];
		delete[] queue;
	}

	void resize(Coord<int> &r) {
		Guard m(mutex);

		dealloc();
		size = r.x*r.y;
		alloc();

		wpos = rpos = 0;
		entries = 0;
	}

	void push(Color c, num_t x, num_t y, num_t s) {
		Guard m(mutex);

		queue[wpos]->set(c, x, y, s);
		wpos = (wpos+1) % size;
		entries++;
	}

	unsigned int numEntries() { return entries; }

	void reset() { wpos = rpos = 0;  entries = 0; }

	void pop(Rect *r) {
		Guard m(mutex);

		memcpy(r, queue[rpos], sizeof(Rect));
		rpos = (rpos+1) % size;
		entries--;
	}
};

#endif // DEM_RECT_H
