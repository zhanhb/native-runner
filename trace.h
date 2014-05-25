/* 
 * File:   trace.h
 * Author: zhanhb
 *
 * Created on April 14, 2014, 6:56 PM
 */

#ifndef TRACE_H
#define	TRACE_H

#if TRACE // true for trace
#include <iostream>
#include "platform.h"
#ifdef _UNICODE
#define out(x) (std::wcerr << "Line " << __LINE__ << ": " << #x << ": " << (x) << std::endl)
#else
#define out(x) (std::cerr << "Line " << __LINE__ << ": " << #x << ": " << (x) << std::endl)
#endif
#else
#define out(x) ((void)0)
#define NDEBUG
#endif

#endif	/* TRACE_H */

