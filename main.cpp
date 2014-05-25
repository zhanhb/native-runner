/*
 * File:   main.cpp
 * Author: zhanhb
 *
 * Created on December 26, 2013, 10:13 PM
 */

//#define TRACE 1

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include "platform.h"
#include "trace.h"
#include "arguments_parser.h"
#include "constants.h"
#include "judgement.h"
#include "io_util.h"
#include "judge_process.h"
#include "copy_stream_thread.h"
#include "sandbox.h"
#include "mingw-unicode.h"  // wmain

static String escapeCString(const String & str) {
    String result;
    size_t size = str.length();
    LPCTSTR p = &str[0];
    result.reserve(size);
    for (; size--;) {
        int ch = *p++ & 0xFFFF;
        if (' ' <= ch && ch < 0x7F) {
            switch (ch) {
                case '"':
                case '\'':
                case '\\':
                    result += '\\';
            }
            result += ch;
        } else {
            switch (ch) {
                case '\b':
                    result += __TEXT("\\b");
                    break;
                case '\n':
                    result += __TEXT("\\n");
                    break;
                case '\t':
                    result += __TEXT("\\t");
                    break;
                case '\r':
                    result += __TEXT("\\r");
                    break;
                default:
                    if (ch < 128) {
                        static const char hexChars[] = "0123456789ABCDEF";
                        result += __TEXT("\\u");
                        result += hexChars[ch >> 12 & 15];
                        result += hexChars[ch >> 8 & 15];
                        result += hexChars[ch >> 4 & 15];
                        result += hexChars[ch & 15];
                    } else {
                        result += ch;
                    }
                    break;
            }
        }
    }
    return result;
}

static BOOL createProcess(String prog, HANDLE & hIn, HANDLE & hOut, HANDLE & hErr,
        HANDLE & outRead, HANDLE & errRead,
        bool redirectErrorStream, String & message, PROCESS_INFORMATION & pi) {
    BOOL ret = FALSE;
    SECURITY_ATTRIBUTES sa = {sizeof (SECURITY_ATTRIBUTES)};
    STARTUPINFO si = {sizeof (STARTUPINFO)};
    LPCTSTR pdir = NULL;
    LPVOID penvBlock = NULL;

    HANDLE outWrite = NULL, errWrite = NULL; // child side

    out(hIn);
    out(hOut);
    out(hErr);
    out(prog);

    sa.bInheritHandle = TRUE;
    if (!(CreatePipe(&outRead, &outWrite, &sa, PIPE_SIZE) &&
            (redirectErrorStream || CreatePipe(&errRead, &errWrite, &sa, PIPE_SIZE)))) {
        message = __TEXT("CreatePipe failed");
        goto Finally;
    }

    out(outRead);
    out(errRead);

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hIn;
    si.hStdOutput = outWrite;
    si.hStdError = redirectErrorStream ? outWrite : errWrite;

    ZeroMemory(&pi, sizeof pi);

    SetHandleInformation(hIn, HANDLE_FLAG_INHERIT, TRUE);
    SetHandleInformation(outRead, HANDLE_FLAG_INHERIT, FALSE);
    SetHandleInformation(outWrite, HANDLE_FLAG_INHERIT, TRUE);
    if (!redirectErrorStream) {
        SetHandleInformation(errWrite, HANDLE_FLAG_INHERIT, TRUE);
        SetHandleInformation(errRead, HANDLE_FLAG_INHERIT, FALSE);
    }

    {
        DWORD processFlag = CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT | CREATE_SUSPENDED;

        // pass error mode SEM_NOGPFAULTERRORBOX to the child process
        UINT oldErrorMode = SetErrorMode(SEM_NOGPFAULTERRORBOX);
        ret = CreateProcess(
                NULL, // executable name
                &prog[0], // command line
                NULL, // process security attribute
                NULL, // thread security attribute
                TRUE, // inherits system handles
                processFlag, // selected based on exe type
                penvBlock, // environment block
                pdir, // change to the new current directory
                &si, // (in)  startup information
                &pi); // (out) process information
        SetErrorMode(oldErrorMode);

        if (!ret) {
            TCHAR msg[1024];
            _sntprintf(msg, 1024,
                    __TEXT("CreateProcess: %s error=%d"), prog.c_str(), GetLastError());
            message = msg;
            goto Finally;
        }
    }

Finally:
    /* Always clean up the child's side of the pipes */
    if (outWrite) CloseHandle(outWrite);
    if (errWrite) CloseHandle(errWrite);

    return ret;
}

int _tmain(int argc, TCHAR **argv) {
    checkRequirements();
    ArgumentsParser parser;
    parser.parse(argc, argv);

    const String & inputFileName = parser.inputFileName;
    const String & outputFileName = parser.outputFileName;
    bool redirectErrorStream = parser.redirectErrorStream;
    const String & errFileName = parser.errFileName;

    LL timeLimit = parser.timeLimit;
    LL memoryLimit = parser.memoryLimit;
    LL outputLimit = parser.outputLimit;

    HANDLE hIn = NULL, hOut = NULL, hErr = NULL; // file handles
    HANDLE outRead = NULL, errRead = NULL; // parent side

    BOOL success = FALSE;
    String message;
    String detailMessage;
    PROCESS_INFORMATION pi;

    do {
        out(argv[0]);
        out(inputFileName);
        out(outputFileName);
        out(redirectErrorStream);
        out(errFileName);
        out(timeLimit);
        out(memoryLimit);
        out(outputLimit);

        if (!(fileOpen(inputFileName, O_RDONLY, hIn) &&
                fileOpen(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, hOut) &&
                (redirectErrorStream || fileOpen(errFileName, O_WRONLY | O_CREAT | O_TRUNC, hErr)))) {
            int error = GetLastError();
            if (error == ERROR_TOO_MANY_OPEN_FILES) {
                detailMessage = __TEXT("Too many open files");
                break;
            }
            if (!hIn) {
                detailMessage = __TEXT("File '");
                detailMessage += inputFileName;
                detailMessage += __TEXT("' not found");
            } else {
                detailMessage = __TEXT("Can't open file '");
                if (!hOut)detailMessage += outputFileName;
                else detailMessage += errFileName;
                detailMessage += __TEXT("' to write");
            }
            break;
        }
        success = TRUE;
    } while (false);

    out(hIn);
    out(hOut);
    out(hErr);

    out(outRead);
    out(errRead);

    if (success && !createProcess(parser.prog,
            hIn, hOut, hErr, outRead, errRead,
            redirectErrorStream, message, pi))success = FALSE;

    Judgement judgement(pi);
    HANDLE hMutex = NULL;
    Sandbox sandbox;

    while (success) {
        success = FALSE;

        hMutex = CreateMutex(
                NULL, // default security descriptor
                FALSE, // mutex not owned
                __TEXT("NameOfMutexObject")); // object name

        if (hMutex == NULL) {
            detailMessage = __TEXT("Create Mutex");
            break;
        }

        out(hMutex);

        judgement.setMutex(hMutex);
        CopyStreamThread pOut(outRead, hOut, outputLimit, judgement);
        CopyStreamThread pErr(errRead, hErr, outputLimit, judgement);

        if (!(pOut.start() && pErr.start())) {
            detailMessage = __TEXT("Create copy stream thread error");
            break;
        }

        if (!sandbox.beforeProcessStart(judgement)) {
            detailMessage = __TEXT("Create sandbox failed.");
            break;
        }

        while (TRUE) {
            DWORD dwCount = ResumeThread(pi.hThread);
            if (dwCount == 0 || dwCount == (DWORD) - 1)break;
        }

        for (success = TRUE;;) {
            if (!judgement.updateMemory()) {
                detailMessage = __TEXT("Get process memory info failed");
                success = FALSE;
                break;
            }
            if (judgement.memory > memoryLimit) {
                judgement.terminate(MEMORY_LIMIT_EXCEED);
                if (judgement.join(TERMINATE_TIMEOUT)) {// wait until the process has really exit
                    success = TRUE;
                } else {
                    success = FALSE;
                    detailMessage = __TEXT("terminate process timeout");
                }
                break;
            }
            if (!judgement.updateTimeNow()) {
                detailMessage = __TEXT("Get process time info failed");
                success = FALSE;
                break;
            }
            LL escaped = judgement.time;
            if (timeLimit <= escaped) {
                judgement.terminate(TIME_LIMIT_EXCEED);
                if (judgement.join(TERMINATE_TIMEOUT)) { // wait until the process has really exit
                    success = TRUE;
                } else {
                    success = FALSE;
                    detailMessage = __TEXT("terminate process timeout");
                }
                break;
            }
            DWORD dwWaitTime = timeLimit - escaped;
            if (dwWaitTime > UPDATE_TIME_THRESHOLD)dwWaitTime = UPDATE_TIME_THRESHOLD;
            if (judgement.join(dwWaitTime)) {
                success = TRUE;
                break;
            }
        }

        if (!success)break;
        success = FALSE;

        if (!judgement.updateMemory())break;
        if (!judgement.updateTime())break;

        pOut.join();
        pErr.join();

        success = TRUE;
        break;
    }

    int exitCode = judgement.getExitCode();
    int haltCode = judgement.getHaltCode();

    switch (exitCode) {
        case EXCEPTION_ACCESS_VIOLATION:
            haltCode = RUNTIME_ERROR;
            message = __TEXT("ACCESS_VIOLATION");
            break;
        case STATUS_ARRAY_BOUNDS_EXCEEDED:
            haltCode = RUNTIME_ERROR;
            message = __TEXT("ARRAY_BOUNDS_EXCEEDED");
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            haltCode = RUNTIME_ERROR;
            message = __TEXT("DIVIDE_BY_ZERO");
            break;
        case EXCEPTION_STACK_OVERFLOW:
            haltCode = RUNTIME_ERROR;
            message = __TEXT("STACK_OVERFLOW");
            break;
        case STATUS_NO_MEMORY:
            haltCode = MEMORY_LIMIT_EXCEED;
            break;
    }

    printf("{time:\"" LLS "\"", judgement.time);
    printf(",memory:\"" LLS "\"", judgement.memory);
    printf(",haltCode:\"%d\""
            ",exitCode:\"%u\"", haltCode, exitCode);

    if (message.length()) _tprintf(__TEXT(",message:\"%s\""), escapeCString(message).c_str());
    if (detailMessage.length()) _tprintf(__TEXT(",detailMessage:\"%s\""), escapeCString(detailMessage).c_str());
    _putts(__TEXT("}"));

    if (!success) {
        // maybe process is still running
        judgement.terminateIfRunning(JUDGE_ERROR);
    }

    if (hMutex)CloseHandle(hMutex);

    if (hIn)CloseHandle(hIn);
    if (hOut)CloseHandle(hOut);
    if (hErr)CloseHandle(hErr);

    if (outRead)CloseHandle(outRead);
    if (errRead)CloseHandle(errRead);

    if (pi.hThread) CloseHandle(pi.hThread);
    if (pi.hProcess) CloseHandle(pi.hProcess);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
