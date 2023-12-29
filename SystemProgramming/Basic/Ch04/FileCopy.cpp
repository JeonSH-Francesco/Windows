#include <iostream>
#include <stdio.h>
#include <windows.h>

int main()
{
	::_wsetlocale(LC_ALL, L"korean");

	HANDLE hFileSource = NULL, hFileTarget = NULL;

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

	//2. 대상 파일을 쓰기모드로 연다.
	hFileTarget = ::CreateFile(TEXT("C:\\Eyes of Glory-copy.zip"),
		GENERIC_WRITE,				//쓰기모드
		0,							//공유안함.
		NULL,						//보안속성 없음.
		CREATE_ALWAYS,				//새파일 생성
		FILE_ATTRIBUTE_NORMAL,
		NULL);						//동기 I/O
	if (hFileTarget == INVALID_HANDLE_VALUE)
	{
		wprintf(L"Failed to open target file [ERROR CODE: %d]\n",
			::GetLastError());

		::CloseHandle(hFileSource);
		return 0;
	}

	//3. 루프를 돌면서 파일 내용을 복사한다.
	LARGE_INTEGER	llFileSize = { 0 };
	LONGLONG	llTotalReadSize = 0;
	DWORD		dwReadSize = 0, dwWriteSize = 0;
	BOOL		bResult = FALSE;
	BYTE		byBuffer[65536];

	if (!::GetFileSizeEx(hFileSource, &llFileSize))
	{
		wprintf(L"원본 파일의 크기를 알 수 없습니다.\n");
		::CloseHandle(hFileSource);
		::CloseHandle(hFileTarget);
		return 0;
	}

	for (LONGLONG i = 0; i < llFileSize.QuadPart; i += dwReadSize)
	{
		//64KB씩 읽어서 복사
		::ZeroMemory(byBuffer, 65536);
		bResult = ::ReadFile(hFileSource,
			byBuffer, 65536, &dwReadSize, NULL);
		if (!bResult)
		{
			wprintf(L"Failed to read source file [ERROR CODE: %d]\n",
				::GetLastError());
			break;
		}
		//파일의 끝이면 종료
		else if (bResult && dwReadSize == 0)
			break;

		llTotalReadSize += dwReadSize;
		wprintf(L"%I64d%%\n",
			llTotalReadSize * 100 / llFileSize.QuadPart);

		//64KB씩 저장
		if (!::WriteFile(hFileTarget,
			byBuffer, dwReadSize, &dwWriteSize, NULL) ||
			dwReadSize != dwWriteSize)
		{
			wprintf(L"Failed to write target file [ERROR CODE: %d]\n",
				::GetLastError());
			break;
		}
	}

	_putws(L"Complete!\n");
	::CloseHandle(hFileSource);
	::CloseHandle(hFileTarget);
	return 0;
}
