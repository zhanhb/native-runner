/* 
 * File:   copy_stream_thread.h
 * Author: zhanhb
 *
 * Created on April 4, 2014, 7:00 PM
 */

#ifndef COPY_STREAM_THREAD_H
#define	COPY_STREAM_THREAD_H

#include "constants.h"
#include "thread.h"
#include "judgement.h"

class CopyStreamThread : public Thread {
private:
    // use reference to ensure close only once
    HANDLE &hRead, &hWrite;
    Judgement & judgement;
    LL dwLimit;

public:

    CopyStreamThread(HANDLE & arg1, HANDLE & arg2, LL arg3, Judgement & arg4)
    : hRead(arg1), hWrite(arg2), dwLimit(arg3), judgement(arg4) { }

    void run();
};

#endif	/* COPY_STREAM_THREAD_H */

