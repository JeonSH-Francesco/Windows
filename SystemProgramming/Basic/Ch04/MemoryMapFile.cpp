#include <iostream>
#include <stdio.h>
#include <windows.h>

int main()
{
	//1. 텍스트 파일을 생성한다.
	HANDLE hFile = ::CreateFile(
		TEXT("C:\\MYFILE.TXT"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	//생성한 파일의 크기를 1024로 강제 설정한다.
	::SetFilePointer(hFile, 1024, NULL, FILE_BEGIN);

	//2.파일에 대한 매핑 객체를 생성한다.
	HANDLE hMap = ::CreateFileMapping(
		hFile, NULL, PAGE_READWRITE, 0, 1024, NULL);
	if (hMap == NULL)
	{
		wprintf(L"Failed to create file mapping obj [ERROR CODE: %d]\n",
			::GetLastError());

		::CloseHandle(hFile);
		return 0;
	}

	//3. 매핑 객체에 대한 접근 포인터를 얻음. (메모리로 추상화)
	char* pszMemory = (char*)::MapViewOfFile(
		hMap, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
	if (pszMemory == NULL)
	{
		::CloseHandle(hMap);
		::CloseHandle(hFile);
		return 0;
	}

	//4. 메모리에 데이터를 쓴다.
	strcpy_s(pszMemory, 1024, "Hello, Memory Mapped File!");
	//::FlushViewOfFile(pszMemory, 1024);

	//5. 메모리 매핑을 해제하고 종료한다.
	::UnmapViewOfFile(pszMemory);
	::CloseHandle(hMap);

	//::FlushFileBuffers(hFile);
	::CloseHandle(hFile);
	return 0;
}
