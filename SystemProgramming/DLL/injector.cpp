#include <windows.h>
#include <iostream>
using namespace std;

// 주입할 DLL 경로를 적절히 위치시키고 설정한다.
const char* dllPath = "C:\\MyDll\\Dll1.dll";

int main() {
    DWORD processId;
    cout << "Enter the target process ID: ";
    cin >> processId;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        cerr << "Failed to open process: " << GetLastError() << endl;
        return 1;
    }

    LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pDllPath == NULL) {
        cerr << "Failed to allocate memory in target process: " << GetLastError() << endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, pDllPath, (LPVOID)dllPath, strlen(dllPath) + 1, NULL)) {
        cerr << "Failed to write to target process memory: " << GetLastError() << endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    HMODULE hKernel32 = GetModuleHandle(L"Kernel32");
    FARPROC pLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pDllPath, 0, NULL);
    if (hThread == NULL) {
        cerr << "Failed to create remote thread: " << GetLastError() << endl;
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    cout << "DLL injection succeeded." << endl;
    return 0;
}
