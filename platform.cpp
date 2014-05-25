#include "platform.h"

void checkRequirements() {
    OSVERSIONINFO ver = {sizeof (ver)};
    BOOL onNT = FALSE;
    GetVersionEx(&ver);
    if (ver.dwPlatformId == VER_PLATFORM_WIN32_NT) onNT = TRUE;
    if (!onNT) {
        _putts(__TEXT("Requirements"));
        _putts(__TEXT(""));
        _tprintf(__TEXT("  %s\t%s\n"),
                __TEXT("Minimum supported client"),
                __TEXT("Windows XP [desktop apps only]"));
        _tprintf(__TEXT("  %s\t%s\n"),
                __TEXT("Minimum supported server"),
                __TEXT("Windows Server 2003 [desktop apps only]"));
        _putts(__TEXT(""));
        exit(-1);
    }
}

#define FT2INT64(ft) \
        ((LL)(ft).dwHighDateTime << 32 | (LL)(ft).dwLowDateTime)

LL currentTimeMillis() {
    static LL fileTime_1_1_70 = 0;
    SYSTEMTIME st0;
    FILETIME ft0;

    if (fileTime_1_1_70 == 0) {
        /* Initialize fileTime_1_1_70 -- the Win32 file time of midnight
         * 1/1/70.
         */
        SYSTEMTIME st1;
        FILETIME ft1;

        memset(&st1, 0, sizeof (st1));
        st1.wYear = 1970;
        st1.wMonth = 1;
        st1.wDay = 1;
        SystemTimeToFileTime(&st1, &ft1);
        fileTime_1_1_70 = FT2INT64(ft1);
    }

    GetSystemTime(&st0);
    SystemTimeToFileTime(&st0, &ft0);

    return (FT2INT64(ft0) - fileTime_1_1_70) / 10000;
}

