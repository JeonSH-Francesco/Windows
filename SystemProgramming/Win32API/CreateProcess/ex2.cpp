#include <iostream>
#include <windows.h>
#include <string>

int main() {
    SECURITY_ATTRIBUTES saAttr;
    HANDLE hChildStdoutRead, hChildStdoutWrite;
    BOOL fSuccess;

    // Set up the security attributes for the pipe
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    // Create a pipe for the child process's STDOUT
    if (!CreatePipe(&hChildStdoutRead, &hChildStdoutWrite, &saAttr, 0)) {
        std::cerr << "Error creating pipe\n";
        return 1;
    }

    // Ensure the read handle to the pipe for STDOUT is not inherited
    if (!SetHandleInformation(hChildStdoutRead, HANDLE_FLAG_INHERIT, 0)) {
        std::cerr << "Error setting handle information\n";
        return 1;
    }

    // Create the child process
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = hChildStdoutWrite;
    siStartInfo.hStdOutput = hChildStdoutWrite;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process
    wchar_t cmdLine[] = L"netstat -ano"; // Wide 문자열로 변경
    bSuccess = CreateProcess(NULL,
        cmdLine, // Command to execute (Wide 문자열 사용)
        NULL,          // Process handle not inheritable
        NULL,          // Thread handle not inheritable
        TRUE,          // Set handle inheritance to TRUE
        0,             // No creation flags
        NULL,          // Use parent's environment block
        NULL,          // Use parent's starting directory 
        &siStartInfo,  // Pointer to STARTUPINFO structure
        &piProcInfo); // Pointer to PROCESS_INFORMATION structure

    if (!bSuccess) {
        std::cerr << "CreateProcess failed\n";
        CloseHandle(hChildStdoutRead);
        CloseHandle(hChildStdoutWrite);
        return 1;
    }

    // Close the write end of the pipe before reading from the read end of the pipe
    CloseHandle(hChildStdoutWrite);

    // Read output from the child process's pipe for STDOUT
    CHAR chBuf[4096];
    DWORD dwRead;
    std::string output;
    while (ReadFile(hChildStdoutRead, chBuf, 4096, &dwRead, NULL) && dwRead != 0) {
        output.append(chBuf, dwRead);
    }

    // Close handles
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
    CloseHandle(hChildStdoutRead);

    // Print the output
    std::cout << output << std::endl;
    return 0;
}
