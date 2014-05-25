#include <fcntl.h>
#include "io_util.h"
#include "platform.h"

BOOL fileOpen(const String & path, int flags, HANDLE & handle) {
    const DWORD access =
        (flags & O_WRONLY) ?  GENERIC_WRITE :
        (flags & O_RDWR)   ? (GENERIC_READ | GENERIC_WRITE) :
        GENERIC_READ;
    const DWORD sharing =
        FILE_SHARE_READ | FILE_SHARE_WRITE;
    const DWORD disposition =
        /* Note: O_TRUNC overrides O_CREAT */
        (flags & O_TRUNC) ? CREATE_ALWAYS :
        (flags & O_CREAT) ? OPEN_ALWAYS   :
        OPEN_EXISTING;
    const DWORD  maybeWriteThrough =
        (flags & (O_SYNC | O_DSYNC)) ?
        FILE_FLAG_WRITE_THROUGH :
        FILE_ATTRIBUTE_NORMAL;
    const DWORD maybeDeleteOnClose =
        (flags & O_TEMPORARY) ?
        FILE_FLAG_DELETE_ON_CLOSE :
        FILE_ATTRIBUTE_NORMAL;
    const DWORD flagsAndAttributes = maybeWriteThrough | maybeDeleteOnClose;
    HANDLE h = NULL;

    h = CreateFile(
            &path[0], /* Wide char path name */
            access, /* Read and/or write permission */
            sharing, /* File sharing flags */
            NULL, /* Security attributes */
            disposition, /* creation disposition */
            flagsAndAttributes, /* flags and attributes */
            NULL);

    if (!h || h == INVALID_HANDLE_VALUE) {
        return FALSE;
    }
    handle = h;
    return TRUE;
}
