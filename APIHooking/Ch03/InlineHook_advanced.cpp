#include <iostream>
#include <windows.h>
#include <psapi.h> // 프로세스 정보 조회를 위한 헤더 파일

using namespace std;

//release x86 최적화 사용 안함으로 설정

#pragma pack(push, 1) // 패딩 없이 1바이트 단위로 정렬
typedef struct JUMP_CODE {
    BYTE opCode; // JMP 명령어 코드(0xE9)
    LPVOID targetAddr; // JMP 대상 주소
} JUMP_CODE;
#pragma pack(pop)

void* g_pOpenProcess;          // OpenProcess 함수 주소 저장
BYTE g_codesBeforeHook[5];     // 후킹 이전의 5바이트 코드 저장

BOOL hookOpenProcess(void);
void unhookOpenProcess(void);

// 후킹된 OpenProcess 함수
HANDLE MyOpenProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) {
    unhookOpenProcess(); // 후킹 해제

    // 원래 OpenProcess 함수 호출 준비
    HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;
    pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))g_pOpenProcess;

    // 추가 권한 요청
    dwDesiredAccess |= PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE;
    HANDLE hProcess = pfOpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);

    // 프로세스 이름 가져오기
    TCHAR name[MAX_PATH + _MAX_FNAME] = { 0 };
    DWORD result = ::GetModuleFileNameEx(hProcess, NULL, name, sizeof(name));
    if (result == 0) {
        wcout << L"Failed to retrieve process name for PID: " << dwProcessId << endl;
        hookOpenProcess();
        return hProcess; // 원래 핸들 반환
    }

    // 특정 프로세스(Notepad.exe) 종료
    if (_wcsicmp(wcsrchr(name, L'\\') + 1, L"notepad.exe") == 0) {
        cout << "\tMyOpenProcess()" << endl;
        cout << "\t************************************" << endl;
        cout << "\t***** Terminating notepad.exe! *****" << endl;
        cout << "\t************************************" << endl;

        // 프로세스 종료
        if (TerminateProcess(hProcess, 0)) {
            cout << "Successfully terminated notepad.exe!" << endl;
        }
        else {
            cerr << "Failed to terminate notepad.exe. Error: " << GetLastError() << endl;
        }

        hookOpenProcess(); // 후킹 재설치
        return NULL; // 종료 후 NULL 반환
    }

    hookOpenProcess(); // 후킹 재설치
    return hProcess; // 원래 OpenProcess 결과 반환
}

// 시스템의 모든 프로세스와 이름 출력
void printProcessList(void) {
    HMODULE hK32 = ::LoadLibrary(L"Kernel32.dll");
    HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;

    pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))
        ::GetProcAddress(hK32, "OpenProcess");

    DWORD aPid[1024] = { 0 };
    DWORD dwNeeded = 0;

    if (::EnumProcesses(aPid, sizeof(aPid), &dwNeeded)) {
        DWORD count = dwNeeded / sizeof(DWORD);
        HANDLE hProcess;
        for (DWORD i = 0; i <= count; ++i) {
            hProcess = pfOpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aPid[i]);
            if (hProcess != NULL) {
                TCHAR name[MAX_PATH + _MAX_FNAME] = { 0 };
                DWORD result = ::GetModuleFileNameEx(hProcess, NULL, name, sizeof(name));
                if (result > 0) {
                    ::wprintf(L"%s [PID : %d]\n", name, aPid[i]);
                }
                ::CloseHandle(hProcess);
            }
        }
    }

    ::FreeLibrary(hK32); // Kernel32.dll 언로드
}

// OpenProcess 함수에 Inline Hooking 설치
BOOL hookOpenProcess(void) {
    HMODULE hK32 = ::GetModuleHandle(L"Kernel32.dll");
    HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;

    pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))
        ::GetProcAddress(hK32, "OpenProcess");
    g_pOpenProcess = pfOpenProcess;

    DWORD dwOldProtect = 0;
    BOOL bResult = ::VirtualProtect((LPVOID)pfOpenProcess, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    if (!bResult) {
        cout << "Failed to change memory protection." << endl;
        return FALSE;
    }

    memcpy(g_codesBeforeHook, pfOpenProcess, 5);

    JUMP_CODE jmpCode = { 0 };
    jmpCode.opCode = 0xE9;
    jmpCode.targetAddr = (void*)((DWORD)MyOpenProcess - (DWORD)pfOpenProcess - 5);
    memcpy(pfOpenProcess, &jmpCode, 5);

    return TRUE;
}

// OpenProcess 함수 후킹 해제
void unhookOpenProcess(void) {
    if (g_pOpenProcess == NULL)
        return;
    memcpy(g_pOpenProcess, g_codesBeforeHook, 5);
}

// 32bit Inline Hook
int main() {
    setlocale(LC_ALL, "");

    if (!hookOpenProcess()) {
        cout << "Hooking Failed." << endl;
        return -1;
    }

    printProcessList(); // 프로세스 목록 출력 -> 후킹이 정상적으로 작동했는지 확인
    return 0;
}

/*이 버전의 경우는 notepad.exe를 종료하는 프로세스이다.
이 코드는 윤리적인 교육용으로 사용하길 바란다.
악성코드 관점에서는 백신과 같은 프로세스를 종료시키는데 초점이 맞춰 있을 것이고
백신 관점에서는 악성코드의 프로세스를 종료시키는데 초점이 맞춰 있기 때문이다.
*/
