#include <iostream>
#include <windows.h>
#include <stdio.h>

//볼륨 및 디스크 정보
//MAX_PATH: 파일 경로를 저장하기 위한 문자열의 최대 길이 MAX_FNAME : 파일 이름 부분만 고려한 최대 길이를 나타내는 사용자가 정의한 이름
void LoadDiskInfo(const TCHAR* pszPath)
{
	TCHAR szVolName[MAX_PATH] = { 0 };
	TCHAR szFileSys[MAX_PATH] = { 0 };
	TCHAR szRoot[MAX_PATH] = { 0 };
	DWORD dwSerialNum = 0, dwMaxCompLen = 0, dwSysFlag = 0;
	
	//3인 이유 : C:\\이라 3byte
	memcpy(szRoot, pszPath, sizeof(TCHAR) * 3);
	//BOOL GetVolumeInformation(디스크의 루트 경로, 볼륨 이름을 저장할 버퍼, 볼륨 이름의 버퍼 크기, 볼륨의 일련 번호, 최대 파일명 길이, 파일 시스템의 속성 플래그, 파일 시스템 이름을 저장할 버퍼,파일 시스템 이름 버퍼 크기)
	::GetVolumeInformation(szRoot, szVolName, MAX_PATH, &dwSerialNum,
		&dwMaxCompLen, &dwSysFlag, szFileSys, MAX_PATH);

	wprintf(TEXT("Volume name : %s, File system : %s\n"),
		szVolName, szFileSys);

	//공용체 ULARGE_INTEGER 호환하기 위한
	ULARGE_INTEGER llAvailableSpace = { 0 };
	ULARGE_INTEGER llTotalSpace = { 0 };
	ULARGE_INTEGER llFreeSpace = { 0 };

	//BOOL GetDiskFreeSpaceEx(디렉터리 경로, 호출자에게 사용가능한 바이트 수, 전체 바이트 수, 사용 가능한 전체 바이트 수)
	if (::GetDiskFreeSpaceEx(szRoot,
		&llAvailableSpace, &llTotalSpace, &llFreeSpace))
	{
		wprintf(TEXT(" (Disk free space: %I64u/%I64u GB)\n"),
			llFreeSpace.QuadPart / (1024 * 1024 * 1024),
			llTotalSpace.QuadPart / (1024 * 1024 * 1024));
	}
}

//파일 목록과 현재 디렉터리
void LoadFileList(const TCHAR* pszPath)
{
	//*.exe: 실행파일만 보기
	//a*.txt: a로 시작하는 텍스트 파일만 보기
	//*.* : 모든 파일 보기
	TCHAR szPath[MAX_PATH] = { 0 };
	wsprintf(szPath, TEXT("%s\\*.*"), pszPath);

	WIN32_FIND_DATA FindData;
	BOOL bResult = TRUE;
	//ZeroMemory(PVOID Destination, SIZE_T Length);
	//ZeroMemory(초기화 하려는 메모리 블록의 시작 주소, 초기화 하려는 메모리 블록의 크기(바이트 단위))
	::ZeroMemory(&FindData, sizeof(WIN32_FIND_DATA));
	
	//HANDLE FindFirstFile(검색할 파일 또는 디렉터리의 경로 및 이름, 검색 결과를 저장할 WIN32_FIND_DATA 구조체 포인터)
	HANDLE hFirstFile = ::FindFirstFile((LPTSTR)(const TCHAR*)szPath, &FindData);
	while (bResult)
	{
		// 파일이나 디렉터리의 속성을 나타내는 멤버 : dwFileAttributes
		//FILE_ATTRIBUTE_DIRECTORY 는 디렉터리인지 여부를 나타내는 상수로, 만약 현재 처맂 ㅜㅇ인 파일이 디렉터리인 경우
		//이 비트 연산은 1이 된다.
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			//현재 파일 또는 디렉터리의 이름을 나타낸다.
			wprintf(TEXT("[DIR] %s\n"), FindData.cFileName);
		}
		else {
			wprintf(TEXT("%s\n"), FindData.cFileName);
		}
		//FindNextFile 함수를 호출하여 다음 파일 도는 디렉터리의 정보를 가져오고
		//FindData 구조체는 찾은 파일 또는 디렉터리의 정보로 업데이트
		//더 이상 찾을 파일이 없으면 FALSE
		
		//FindNextFile(FindFirstFile에서 반환된 검색 핸들, 검색 결과를 저장할 WIN32_FIND_DATA 구조체의 포인터)
		bResult = ::FindNextFile(hFirstFile, &FindData);
	}
}

int main()
{
	::_wsetlocale(LC_ALL, TEXT("korean"));
	//locale은 특정 지역 또는 언어 환경에 대한 문화적 규칙, 문자열 정렬, 숫자 및 날짜 형식 등을 지정하는데 사용
	//_wsetlocale(LC_ALL: 모든 locale 카테고리에 대한 설정, 설정하려는 locale 이름)
	//_w가 함수 이름 앞에 붙어 있으면 이 함수를 통해 locale을 설정하면
	// 프로그램의 출력이나 문자열 처리와 관련된 다양한 작업에서 해당 locale 규칙이나 형식을 다르게 된다.

	LoadDiskInfo(TEXT("C:\\"));
	LoadFileList(TEXT("C:\\"));

	TCHAR szCurDir[MAX_PATH] = { 0 };
	//DWORD GetCurrentDirectory(버퍼 크기, 디렉터리 경로를 저장할 버퍼)
	::GetCurrentDirectory(MAX_PATH, szCurDir);
	wprintf(TEXT("Current Directory: %s\n"), szCurDir);
}
