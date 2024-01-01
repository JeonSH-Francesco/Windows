#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <psapi.h>

int main()
{
	_wsetlocale(LC_ALL, L"korean");

	DWORD		pidArray[512] = { 0 };
	DWORD		cbNeeded = 0;
	DWORD		nProcesses = 0;
	TCHAR		szBuffer[MAX_PATH + _MAX_FNAME] = { 0 };
	UINT		uCounter = 0;


	if (::EnumProcesses(pidArray, sizeof(pidArray), &cbNeeded))
	{
		nProcesses = cbNeeded / sizeof(DWORD);
		for (DWORD i = 0; i <= nProcesses; i++)
		{
			HANDLE hProcess;
			DWORD pid = pidArray[i];
      
			//SYSTEM Permission 함수인 경우는 권한이 거부됨.(ERROR CODE : 5)
			hProcess = ::OpenProcess(
				PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
			if (hProcess != NULL)
			{
				::ZeroMemory(szBuffer, MAX_PATH + _MAX_FNAME);
				if (::GetModuleFileNameEx(hProcess, NULL,
					szBuffer, MAX_PATH + _MAX_FNAME) != NULL)
				{
					_tprintf(TEXT("[PID: %d] %s\n"), pid, szBuffer);
				}
				::CloseHandle(hProcess);
			}
			else
			{
				_tprintf(TEXT("[PID: %d, Error code: %d]\n"),
					pid, ::GetLastError());
				continue;
			}
		}
	}

	return 0;
}
