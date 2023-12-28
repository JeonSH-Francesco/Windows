#include <iostream>
#include <stdio.h>
#include <process.h>
#include <Windows.h>


HANDLE g_hSema; //세마포어 핸들
TCHAR g_StringList[10][64] = { 0 };

UINT WINAPI ThreadSemaphore(LPVOID pParam)
{
	int nIndex = (int)pParam;

	while (TRUE)
	{
		///////////////////////////////////////////////////////
		::wsprintf(g_StringList[nIndex],
			TEXT("%d thread is waiting!"), nIndex);
		::Sleep(500);

		///////////////////////////////////////////////////////
		DWORD dwResult = ::WaitForSingleObject(g_hSema, INFINITE);
		//세마포어 핸들 g_hSema를 놓고, WaitForSingleObject 함수를 10개의 스레드가 동시에 호출
		::wsprintf(g_StringList[nIndex],
			TEXT("##%d thread is selected!##"), nIndex);
		::Sleep(500);
		::ReleaseSemaphore(g_hSema, 1, NULL);
		//Count 1을 통해 Release 되면서 다른 스레드가 진입할 수 있도록 한다.
	}
	return 0;
}

int main()
{
	g_hSema = ::CreateSemaphore(NULL, 3, 3, NULL);
	//WaitForSingleObject 함수를 호출할 때, 10개의 스레드 중 동시 진입 할 수 있는 스레드는 3개로 제한한다.

	UINT nThreadId = 0;
	HANDLE hThread = NULL;

	for (int i = 0; i < 10; ++i)
	{
		hThread = (HANDLE)::_beginthreadex(
			NULL,
			0,
			ThreadSemaphore,
			(LPVOID)i,
			0,
			&nThreadId);
		::CloseHandle(hThread);
	}

	while (1)
	{
		system("cls");
		for (int i = 0; i < 10; ++i)
			_putws(g_StringList[i]);

		::Sleep(1000);
	}

	::CloseHandle(g_hSema);
}
