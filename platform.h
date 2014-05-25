/* 
 * File:   platform.h
 * Author: zhanhb
 *
 * Created on January 5, 2014, 5:23 PM
 */

#ifndef PLATFORM_H
#define	PLATFORM_H

#define UNICODE
#define _UNICODE

#undef WINVER
#undef _WIN32_WINNT
#define WINVER 0x0500
#define _WIN32_WINNT 0x0500

#include <stdio.h>
#include <string>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include "longlong.h"
// remember to add project option -DPSAPI_VERSION=1
#include "psapi.h"
#pragma comment(lib, "psapi.lib")
#ifdef _UNICODE
typedef std::wstring String;
#else
typedef std::string String;
#endif

void checkRequirements();

LL currentTimeMillis();

#endif	/* PLATFORM_H */

