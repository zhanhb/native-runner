#include "thread.h"

static DWORD WINAPI ThreadRun(LPVOID param) {
    static_cast<Thread*> (param)->run();
    return 0;
}

BOOL Thread::start() {
    if (started)return FALSE;
    started = true;
    SECURITY_ATTRIBUTES saThreadAttributes = {sizeof (SECURITY_ATTRIBUTES)};
    SIZE_T dwStackSize = 0;
    LPTHREAD_START_ROUTINE lpStartAddress = &ThreadRun; // A pointer to the application-defined function to be executed by the thread.
    LPVOID lpParameter = this; // A pointer to a variable to be passed to the thread.
    DWORD dwCreationFlags = 0; // The flags that control the creation of the thread.
    LPDWORD lpThreadId = NULL; // A pointer to a variable that receives the thread identifier.

    handle = CreateThread(&saThreadAttributes,
            dwStackSize,
            lpStartAddress,
            lpParameter,
            dwCreationFlags,
            lpThreadId);
    return handle != NULL;
}

void Thread::join() {
    HANDLE h = handle;
    if (h) WaitForSingleObject(h, INFINITE);
}

Thread::~Thread() {
    HANDLE h = handle;
    if (h) CloseHandle(h);
}