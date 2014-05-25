/* 
 * File:   judgement.h
 * Author: zhanhb
 *
 * Created on January 6, 2014, 11:54 PM
 */

#ifndef JUDGEMENT_H
#define	JUDGEMENT_H

#include <assert.h>
#include "platform.h"

class Judgement {
private:
    HANDLE hMutex;
    BOOL halted;
    int haltCode;

public:
    const PROCESS_INFORMATION & pi;

    LL time, memory;

    Judgement(PROCESS_INFORMATION & p) : pi(p), hMutex(NULL), halted(FALSE), time(0), memory(0), haltCode(0) { }

private:

    void forceTerminate(int errorCode) {
        haltCode = errorCode;
        if (pi.hProcess) TerminateProcess(pi.hProcess, 1);
    }
public:

    void terminate(int errorCode) {
        if (halted)return;
        HANDLE hMutex = this->hMutex;
        if (hMutex == NULL) {
            halted = TRUE;
            forceTerminate(errorCode);
            return;
        }
        DWORD dwWaitResult = WaitForSingleObject(
                hMutex, // handle to mutex
                INFINITE); // no time-out interval
        switch (dwWaitResult) {
                // The thread got ownership of the mutex
            case WAIT_OBJECT_0:
                if (!halted) {
                    halted = TRUE;
                    forceTerminate(errorCode);
                }
                // Release ownership of the mutex object
                if (!ReleaseMutex(hMutex)) {
                    // Handle error.
                }
                break;
                // The thread got ownership of an abandoned mutex
            case WAIT_ABANDONED:
                assert(FALSE);
        }
    }

    void setMutex(HANDLE mutex) {
        hMutex = mutex;
    }

    void terminateIfRunning(int errorCode) {
        if (pi.hProcess != NULL && WaitForSingleObject(pi.hProcess, 0) != WAIT_OBJECT_0) {
            terminate(errorCode);
        }
    }
private:

    void updateTime(LL time) {
        this->time = time;
    }

    void updateMemory(LL memory) {
        if (memory > this->memory)
            this->memory = memory;
    }
public:

    BOOL join(DWORD millis) {
        return pi.hProcess != NULL && WaitForSingleObject(pi.hProcess, millis) == WAIT_OBJECT_0;
    }

#define FT2INT64(ft) \
       (((LL)(ft).dwHighDateTime << 32 | (LL)(ft).dwLowDateTime) / 10000)
#define TIME_DIFF(ft1, ft2) ((FT2INT64(ft1) - FT2INT64(ft2)))

    BOOL updateTime() {
        FILETIME ftCreateTime, ftExitTime, temp;
        if (GetProcessTimes(pi.hProcess, &ftCreateTime, &ftExitTime, &temp, &temp)) {
            updateTime(TIME_DIFF(ftExitTime, ftCreateTime));
            return TRUE;
        }
        return FALSE;
    }

    BOOL updateTimeNow() {
        FILETIME ftCreateTime, temp;
        SYSTEMTIME stNow;
        if (GetProcessTimes(pi.hProcess, &ftCreateTime, &temp, &temp, &temp)) {
            GetSystemTime(&stNow);
            SystemTimeToFileTime(&stNow, &temp);
            updateTime(TIME_DIFF(temp, ftCreateTime));
            return TRUE;
        }
        return FALSE;
    }

#undef FT2INT64

    BOOL updateMemory() {
        if (pi.hProcess != NULL) {
            PROCESS_MEMORY_COUNTERS pmc;
            memset(&pmc, 0, sizeof pmc);
            pmc.cb = sizeof pmc;

            if (GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof pmc)) {
                LL memory = pmc.PeakWorkingSetSize;
                updateMemory(memory);
                return TRUE;
            }
        }
        return FALSE;
    }

    int getHaltCode() const {
        return haltCode;
    }

    int getExitCode() const {
        DWORD dwExitCode = 0;
        if (pi.hProcess) GetExitCodeProcess(pi.hProcess, &dwExitCode);
        return dwExitCode;
    }
};

#endif	/* JUDGEMENT_H */

