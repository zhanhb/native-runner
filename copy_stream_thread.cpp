#include "copy_stream_thread.h"

void CopyStreamThread::run() {
    CopyStreamThread * param = this;
    HANDLE & hRead = param->hRead, & hWrite = param->hWrite;
    Judgement & judgement = param->judgement;
    LL limit = param->dwLimit;

    DWORD dwRead = 0, dwWritten = 0;
    char buf[BUFFER_SIZE];

    while (TRUE) {
        if (ReadFile(hRead, buf, BUFFER_SIZE, &dwRead, NULL) || dwRead != 0) {
            if (dwRead == 0) break;
            limit -= dwRead;
            if (limit < 0) {
                judgement.terminate(OUTPUT_LIMIT_EXCEED);
                break;
            }
            if (!WriteFile(hWrite, buf, dwRead, &dwWritten, NULL)) {
                // process may exited now, ignore the error
                break;
            }
        } else {
            // process may exited now, ignore the error
            break;
        }
    }

    CloseHandle(hRead);
    hRead = NULL;
    CloseHandle(hWrite);
    hWrite = NULL;
}
