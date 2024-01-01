
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>

int main()
{
	_wsetlocale(LC_ALL, L"korean");
	HANDLE		hSnapshot = INVALID_HANDLE_VALUE;

	//CreateToolhelp32Snapshot()함수
	//권한(SYSTEM)이 거부된 프로세스들도 다 나열->현재 실행중인 프로세스를 모두 나열
	hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32 = { sizeof(pe32) };

		BOOL bFlag = ::Process32First(hSnapshot, &pe32);
		for (; bFlag; bFlag = ::Process32Next(hSnapshot, &pe32))
			_tprintf(TEXT("[PID: %d] %s\n"),
				pe32.th32ProcessID, pe32.szExeFile);
		::CloseHandle(hSnapshot);
	}
	
	return 0;
}
