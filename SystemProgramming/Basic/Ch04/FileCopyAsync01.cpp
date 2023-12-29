#include <iostream>
#include <stdio.h>
#include <Windows.h>


void CloseAll(char* pszMem, HANDLE hMap, HANDLE hSrc, HANDLE hDst)
{
	if (pszMem != NULL)
		::UnmapViewOfFile(pszMem);
	if (hMap != NULL)
		::CloseHandle(hMap);
	if (hSrc != NULL)
		::CloseHandle(hSrc);
	if (hDst != NULL)
		::CloseHandle(hDst);
}

int main()
{
	::_wsetlocale(LC_ALL, L"korean");
	HANDLE hFileSource = NULL, hFileTarget = NULL;
	LARGE_INTEGER	llFileSize = { 0 };
	HANDLE	hMap = NULL;
	char* pszMemory = NULL;

	//1. 원본 파일을 읽기모드로 연다.
	hFileSource = ::CreateFile(TEXT("C:\\Eyes of Glory.zip"),
		GENERIC_READ,				//읽기모드
		FILE_SHARE_READ,			//읽기모드 공유허용
		NULL,						//보안속성 없음.
		OPEN_EXISTING,				//존재하는 파일 열기
		FILE_ATTRIBUTE_NORMAL,
		NULL);						//동기 I/O
	if (hFileSource == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Failed to open source file [ERROR CODE: %d]\n",
			::GetLastError());
		return 0;
	}

	if (!::GetFileSizeEx(hFileSource, &llFileSize))
	{
		wprintf(L"Failed to get source file size [ERROR CODE: %d]\n",
			::GetLastError());
		CloseAll(NULL, NULL, hFileSource, NULL);
		return 0;
	}

	//2. 원본 파일에 대한 파일매핑 객체 생성
	hMap = ::CreateFileMapping(hFileSource, NULL, PAGE_READONLY, 0,
		(DWORD)llFileSize.QuadPart, NULL);
	if (hMap == NULL)
	{
		wprintf(L"Failed to create file mapping obj [ERROR CODE: %d]\n",
			::GetLastError());
		CloseAll(NULL, NULL, hFileSource, NULL);
		return 0;
	}

	//매핑 객체에 대한 접근 포인터를 얻음.
	pszMemory = (char*)::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0,
		(DWORD)llFileSize.QuadPart);
	if (pszMemory == NULL)
	{
		CloseAll(NULL, hMap, hFileSource, NULL);
		return 0;
	}

	//3. 대상 파일을 비동기 쓰기모드 모드로 개방
	::DeleteFile(TEXT("C:\\Eyes of Glory.zip"));
	hFileTarget = ::CreateFile(TEXT("C:\\Eyes of Glory-filecopy.zip"),
		GENERIC_ALL,				//쓰기모드
		0,							//공유안함.
		NULL,						//보안속성 없음.
		CREATE_ALWAYS,				//새파일 생성
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (hFileTarget == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Failed to open target file [ERROR CODE: %d]\n",
			::GetLastError());
		CloseAll(pszMemory, hMap, hFileSource, NULL);
		return 0;
	}

	//4. WriteFile() 함수를 이용해서 비동기 모드로 저장한다.
	LPOVERLAPPED pOverLapped = (LPOVERLAPPED)malloc(sizeof(OVERLAPPED));
	pOverLapped->OffsetHigh = 0;
	pOverLapped->Offset = 0;
	pOverLapped->hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	DWORD dwWritten = 0;
	::WriteFile(hFileTarget, pszMemory, (DWORD)llFileSize.QuadPart,
		&dwWritten, pOverLapped);
	if (::GetLastError() != ERROR_IO_PENDING)
	{
		wprintf(L"WriteFile [ERROR CODE: %d]", ::GetLastError());

		CloseAll(pszMemory, hMap, hFileSource, NULL);
		::CloseHandle(pOverLapped->hEvent);
		free(pOverLapped);
		return 0;
	}

	//5. 비동기 처리가 완료 될때까지 대기
	if (::WaitForSingleObject(pOverLapped->hEvent, INFINITE) ==
		WAIT_OBJECT_0)
		_putws(L"Complete!\n");

	CloseAll(pszMemory, hMap, hFileSource, hFileTarget);
	::CloseHandle(pOverLapped->hEvent);
	free(pOverLapped);

	return 0;
}


