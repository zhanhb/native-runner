/* 
 * File:   sandbox.h
 * Author: zhanhb
 *
 * Created on April 4, 2014, 9:14 PM
 */

#ifndef SANDBOX_H
#define	SANDBOX_H
#include "platform.h"
#include "judgement.h"

class Sandbox {
public:
    Sandbox();
    ~Sandbox();
    BOOL beforeProcessStart(Judgement &);
private:
    HANDLE hJob;
};

#endif	/* SANDBOX_H */

