#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#define ACCEPTED                0x00000000
#define PRESENTATION_ERROR      0x00000001
#define WRONG_ANSWER            0x00000002
#define TIME_LIMIT_EXCEED       0x00000003
#define MEMORY_LIMIT_EXCEED     0x00000004
#define RUNTIME_ERROR           0x00000005
#define COMPILE_ERROR           0x00000006
#define OUTPUT_LIMIT_EXCEED     0x00000007
#define QUEUING                 0x00000008
#define COMPILING               0x00000009
#define RUNNING                 0x0000000A
#define VALIDATING              0x0000000B
#define JUDGE_ERROR             0x0000000C
#define RESTRICTED_FUNCTION     0x0000000D

#define UPDATE_TIME_THRESHOLD           50
#define BUFFER_SIZE                     8192
#define PIPE_SIZE                       BUFFER_SIZE
#define TERMINATE_TIMEOUT               15000

#endif
