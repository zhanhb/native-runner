/* 
 * File:   longlong.h
 * Author: zhanhb
 *
 * Created on January 4, 2014, 8:18 PM
 */

#ifndef LONGLONG_H
#define	LONGLONG_H

#include <limits.h>
#if defined (_WIN32) || defined (__WIN32) || defined (WIN32) || defined (__WIN32__)
#define LL __int64
#define LLS "%I64d"
#define LLU "%I64u"
#define LL_MAX _I64_MAX
#else
#define LL long long
#define LLS "%lld"
#define LLU "%llu"
#define LL_MAX LONG_LONG_MAX
#endif

#endif	/* LONGLONG_H */

