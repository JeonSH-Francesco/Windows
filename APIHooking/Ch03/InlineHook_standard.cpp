#include <iostream>
#include <windows.h>
#include <psapi.h> //프로세스 정보 조회를 위한 헤더 파일

using namespace std;

#pragma pack(push,1) //패딩 없이 정렬 기준을 1바이트로 설정한다.
//즉, 멤버 변수가 정렬 요구사항을 무시하고 연속적으로 배치한다.
//JMP 명령어 구조를 표현하는 구조체
typedef struct JUMP_CODE {
	BYTE opCode; // JMP 명령어 코드(0xE9)
	LPVOID targetAddr; //JMP 대상 주소
}JUMP_CODE;
#pragma pack(pop) //정렬 설정을 이전 상태로 복원하여 추가 구조체 선언시 영향 없도록

//OpenProcess 함수 주소 저장
void* g_pOpenProcess;
// 후킹 이전의 5바이트 코드 저장
BYTE g_codesBeforeHook[5];

//함수 선언
BOOL hookOpenProcess(void);
void unhookOpenProcess(void);


//MyOpenProcess 후킹된 OpenProcess 함수
HANDLE MyOpenProcess(DWORD dwDesiredAccess, //요청된 접근 권한 
	BOOL bInheirtHandle,  //핸들을 상속할 수 있는지 여부
	DWORD dwProcessId //열고자 하는 프로세스의 PID
	)
{
	unhookOpenProcess(); //후킹 해제

	//원래 OpenProcess 함수 호출 준비-> 특정 함수의 주소를 저장할 수 있는 포인터
	//함수 포인터를 사용하는 이유
	//-> GetProcAddress로 특정 함수의 주소를 동적으로 로딩할 때 사용하기 위함
	HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;
    pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))g_pOpenProcess;

	//추가 권한을 요청
	dwDesiredAccess |= PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
	HANDLE hProcess = pfOpenProcess(dwDesiredAccess, bInheirtHandle, dwProcessId);

	//프로세스 이름 가져오기
	TCHAR name[MAX_PATH + _MAX_FNAME] = { 0 };
	DWORD result = ::GetModuleFileNameEx(hProcess, NULL, name, size(name));

	if (result == 0) {
		wcout << L"Failed to retrieve process name for PID: " << dwProcessId << endl;
		hookOpenProcess();
		return hProcess; // 원래 핸들 반환
	}

	//특정 프로세스(Notepad.exe) 차단
	//MyOpenProcess에서 notepad.exe를 감지하고 NULL을 반환하여 차단
	//반환값이 NULL이므로 호출자는 프로세스 핸들을 얻지 못함
	if (_wcsicmp(wcsrchr(name, L'\\') + 1, L"notepad.exe") == 0) {
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

	DWORD aPid[1024] = { 0 };  // 프로세스 ID를 저장할 배열
	DWORD dwNeeded = 0;        // 필요한 바이트 수

	//프로세스 목록 가져오기
	if (::EnumProcesses(aPid, sizeof(aPid), &dwNeeded)) {
		DWORD count = dwNeeded / sizeof(DWORD); // PID의 개수 계산
		HANDLE hProcess;
		//각 프로세스 ID(PID)에 대해 OpenProcess 호출
		for (DWORD i = 0; i <= count; ++i) {
			hProcess = pfOpenProcess(
				PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
				FALSE, aPid[i]);
			if (hProcess != NULL) { //프로세스 핸들이 유효한 경우 실행
				TCHAR name[MAX_PATH + _MAX_FNAME] = { 0 }; 
				//프로세스 모듈(실행 파일 또는 DLL 경로를)의 경로를 저장할 배열 초기화
				DWORD result = ::GetModuleFileNameEx(hProcess,NULL,name,sizeof(name));
				//해당 프로세스의 기본 모듈(실행 파일)경로를 name 배열에 저장
				if (result > 0) {
					//GetModuleFileNameEx 호출 성공 시, 경로 문자열의 길이(문자 수)를 반환
					::wprintf(L"%s [PID : %d]\n", name, aPid[i]);
					//모듈의 경로와 해당 프로세스의 PID를 출력
				}
				::CloseHandle(hProcess); //핸들 닫기

			}
		}
	}
	::FreeLibrary(hK32); //Kernel32.dll 언로드
}

//OpenProcess 함수에 Inline Hooking 설치
BOOL hookOpenProcess(void) {
	HMODULE hK32 = ::GetModuleHandle(L"Kernel32.dll");
	HANDLE(WINAPI * pfOpenProcess)(DWORD, BOOL, DWORD) = NULL;

	pfOpenProcess = (HANDLE(WINAPI*)(DWORD, BOOL, DWORD))
		::GetProcAddress(hK32, "OpenProcess");
	g_pOpenProcess = pfOpenProcess;

	DWORD dwOldProtect = 0;
	//메모리 보호 설정 변경
	BOOL bResult = ::VirtualProtect(
	(LPVOID)pfOpenProcess,5,
		PAGE_EXECUTE_READWRITE,&dwOldProtect);

	if (!bResult) {
		cout << "Failed to change memory protection." << endl;
		return FALSE;
	}

	//후킹 전 원래 코드 저장
	memcpy(g_codesBeforeHook,pfOpenProcess,5);

	//JMP 명령어 생성
	JUMP_CODE jmpCode = { 0 };
	jmpCode.opCode = 0xE9; //jmp
	//MyOpenProcess로 점프할 상대 주소를 계산
	jmpCode.targetAddr = (void*)((DWORD)MyOpenProcess - (DWORD)pfOpenProcess - 5);

	//JMP 명령어를 OpenProcess 시작 부분에 덮어쓰기
	memcpy(pfOpenProcess, &jmpCode, 5);

	return TRUE;
}

//OpenProcess 함수 후킹 해제
void unhookOpenProcess(void)
{
	if (g_pOpenProcess == NULL)
		return;
	//원래 코드 복원
	memcpy(g_pOpenProcess, g_codesBeforeHook, 5);
}

//32bit inline hook
int main(){
	setlocale(LC_ALL,"");

	if (!hookOpenProcess()) {
		cout << "Hooking Failed." << endl;
		return -1;
	}
	printProcessList(); //프로세스 목록 출력 -> 후킹이 정상적으로 작동했는지 확인
	
	return 0;
}





/*
세부 및 전체적인 설명
OpenProcess 함수의 시작 부분에 JMP 명령어를 덮어씌워 흐름을 사용자 정의 함수(MyOpenProcess)로 변경
hookOpenProcess를 호출하여 OpenProcess의 흐름을 가로챔
★MyOpenProcess 실행 시, 원래의 OpenProcess 동작을 수행하기 위해 후킹을 일시 해제
동작 완료 후 후킹을 다시 설치하여 흐름을 유지
MyOpenProcess로 점프할 상대 주소를 계산하여 JMP 명령어를 OpenProcess 시작 부분에 덮어쓰기

hookOpenProcess가 한 번 설치되면 OpenProcess 호출 흐름이 MyOpenProcess로 변경.
MyOpenProcess는 각 호출마다
1. 후킹을 해제
2. 원래 OpenProcess를 호출해 작업 수행
3. 필요 시 특정 프로세스(notepad.exe)를 차단
4. 후킹을 재설치. -> NULL 반환 이후에 실행되는 경우도 마찬가지로 하기 위함
이를 통해 프로세스가 실행되는 동안 지속적으로 OpenProcess 호출을 감시하고 제어

프로세스 목록을 출력하여 후킹이 정상적으로 작동했는지 확인하며
OpenProcess 호출을 후킹하고 특정 조건(notepad.exe)에서 NULL 반환하여
OpenProcess 호출에 대해 접근을 거부한다.

의의)
보안 S/W 개발자는 OpenProcess에 대한 후킹이나 행위 기반 감시를 통해 
악성코드의 이러한 시도를 선제적으로 방어하는데 의의를 두는 예제라고 볼 수 있음.
*/
