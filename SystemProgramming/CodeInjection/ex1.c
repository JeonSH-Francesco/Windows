#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

DWORD WINAPI InjectedFunction(LPVOID lpParam) {
    // MessageBox 호출
    MessageBox(NULL, L"Code Injection Successful!", L"Injection", MB_OK);
    return 0;
}


// 주입할 코드
BOOL InjectCode(DWORD dwPID) {
    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;
    LPVOID pRemoteBuf = NULL;
    DWORD dwThreadId = 0;

    // 목적 프로세스 열기
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    if (hProcess == NULL) {
        printf("Failed to open process.\n");
        return FALSE;
    }

    // 목적 프로세스의 주소 공간에 코드 삽입
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (pRemoteBuf == NULL) {
        printf("Failed to allocate memory in the remote process.\n");
        CloseHandle(hProcess);
        return FALSE;
    }

    // InjectedFunction 함수의 주소를 목적 프로세스로 복사
    WriteProcessMemory(hProcess, pRemoteBuf, &InjectedFunction, 4096, NULL);

    // 원격 스레드 생성
    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteBuf, NULL, 0, &dwThreadId);
    if (hThread == NULL) {
        printf("Failed to create remote thread.\n");
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    // 스레드 종료까지 대기
    WaitForSingleObject(hThread, INFINITE);

    // 정리
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return TRUE;
}

int main() {
    DWORD dwPID;
    printf("Enter PID of the target process: ");
    scanf("%u", &dwPID);

    if (InjectCode(dwPID)) {
        printf("Code injection successful!\n");
    }
    else {
        printf("Code injection failed.\n");
    }

    return 0;
}
