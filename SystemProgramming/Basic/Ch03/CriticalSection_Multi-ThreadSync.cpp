//두 개의 스레드가 g_pszBuffer 변수에 안전하게 접근할 수 있도록 크리티컬 섹션을 사용하여
//g_pszBuffer에 안전하게 접근하도록 보장하고, 스레드 간의 동기화를 유지하면서 데이터를 공유할 수 있도록 하였다.
//SetString을 호출하는 순서와 관계없이 결과는 랜덤하게 먼저 실행되는 Thread가 ThreadFunc1, ThreadFunc2 중 하나의 문자열을 출력한다.

#include <iostream>
#include <windows.h>
#include <process.h>

//전역 변수
char* g_pszBuffer = NULL;
CRITICAL_SECTION g_cs;
//Windows 환경에서 제공되는 동기화 객체 중 하나
//임계영역에 대한 동시 접근을 막기 위한 변수 사용 
//-> 멀티 스레드 프로그램에서 여러 스레드가 공유 자원에 접근 할 때 경쟁 조건 방지 데이터 일관성 유지
bool g_bExit = false; // 종료 조건을 나타내는 플래그 

//문자열을 설정하는 함수
void SetString(const char* pszData) {
	::EnterCriticalSection(&g_cs);
	if (g_pszBuffer != NULL) {
		free(g_pszBuffer); //메모리 해제
		g_pszBuffer = (char*)malloc(64);//새로운 메모리 할당
		sprintf_s(g_pszBuffer, 64, "%s", pszData); // 문자열 복사
	}
	else {
		g_pszBuffer = (char*)malloc(64); //메모리 할당
		sprintf_s(g_pszBuffer, 64, "%s", pszData); //문자열 복사
	}
	::LeaveCriticalSection(&g_cs);
}

BOOL GetString(char* pszData) {
	::EnterCriticalSection(&g_cs);

	if (pszData != NULL) {
		sprintf_s(pszData, 64, "%s", g_pszBuffer);
		free(g_pszBuffer);
		g_pszBuffer = NULL;
		
		::LeaveCriticalSection(&g_cs);
		return TRUE;
	}

	::LeaveCriticalSection(&g_cs);
	return FALSE;
}

//첫 번째 스레드의 실행 함수
UINT ThreadFunc1(LPVOID pParam){
	while (!g_bExit) {
		::Sleep(1);
		SetString("ThreadFunc1");
	}
	return 0;
}

//두 번째 스레드의 실행 함수
UINT ThreadFunc2(LPVOID pParam) {
	while (!g_bExit) {
		::Sleep(1);
		SetString("ThreadFunc2");
	}
	return 0;
}

int main() {
	::InitializeCriticalSection(&g_cs);

	UINT nThreadId = 0;
	HANDLE hThread = (HANDLE)::_beginthreadex(
	NULL,
	0,
	ThreadFunc1,
	NULL,
	0,
	&nThreadId);
	::CloseHandle(hThread);

	hThread = (HANDLE)::_beginthreadex(
	NULL,
	0,
	ThreadFunc2,
	NULL,
	0,
	&nThreadId);

	char szBuffer[64];
	for (int i = 0; i < 5; i++) {
		::Sleep(500);

		GetString(szBuffer);
		puts(szBuffer);
	}

	g_bExit = true; // 모든 스레드에게 종료 조건 알림
	::WaitForSingleObject(hThread, INFINITE);
	::CloseHandle(hThread);

	::DeleteCriticalSection(&g_cs);
	return 0;
}
