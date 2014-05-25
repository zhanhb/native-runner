#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/arguments_parser.o \
	${OBJECTDIR}/copy_stream_thread.o \
	${OBJECTDIR}/io_util_md.o \
	${OBJECTDIR}/judge_process.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/platform.o \
	${OBJECTDIR}/sandbox.o \
	${OBJECTDIR}/thread.o


# C Compiler Flags
CFLAGS=-DPSAPI_VERSION=1

# CC Compiler Flags
CCFLAGS=-m32 -DPSAPI_VERSION=1
CXXFLAGS=-m32 -DPSAPI_VERSION=1

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=lib/Psapi.Lib lib/psapi.dll

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nativerunner.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nativerunner.exe: lib/Psapi.Lib

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nativerunner.exe: lib/psapi.dll

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nativerunner.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nativerunner ${OBJECTFILES} ${LDLIBSOPTIONS} -static

${OBJECTDIR}/arguments_parser.o: arguments_parser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/arguments_parser.o arguments_parser.cpp

${OBJECTDIR}/copy_stream_thread.o: copy_stream_thread.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/copy_stream_thread.o copy_stream_thread.cpp

${OBJECTDIR}/io_util_md.o: io_util_md.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/io_util_md.o io_util_md.cpp

${OBJECTDIR}/judge_process.o: judge_process.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/judge_process.o judge_process.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/platform.o: platform.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/platform.o platform.cpp

${OBJECTDIR}/sandbox.o: sandbox.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sandbox.o sandbox.cpp

${OBJECTDIR}/thread.o: thread.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/thread.o thread.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nativerunner.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
