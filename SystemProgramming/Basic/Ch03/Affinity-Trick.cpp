#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <process.h>

UINT WINAPI ThreadFunction(LPVOID pParam)
{
    int nTmp = 0;
    while (1)
    {
        ++nTmp;
    }
    return 0;
}

int main()
{
    UINT nThreadId = 0;
    HANDLE hThread = (HANDLE)::_beginthreadex(
        NULL,
        0,
        ThreadFunction,
        NULL,
        0,
        &nThreadId);
    if (hThread == NULL) {
        std::cout << "ERROR: Failed to begin thread." << std::endl;
        return 0;
    }

    for (int i = 0; i < 4; ++i)
    {
        ::SetThreadAffinityMask(hThread, 0x00000001 << i);
        ::Sleep(5000);
        //몇 번 코어의 CPU 점유율을 순서대로 5초 간격으로 100%까지 끓어 올리도록
    }

    ::WaitForSingleObject(hThread, INFINITE);
    ::CloseHandle(hThread);
}
