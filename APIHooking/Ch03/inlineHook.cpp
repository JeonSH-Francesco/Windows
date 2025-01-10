#include <iostream>
#include <windows.h>
#include <psapi.h> //프로세스 정보 조회를 위한 헤더 파일

using namespace std;

#pragma pack(push, 1)
//JMP 명령어 구조를 표현하는 구조체
typedef struct JUMP_CODE {
    BYTE opCode; //JMP 명령어 코드(0xE9)
    LPVOID targetAddr; //JMP 대상 주소
} JUMP_CODE;
#pragma pack(pop)

//OpenProcess 함수 주소 저장
void* g_pOpenProcess;
//후킹 이전의 5바이트 코드 저장
BYTE g_codesBeforeHook[5];

//함수 선언
BOOL hookOpenProcess(void);
void unhookOpenProcess(void);


//MyOpenProcess 후킹된 OpenProcess 함수
HANDLE MyOpenProcess(
    DWORD dwDesiredAccess, BOOL  bInheritHandle, DWORD dwProcessId)
{
    unhookOpenProcess(); //후킹 해제

    //원래 OpenProcess 함수 호출 준비
    HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;
    pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))g_pOpenProcess;

    //추가 권한을 요청
    dwDesiredAccess |= PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
    HANDLE hProcess = pfOpenProcess(dwDesiredAccess,
        bInheritHandle, dwProcessId);

    //프로세스 이름 가져오기
    TCHAR name[MAX_PATH + _MAX_FNAME] = { 0 };
    ::GetModuleFileNameEx(hProcess, NULL, name, size(name));
    //_putws(name);

    //특정 프로세스(Notepad.exe)차단
    if (wcsstr(name, L"Notepad.exe") != NULL)
    {
        cout << "\tMyOpenProcess()" << endl;
        cout << "\t************************************" << endl;
        cout << "\t***** Block notepad process!!! *****" << endl;
        cout << "\t************************************" << endl;

        hookOpenProcess(); //후킹 재설치
        return NULL; //Notepad 프로세스 차단
    }

    hookOpenProcess(); //후킹 재설치
    return hProcess; //원래 OpenProcess 결과 반환
}


//시스템의 모든 프로세스와 이름을 출력
void printProcessList(void)
{
    HMODULE hK32 = ::LoadLibrary(L"Kernel32.dll");
    HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;

    pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))
        ::GetProcAddress(hK32, "OpenProcess");

    DWORD aPid[1024] = { 0 };
    DWORD dwNeeded = 0;

    //프로세스 목록 가져오기
    if (::EnumProcesses(aPid, sizeof(aPid), &dwNeeded))
    {
        DWORD count = dwNeeded / sizeof(DWORD);
        HANDLE hProcess;
        //각 프로세스 ID(PID)에 대해 OpenProceass 호출
        for (DWORD i = 0; i <= count; ++i)
        {
            hProcess = pfOpenProcess(
                PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                FALSE, aPid[i]);
            if (hProcess != NULL)
            {
                TCHAR name[MAX_PATH + _MAX_FNAME] = { 0 };
                ::GetModuleFileNameEx(hProcess, NULL, name, sizeof(name));
                ::wprintf(L"%s [PID: %d]\n", name, aPid[i]);

                ::CloseHandle(hProcess);//핸들 닫기
            }
            //else
            //    ::wprintf(L"ERROR [PID: %d]\n", aPid[i]);
        }
    }

    ::FreeLibrary(hK32); //kernel32.dll 언로드
}


// OpenProcess 함수에 인라인 후킹 설치
BOOL hookOpenProcess(void)
{
    HMODULE hK32 = ::GetModuleHandle(L"Kernel32.dll");
    HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;

    pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))
        ::GetProcAddress(hK32, "OpenProcess");
    g_pOpenProcess = pfOpenProcess;

    DWORD dwOldProtect = 0;
    //메모리 보호 설정 변경
    BOOL bResult = ::VirtualProtect(
        (LPVOID)pfOpenProcess, 5,
        PAGE_EXECUTE_READWRITE, &dwOldProtect);
    //후킹 전 원래 코드 저장
    memcpy(g_codesBeforeHook, pfOpenProcess, 5);

    //JMP 명령어 생성
    //목적지주소 - 현재명령어주소 - 5
    //5(Byte)는 JMP, CALL 명령어의 크기 만큼 빼주는 것.
    JUMP_CODE jmpCode = { 0 };
    jmpCode.opCode = 0xE9;  //jmp
    jmpCode.targetAddr =
        (void*)((DWORD)MyOpenProcess - (DWORD)pfOpenProcess - 5);

    //JMP 명령어를 OpenProcess 시작 부분에 덮어쓰기
    memcpy(pfOpenProcess, &jmpCode, 5);
    //::FlushInstructionCache(::GetCurrentProcess(), g_pOpenProcess, 5);

    return TRUE;
}

//OpenProcess 함수 후킹 해제
void unhookOpenProcess(void)
{
    if (g_pOpenProcess == NULL)
        return;
    //원래 코드 복원
    memcpy(g_pOpenProcess, g_codesBeforeHook, 5);
    //::FlushInstructionCache(::GetCurrentProcess(), g_pOpenProcess, 5);
}


//32bit inline hook
int main()
{
    setlocale(LC_ALL, "");

    hookOpenProcess(); //OpenProcess 함수의 동작을 사용자 정의 함수인 MyOpenProcess로 대체
    //첫 5바이트를 JMP 명령어로 덮어씌워 MyOpenProcess로 흐름을 변경합니다.
    printProcessList(); //프로세스 목록 출력-> 후킹이 정상적으로 작동했는지 확인
   
    return 0;
}

