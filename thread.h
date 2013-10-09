#ifndef R_THREAD_H
#define R_THREAD_H
#pragma once

#include <queue>
#include <string>
#include <exception>
#include <cerrno>
#include <cstring>

extern "C" {
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

void *__thread_run(void *obj);
}


class atomic_long {
private:
	volatile long counter;

public:
	atomic_long() : counter(0) {}

	uint64_t inc()   { return __sync_add_and_fetch(&counter, 1); }
	void set(long n) { counter = n; }
	long get()       { return counter; }
};


// Base Exception for all services

class Exception : public std::exception {
private:
	const char  *msg;
public:
	Exception() : msg(0) {}
	Exception(const char *m) : msg(m){}

	virtual const char *what() const throw() {
		if (!msg) return "Exception in thread library";
		return msg;
	}
};


// An exception to be thrown from threading routines
//
// TODO: eventually this should look at the error return codes and populate
// with useful end-user messages

class ThreadException : public Exception {
private:
        std::string msg;
public:
	ThreadException() : Exception() {}
	ThreadException(const char *m) : Exception(m) {}
	ThreadException(int e, const char *m) 
        {
                msg = m;
                msg += ": ";
                msg += strerror(e);
                Exception(msg.c_str());
                printf("EXC: %s\n", msg.c_str());
        }

        virtual ~ThreadException() throw() {}
};


#if 0
#ifdef linux
# define LTYPE		pthread_spinlock_t
# define LVAR 		spinlock
# define INIT() 	pthread_spin_init(&spinlock, 0)
# define DESTROY() 	pthread_spin_destroy(&spinlock)
# define LOCK()	pthread_spin_lock(&spinlock)
# define UNLOCK()	pthread_spin_unlock(&spinlock)
#else // !linux
# include <libkern/OSAtomic.h>
# define LTYPE		OSSpinLock
# define LVAR 		osspinlock
# define INIT() 	do { osspinlock = 0; } while (0)
# define DESTROY() 
#  define LOCK()	OSSpinLockLock(&osspinlock)
# define UNLOCK()	OSSpinLockUnlock(&osspinlock)
#endif // linux


class SpinLock {
private:
	LTYPE LVAR;
public:
	SpinLock() { INIT(); }
	void lock() { LOCK(); }
	void unlock() { UNLOCK(); }
	~SpinLock() { DESTROY(); }
};
#else

class SpinLock {
private:
        volatile int exclusion;
public:
        SpinLock() : exclusion(0) {}
        void lock()   { while (__sync_lock_test_and_set(&exclusion, 1)); }
        void unlock() { __sync_lock_release(&exclusion); }
};
#endif

class Thread {
protected:
	static atomic_long nextId;
	static SpinLock idmtx;

	long threadId;
	bool running;

	pthread_t thread;
	pthread_attr_t attr;

	void exit() {
		pthread_exit(NULL);
	}

	friend void *__thread_run(void *);
public:
	int getId() { return threadId; }

	Thread() : running(false) {
		idmtx.lock();
		threadId = nextId.inc();
		idmtx.unlock();

		if (pthread_attr_init(&attr) != 0)
			throw ThreadException("pthread_attr_init");
	}

	void start() {
		if (running) return;

		int rc = pthread_create(
				&thread,
				&attr,
				__thread_run,
				static_cast<void*>(this));
		if (rc != 0) throw ThreadException("pthread_create");
	}

	void join() {
		void *retval;
		if (pthread_join(thread, &retval) != 0)
			throw ThreadException("pthread_join");
	}

	virtual ~Thread() {}
protected:
	virtual void run() = 0;
};


// The following classes are higher level objects which utilize the above
// low level classes.
//
// Classes: Guard, SimpleCond, PrioritySemaphore<T>

class Guard {
private:
        SpinLock &mutex;
public:
        Guard(SpinLock &m) : mutex(m) { mutex.lock(); }
        ~Guard() { mutex.unlock(); }
};


template <typename T>
class SyncVar {
private:
        T value;
        bool changed;
        SpinLock lock;
public:
        SyncVar(T initial) : value(initial), changed(true) {}

        bool isDirty() {
                Guard g(lock);
                return changed;
        }

        SyncVar & operator = (T & v) {
                Guard g(lock);
                value   = v,
                changed = true;
                return *this;
        }

        operator T&() {
                Guard g(lock);
                changed = false;
                return value; 
        }
};

#endif // R_THREAD_H
