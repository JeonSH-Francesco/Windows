#include <iostream>
#include <tchar.h>
#include <windows.h>

int main()
{
    // 시스템 정보를 저장할 구조체를 초기화
    SYSTEM_INFO si = { 0 };

    // GetSystemInfo 함수를 사용하여 시스템 정보 확인
    ::GetSystemInfo(&si);
    // 페이지 크기를 출력
    _tprintf(_T("Page size: %d\n"), si.dwPageSize);
    // 할당 단위 크기를 출력
    _tprintf(_T("Allocation size: %d\n"), si.dwAllocationGranularity);

    return 0;
}
