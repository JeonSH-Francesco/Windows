#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

typedef struct _THREAD_PARAM {
    FARPROC pLoadLibraryA;
    FARPROC pGetProcAddress;
    char szLibName[MAX_PATH];
    char szFuncName[MAX_PATH];
    char szMessage[MAX_PATH];
} THREAD_PARAM;

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
    THREAD_PARAM* pParam = (THREAD_PARAM*)lpParameter;

    // Load the library
    HMODULE hMod = ((HMODULE(*)(LPCSTR))pParam->pLoadLibraryA)(pParam->szLibName);

    if (hMod) {
        // Get the function address
        FARPROC pFunc = ((FARPROC(*)(HMODULE, LPCSTR))pParam->pGetProcAddress)(hMod, pParam->szFuncName);
        if (pFunc) {
            // Call the function (MessageBoxA)
            ((int(*)(HWND, LPCSTR, LPCSTR, UINT))pFunc)(NULL, pParam->szMessage, "Injection", MB_OK);
        }
    }

    return 0;
}

BOOL InjectCode(DWORD dwPID) {
    HMODULE hMod = NULL;
    THREAD_PARAM param = { 0 }; // 구조체 변수를 선언하고 초기화합니다.
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;
    LPVOID pRemoteBuf[2] = { 0, };
    DWORD dwSize = 0;

    hMod = GetModuleHandleA("kernel32.dll");

    // Set THREAD_PARAM
    param.pLoadLibraryA = GetProcAddress(hMod, "LoadLibraryA");
    param.pGetProcAddress = GetProcAddress(hMod, "GetProcAddress");
    strcpy_s(param.szLibName, MAX_PATH, "user32.dll");
    strcpy_s(param.szFuncName, MAX_PATH, "MessageBoxA");
    strcpy_s(param.szMessage, MAX_PATH, "Code Injection Successful!");

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    if (hProcess == NULL) {
        printf("Failed to open process. Error: %d\n", GetLastError());
        return FALSE;
    }

    // Allocate memory in the remote process for THREAD_PARAM
    dwSize = sizeof(THREAD_PARAM);
    pRemoteBuf[0] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteBuf[0] == NULL) {
        printf("Failed to allocate memory in the remote process. Error: %d\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }
    WriteProcessMemory(hProcess, pRemoteBuf[0], (LPVOID)&param, dwSize, NULL);

    // Allocate memory in the remote process for ThreadProc
    dwSize = (DWORD)InjectCode - (DWORD)ThreadProc;
    pRemoteBuf[1] = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (pRemoteBuf[1] == NULL) {
        printf("Failed to allocate memory for the thread procedure. Error: %d\n", GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf[0], 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    WriteProcessMemory(hProcess, pRemoteBuf[1], (LPVOID)ThreadProc, dwSize, NULL);

    // Create the remote thread
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteBuf[1], pRemoteBuf[0], 0, NULL);
    if (hThread == NULL) {
        printf("Failed to create remote thread. Error: %d\n", GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf[0], 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, pRemoteBuf[1], 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);

    // Cleanup
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteBuf[0], 0, MEM_RELEASE);
    VirtualFreeEx(hProcess, pRemoteBuf[1], 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return TRUE;
}

int main() {
    DWORD dwPID;

    printf("Enter the PID of the target process: ");
    scanf("%u", &dwPID);

    if (InjectCode(dwPID)) {
        printf("Code injected successfully.\n");
    }
    else {
        printf("Failed to inject code.\n");
    }

    return 0;
}
