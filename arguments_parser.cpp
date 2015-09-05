#include "arguments_parser.h"
#include "platform.h"
#include "judge_process.h"

static String basename(const String & filename) {
    return filename.substr(filename.find_last_of('\\') + 1);
}

#define usage() _usage(argv[0], 1, NULL)
#define parseLong(s, x) _parseLong(argv, s, x)

static void _usage(LPCTSTR sProg, int exitValue, LPCTSTR prompt) {
    if (prompt != NULL)_ftprintf(stderr, _TEXT("%s\n"), prompt);
    _ftprintf(stderr, _TEXT("Usage: %s [option] commands...\n\n"), basename(sProg).c_str());

    static const char *options[] = {
        "-input <file>", "input file",
        "-output <file>", "output file",
        "-error <file>", "error output file",
        "-redirectErrorStream", "redirect error stream to output file if this argument is present",
        "-time <time limit>", "time limit",
        "-memory <memory limit>", "memory limit",
        "-ol <output limit>", "output limit",
        "-help, --help, -?, /?", "show usage"
    };
#define SIZE(x) (sizeof(x) / sizeof(x[0]))
    int len = 8;
    for (int i = 1; i < SIZE(options); i += 2) {
        int tlen = strlen(options[i - 1]);
        if (len < tlen)len = tlen;
    }
    for (int i = 1; i < SIZE(options); i += 2) {
        fprintf(stderr, "  %-*s  %s\n", len, options[i - 1], options[i]);
    }
#undef SIZE
    _ftprintf(stderr, __TEXT("\n"));
    ExitProcess(exitValue);
}

static void _parseLong(const LPCTSTR * argv, LPTSTR s, LL & x) {
    size_t len = _tcslen(s), n;
    if (len > 18 || 1 != _stscanf(s, __TEXT("" LLS "%n"), &x, &n) || n != len) {
        _ftprintf(stderr, __TEXT("Number format exception for input string: '%s'.\n"), s);
        usage();
    }
}

static void _checkNotEmpty(TCHAR ** argv, const String & ref, const LPCTSTR message) {
    if (ref.length() == 0) {
        if (message != NULL) {
            _usage(argv[0], -1, message);
        }
        usage();
    }
}

#define checkNotEmpty(ref, message) _checkNotEmpty(argv, ref, message)

void ArgumentsParser::parse(int argc, TCHAR ** argv) {
    String inputFileTag = __TEXT("-input");
    String outputFileTag = __TEXT("-output");
    String errorFileTag = __TEXT("-error");
    String timeLimitTag = __TEXT("-time");
    String memoryLimitTag = __TEXT("-memory");
    String outputLimitTag = __TEXT("-ol");
    String redirectErrorStreamTag = __TEXT("-redirectErrorStream");
    String str;

    int argNo = 1;
    while (argNo < argc) {
        str = argv[argNo];
        if (str.length() && str[0] == '-') {
            if (str == inputFileTag) {
                if (++argNo < argc) {
                    inputFileName = argv[argNo++];
                    continue;
                }
            } else if (str == outputFileTag) {
                if (++argNo < argc) {
                    outputFileName = argv[argNo++];
                    continue;
                }
            } else if (str == errorFileTag) {
                if (++argNo < argc) {
                    errFileName = argv[argNo++];
                    continue;
                }
            } else if (str == timeLimitTag) {
                if (++argNo < argc) {
                    parseLong(argv[argNo++], timeLimit);
                    continue;
                }
            } else if (str == memoryLimitTag) {
                if (++argNo < argc) {
                    parseLong(argv[argNo++], memoryLimit);
                    continue;
                }
            } else if (str == outputLimitTag) {
                if (++argNo < argc) {
                    parseLong(argv[argNo++], outputLimit);
                    continue;
                }
            } else if (str == redirectErrorStreamTag) {
                redirectErrorStream = true;
                ++argNo;
                continue;
            } else {
                _ftprintf(stderr, __TEXT("Unknown option '%s'\n"), str.c_str());
            }
            usage();
        } else {
            parseArguments(argc - argNo, argv + argNo, prog);
            break;
        }
    }
    if (argc <= 1)usage();
    checkNotEmpty(inputFileName, __TEXT("Input file missing."));
    checkNotEmpty(outputFileName, __TEXT("Output file missing."));
    if (!redirectErrorStream)checkNotEmpty(outputFileName, __TEXT("Error output file missing."));
    checkNotEmpty(prog, __TEXT("Program arguments must not be empty."));
}

