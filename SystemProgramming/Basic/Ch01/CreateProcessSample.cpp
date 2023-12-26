#include <iostream>
#include <Windows.h>
#include <lm.h> 
#include <Sddl.h>
#include <Psapi.h>

//새로운 프로세스를 생성하는 함수


void CreateSample01()
{
	STARTUPINFO si;
	::ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	//cb : count of bytes의 약자
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
	//winsta0은 Windows Station(창 역할과 사용자 세션에 대한 개념)
	//default는 해당 Windows Station의 기본 세션을 나타냅니다.
	//CreateProcessAsUser 함수로 새로운 프로세스를 생성할 때,
	//새로운 프로세스가 표시되는 데스크톱을 지정하는 부분입니다.
	//여기서 "winsta0\\default"는 일반적으로 "winsta0" 데스크톱에서 default 세션에 새로운 프로세스를 표시하라는 의미
	//HANDLE은 포인터다!!
	HANDLE hToken = NULL;
	HANDLE hProcess = NULL;

	//현재 프로세스에 대한 토큰을 얻는다.
	//PROCESS_QUERY_INFORMATION 정보 조회 = 읽기
	hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION,
		TRUE, ::GetCurrentProcessId());
	//OpenProcess() : 만들어서 작동중인 프로세스에 대한 핸들을 얻어 오는 것이고
	::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
	//OpenProcessToken()을 통해서 토큰을 담는다.
	//얻은 토큰으로 프로세스를 생성한다.

	TCHAR szBuffer[_MAX_FNAME] = TEXT("notepad.exe");
	//어떤 프로그램을 관리자 권한으로 띄우기 위해서 CreateProcessAsUser함수 사용
	if (CreateProcessAsUser(
		hToken,//CreateProcess를 그냥 호출해줘도 됨.
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
/*
현재 실행중인 프로세스의 토큰을 그대로 적용해주는데,
CreatProcessAsUser를 통해 만들어 봤음.

void CreateSample02()
{
	PROCESS_INFORMATION pi;
	::ZeroMemory(&pi, sizeof(pi));

	STARTUPINFO si;
	::ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = (LPTSTR)TEXT("winsta0\\default");

	TCHAR szBuffer[_MAX_FNAME] = TEXT("notepad.exe");

	if (CreateProcess(
		NULL,                // No module name (use command line). 
		szBuffer,
		NULL,                // Process handle not inheritable. 
		NULL,                // Thread handle not inheritable. 
		FALSE,               // Set handle inheritance to FALSE. 
		0,                   // No creation flags. 
		NULL,                // Use parent's environment block. 
		NULL,                // Use parent's starting directory. 
		&si,                 // Pointer to STARTUPINFO structure.
		&pi)                 // Pointer to PROCESS_INFORMATION structure.
		)
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
	}
}
이렇게 해줘도 됨.
*/

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
	//Process를 연다음에 토큰을 꺼내고

	//얻은 토큰에서 객체 소유자의 SID를 구한다.
	PTOKEN_USER pTokenUser = NULL;
	DWORD dwLength = 0;
	//사용자 토큰 정보 받아오고
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
		//ConvertSidToStringSid() : 일반적인 숫자의 Sid를 문자로 바꿔줘서 SID 출력

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
	CreateSample01();
	CreateSample02();
	PrintSid();
}
//SID확인 방법
//CMD
//wmic useraccount where name='username' get sid
//PowerShell
//Get-WmiObject Win32_UserAccount | Where-Object { $_.Name -eq 'username' } | Select-Object SID
