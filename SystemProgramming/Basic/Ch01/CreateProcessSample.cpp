// CreateProcessSample.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//cmd에서 wmic useraccount where name='사용자이름' get sid으로 확인 가능

#include <iostream>
#include <Windows.h>
#include <lm.h>
#include <Sddl.h>
#include <Psapi.h>


void CreateSample01()
{
	STARTUPINFO si;
	::ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION pi;
	::ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	TCHAR szBuffer[_MAX_FNAME] = TEXT("notepad.exe");
	if (::CreateProcess(NULL, // No module name (use command line). 
		szBuffer,
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi)              // Pointer to PROCESS_INFORMATION structure.
		)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}


void CreateSample02()
{
	PROCESS_INFORMATION pi;
	::ZeroMemory(&pi, sizeof(pi));

	STARTUPINFO si;
	::ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = (LPTSTR)TEXT("winsta0\\default");

	HANDLE hToken = NULL;
	HANDLE hProcess = NULL;

	//현재 프로세스에 대한 토큰을 얻는다.
	hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION,
		TRUE, ::GetCurrentProcessId());
	::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);

	//얻은 토큰으로 프로세스를 생성한다.
	TCHAR szBuffer[_MAX_FNAME] = TEXT("notepad.exe");
	if (CreateProcessAsUser(
		hToken,
		NULL, // No module name (use command line). 
		szBuffer,
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		TRUE,             // Set handle inheritance to FALSE. 
		CREATE_NEW_CONSOLE,
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi)              // Pointer to PROCESS_INFORMATION structure.
		)
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}

	::CloseHandle(hToken);
	::CloseHandle(hProcess);
}

void PrintSid()
{
	HANDLE hToken = NULL;
	HANDLE hProcess = NULL;
	//현재 프로세스의 토큰을 얻는다.
	hProcess = ::OpenProcess(
		PROCESS_QUERY_INFORMATION, TRUE,
		::GetCurrentProcessId());
	::OpenProcessToken(hProcess,
		TOKEN_ALL_ACCESS, &hToken);

	//얻은 토큰에서 객체 소유자의 SID를 구한다.
	PTOKEN_USER pTokenUser = NULL;
	DWORD dwLength = 0;
	::GetTokenInformation(hToken,
		TokenUser, (LPVOID)pTokenUser, 0, &dwLength);
	pTokenUser = (PTOKEN_USER)HeapAlloc(
		::GetProcessHeap(), HEAP_ZERO_MEMORY, dwLength);

	if (::GetTokenInformation(hToken,
		TokenUser, pTokenUser, dwLength, &dwLength))
	{
		TCHAR* pszBuffer = NULL;
		//SID를 문자열로 바꾸어 출력한다.
		::ConvertSidToStringSid(pTokenUser->User.Sid, &pszBuffer);

		wprintf(L"SID: %s\n", pszBuffer);
		::LocalFree((HLOCAL)pszBuffer);
	}
	else
	{
		wprintf(L"ERROR NUMBER: %d", ::GetLastError());
	}

	::CloseHandle(hToken);
	::CloseHandle(hProcess);
	::HeapFree(::GetProcessHeap(), 0, (LPVOID)pTokenUser);
}

int main()
{
	//CreateSample01();
	//CreateSample02();
	PrintSid();
}
