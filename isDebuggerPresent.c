#include <stdio.h>
#include <windows.h>

int main() {
    BOOL isDebuggerPresent = FALSE;

    // Check if the process is being debugged remotely
    if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent)) {
        if (isDebuggerPresent) {
            printf("Debugging is detected using CheckRemoteDebuggerPresent.\n");
        }
        else {
            printf("No remote debugging detected using CheckRemoteDebuggerPresent.\n");
        }
    }
    else {
        printf("Error while checking debugging status using CheckRemoteDebuggerPresent.\n");
    }

    return 0;
}

