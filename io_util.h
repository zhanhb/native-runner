/* 
 * File:   io_util.h
 * Author: zhanhb
 *
 * Created on January 5, 2014, 7:15 PM
 */

#ifndef IO_UTIL_H
#define	IO_UTIL_H

#include "platform.h"

#if !defined(O_DSYNC) || !defined(O_SYNC)
#define O_SYNC  (0x0800)
#define O_DSYNC (0x2000)
#endif

// TODO
BOOL fileOpen(const String & path, int flags, HANDLE & handle);

#endif	/* IO_UTIL_H */

