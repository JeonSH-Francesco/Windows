#include <windows.h>
#include <iostream>
using namespace std;

int main() {
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD dwProcessID = 0;
    DWORD dwThreadID = 0;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    BOOL bCreateProcess = CreateProcess(
        L"C:\\Windows\\System32\\notepad.exe",
        NULL,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    if (bCreateProcess == FALSE) {
        cout << "Create Process Failed & Error No -" << GetLastError() << endl;
        return 1; // 실패 시 종료
    }

    cout << "Create Process Success" << endl;
    cout << "Process ID: " << pi.dwProcessId << endl;
    cout << "Thread ID: " << pi.dwThreadId << endl;

    // GetProcessId 및 GetThreadId 함수를 호출하기 전에 핸들이 유효한지 확인
    if (pi.hProcess != NULL) {
        cout << "GetProcessID: " << GetProcessId(pi.hProcess) << endl;
    }
    if (pi.hThread != NULL) {
        cout << "GetThreadID: " << GetThreadId(pi.hThread) << endl;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}
