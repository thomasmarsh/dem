#ifndef DEM_GENERATOR_H
#define DEM_GENERATOR_H

#include "common.h"
#include "thread.h"

#include <assert.h>

struct AntialiasGenerator {
private:
	SpinLock mutex;
public:
};

struct Generator {
private:
	SpinLock mutex;

	int maxLevel;
	unsigned long pos, max;
	Coord<int>    cpos, res;


	bool skip(int level) {
		return ((((cpos.x >> level) % 2) == 0) &&
			(((cpos.y >> level) % 2) == 0));
	}

	void setCurrent(Coord<int> &coord) {
		coord.x = pos % res.x;
		coord.y = pos / res.y;
	}
public:
	Generator() : maxLevel(5), pos(0), max(0) {
		cpos.x = 0;
		cpos.y = 0;
	}


	void resize(Coord<int> r) {
		Guard g(mutex);

		res = r;
		max = res.x * res.y;
	}

	void reset(int m) {
		Guard g(mutex);
		maxLevel = m;
		cpos.x = 0;
		cpos.y = 0;
		pos = 0;
	}

	bool done() { return pos >= (max-1); }

	void increment(int amt, int level) {
		if (level == 0) {
			pos++;
			cpos.x = pos % res.x;
			cpos.y = pos / res.y;
			return;
		}

		cpos.x += amt;
		if (cpos.x >= res.x) {
			cpos.x = 0;
			cpos.y += amt;
		}
		pos = cpos.x + cpos.y * res.y;
	}

	void next(Coord<int> &coord, int level) {
		Guard m(mutex);

		if (!(pos % (4096*10)))
			fprintf(stderr, "%0.2f%%      \r",
                                100*(float) pos / ((float) max));

		int amt = 1 << level;

		if ((level == maxLevel) ||
		    (level == 0 && maxLevel == 0)) {
			setCurrent(coord);
			increment(amt, level);
			return;
		}

		while (!done() && skip(level))
			increment(amt, level);

		setCurrent(coord);
		increment(amt, level);
	}
};

#endif // DEM_GENERATOR_H
