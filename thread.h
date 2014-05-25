/* 
 * File:   thread.h
 * Author: zhanhb
 *
 * Created on January 3, 2014, 10:00 PM
 */

#ifndef THREAD_H
#define	THREAD_H

#include "platform.h"

class Thread {
public:

    Thread() : handle(NULL), started(FALSE) { }

    virtual BOOL start();
    virtual void run() = 0;
    void join();
    virtual ~Thread();
private:
    BOOL started;
    HANDLE handle;
};

#endif	/* THREAD_H */

