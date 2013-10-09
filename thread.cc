#include "thread.h"


void *__thread_run(void *obj) {
	void *retval = 0;

	Thread *thread = static_cast<Thread*>(obj);
	thread->running = true;
	thread->run();
	thread->running = false;
	thread->exit();
	return retval;
}


SpinLock Thread::idmtx;
atomic_long Thread::nextId;
