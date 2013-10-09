#ifndef DEM_BUFFER_H
#define DEM_BUFFER_H

#include "common.h"
#include "thread.h"

#include <assert.h>

template <typename T>
struct RenderBuf {
private:
	//SpinLock mutex;
	Coord<int> res;
	T *buffer;

public:
	RenderBuf() : buffer(0)/*,cur(0),count(0)*/ {}
	~RenderBuf() { delete[] buffer; }

	void resize(Coord<int> r) {
		res = r;

		if (buffer)
			delete[] buffer;

		buffer = new T[res.x*res.y];
	}

	void set(const Coord<int> c, T val) {
		//Guard m(mutex);
		buffer[c.x+c.y*res.y] = val;
	}

	void print() {
		fprintf(stderr, "             \r");
		FILE *fp = fopen("out.ppm", "w");
		fprintf(fp, "P6\n%d %d\n255\n", res.x, res.y);
		printf("saving...");
		fflush(stdout);
		int count = 0;
		for (int y=0; y < res.y; y++) {
			for (int x=0; x < res.x; x++) {
				Color &c = buffer[x+y*res.y];
				fprintf(fp, "%c%c%c", (char) (c.r * 255),
						      (char) (c.g * 255),
						      (char) (c.b * 255));
				count += 3;
			}
		}
		fclose(fp);
		printf("done\n");
		fflush(stdout);
	}
};

#endif // DEM_BUFFER_H
