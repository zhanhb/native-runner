#include "sandbox.h"

Sandbox::Sandbox() {
    hJob = CreateJobObject(NULL, NULL);

    JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = {0};
    jobli.ActiveProcessLimit = 1;
    // These are the only 1 restrictions I want placed on the job (process).
    jobli.LimitFlags = JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
    SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &jobli,
            sizeof (jobli));

    // Second, set some UI restrictions.
    JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir = {0};
    jobuir.UIRestrictionsClass =
            // The process can't access USER objects (such as other windows)
            // in the system.
            JOB_OBJECT_UILIMIT_HANDLES |
            JOB_OBJECT_UILIMIT_READCLIPBOARD |
            JOB_OBJECT_UILIMIT_WRITECLIPBOARD |
            JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS |
            JOB_OBJECT_UILIMIT_DISPLAYSETTINGS |
            JOB_OBJECT_UILIMIT_GLOBALATOMS |
            // Prevents processes associated with the job from creating desktops
            // and switching desktops using the CreateDesktop and SwitchDesktop functions.
            JOB_OBJECT_UILIMIT_DESKTOP |
            // The process can't log off the system.
            JOB_OBJECT_UILIMIT_EXITWINDOWS;
    SetInformationJobObject(hJob, JobObjectBasicUIRestrictions, &jobuir,
            sizeof (jobuir));
}

BOOL Sandbox::beforeProcessStart(Judgement & judgement) {
    // After this we can allow the child process' thread to execute code.
    return AssignProcessToJobObject(hJob, judgement.pi.hProcess);
}

Sandbox::~Sandbox() {
    CloseHandle(hJob);
}
