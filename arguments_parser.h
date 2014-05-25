/* 
 * File:   argument_parser.h
 * Author: zhanhb
 *
 * Created on April 4, 2014, 11:34 AM
 */

#ifndef ARGUMENT_PARSER_H
#define	ARGUMENT_PARSER_H
#include "platform.h"

class ArgumentsParser {
public:
    String inputFileName;
    String outputFileName;
    String errFileName;

    LL timeLimit;
    LL memoryLimit;
    LL outputLimit;
    String prog;
    bool redirectErrorStream;

    ArgumentsParser() : timeLimit(LL_MAX), memoryLimit(LL_MAX), outputLimit(LL_MAX), redirectErrorStream(false) {
    }

    void parse(int argc, TCHAR ** argv);
};

#endif	/* ARGUMENT_PARSER_H */

