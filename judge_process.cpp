#include "judge_process.h"

#define VERIFICATION_CMD_BAT    0
#define VERIFICATION_WIN32      1
#define VERIFICATION_LEGACY     2

static bool isQuoted(LPCTSTR arg) {
    size_t lastPos = _tcslen(arg);
    if (lastPos >= 2 && arg[0] == '"' && arg[lastPos - 1] == '"') {
        // The argument has already been quoted.
        return true;
    }
    return false;
}

static bool needsEscaping(int verificationType, LPCTSTR arg) {
    // Switch off MS heuristic for internal ["].
    // Please, use the explicit [cmd.exe] call
    // if you need the internal ["].
    //    Example: "cmd.exe", "/C", "Extended_MS_Syntax"

    // For [.exe] or [.com] file the unpaired/internal ["]
    // in the argument is not a problem.
    bool argIsQuoted = isQuoted(arg);

    static const TCHAR * ESCAPE_VERIFICATION[] = {
        __TEXT(" \t<>&|^"),
        __TEXT(" \t<>"),
        __TEXT(" \t")
    };

    const TCHAR * testEscape = ESCAPE_VERIFICATION[verificationType];
    if (!argIsQuoted) {
        for (int i = 0; arg[i]; ++i) {
            if (_tcschr(testEscape, arg[i]))return true;
        }
    }
    return false;
}

static void createCommandLine(int verificationType, int argc, const LPCTSTR * argv, String & cmdbuf) {
    cmdbuf = '"';
    cmdbuf += argv[0];
    cmdbuf += '"';

    for (int i = 1; i < argc; ++i) {
        cmdbuf += __TEXT(' ');
        const TCHAR * s = argv[i];
        if (needsEscaping(verificationType, s)) {
            cmdbuf += __TEXT('"');
            cmdbuf += s;

            // The code protects the [java.exe] and console command line
            // parser, that interprets the [\"] combination as an escape
            // sequence for the ["] char.
            //     http://msdn.microsoft.com/en-us/library/17w5ykft.aspx
            //
            // If the argument is an FS path, doubling of the tail [\]
            // char is not a problem for non-console applications.
            //
            // The [\"] sequence is not an escape sequence for the [cmd.exe]
            // command line parser. The case of the [""] tail escape
            // sequence could not be realized due to the argument validation
            // procedure.
            if ((verificationType != VERIFICATION_CMD_BAT) && s[0] && s[_tcslen(s) - 1] == __TEXT('\\')) {
                cmdbuf += __TEXT('\\');
            }
            cmdbuf += __TEXT('"');
        } else {
            cmdbuf += s;
        }
    }
}

static bool isShellFile(LPCTSTR executablePath) {
    size_t len = _tcslen(executablePath);
    if (len > 4) {
        if (executablePath[len - 4] == '.'
                && (executablePath[len - 3] == 'C' || executablePath[len - 3] == 'c')
                && (executablePath[len - 2] == 'M' || executablePath[len - 2] == 'm')
                && (executablePath[len - 1] == 'D' || executablePath[len - 1] == 'd'))
            return true;

        if (executablePath[len - 4] == '.'
                && (executablePath[len - 3] == 'B' || executablePath[len - 3] == 'b')
                && (executablePath[len - 2] == 'A' || executablePath[len - 2] == 'a')
                && (executablePath[len - 1] == 'T' || executablePath[len - 1] == 't'))
            return true;
    }
    return false;
}

void parseArguments(int argc, const LPCTSTR * argv, String & cmdstr) {
    if (argc == 0) {
        cmdstr.clear();
        return;
    }
    // Quotation protects from interpretation of the [path] argument as
    // start of longer path with spaces. Quotation has no influence to
    // [.exe] extension heuristic.
    createCommandLine(
            // We need the extended verification procedure for CMD files.
            isShellFile(argv[0])
            ? VERIFICATION_CMD_BAT
            : VERIFICATION_WIN32,
            argc, argv,
            cmdstr);
}
