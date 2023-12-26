#include <iostream>
using namespace std;
#include <process.h>
#include <windows.h>

int g_bFlag = FALSE; //스레드를 통제하기 위한 전역 변수

UINT WINAPI ThreadFunction(LPVOID pParam)
{
	cout << "ThreadFunction() - begin" << endl;
	g_bFlag = TRUE;
	while (g_bFlag) {
		cout << "Worker Thread - while loop" << endl;
		::Sleep(1000);
	}
	cout << "ThreadFunction-End" << endl;
	return 0;
}

int main() {
	cout << "main() - Begin" << endl;
	UINT nThreadId = 0;
	HANDLE hThread = (HANDLE)::_beginthreadex(
		NULL,
		0,
		ThreadFunction,
		(LPVOID)"PARAM",
		0,
		&nThreadId);

	if (hThread == NULL) {
		cout << "ERROR: Failed to begin thread" << endl;
		return 0;
	}
	
	::Sleep(2000);
	cout << "main() - SuspendThread()" << endl;
	::SuspendThread(hThread);

	for (int i = 0; i < 3; i++) {
		::WaitForSingleObject(hThread, 1000);
		cout << "main() - WaitForSingleObject(1000)" << endl;
	}

	::ResumeThread(hThread);
	cout << "main() - ResumeThread()" << endl;

	::Sleep(2000);
	g_bFlag = FALSE;
	::WaitForSingleObject(hThread, INFINITE);
	::CloseHandle(hThread);

	cout << "main() - End" << endl;
	return 0;
}
