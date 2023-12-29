#include <iostream>
#include <windows.h>
#include <tchar.h>

void CALLBACK FileIoComplete(
    DWORD dwError,
    DWORD dwTransferred,
    LPOVERLAPPED pOverlapped)
{
    printf("FileIoComplete() Callback - [%d bytes] 쓰기 완료\n", dwTransferred);

    // 사용한 메모리를 해제
    delete[]((char*)pOverlapped->hEvent);
    delete pOverlapped;
    puts("FileIoComplete() - return \n");
}

DWORD WINAPI IoThreadFunction(LPVOID pParam) {
    char* pszBuffer = new char[16];
    memset(pszBuffer, 0, sizeof(char) * 16);
    strcpy_s(pszBuffer, sizeof(char) * 16, "Hello IOCP");

    LPOVERLAPPED pOverlapped = new OVERLAPPED;
    memset(pOverlapped, 0, sizeof(OVERLAPPED));

    pOverlapped->Offset = 1024 * 1024 * 5;
    pOverlapped->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    puts("IoThreadFunction() - 중첩된 쓰기 시도");
    ::WriteFileEx((HANDLE)pParam,
        pszBuffer,
        sizeof(char) * 16,
        pOverlapped,
        FileIoComplete);

    // 기다리는 동안 작업을 수행
    for (; ::SleepEx(1, TRUE) != WAIT_IO_COMPLETION;);

    // 사용한 메모리를 해제
    CloseHandle(pOverlapped->hEvent);
    delete pOverlapped;
    delete[] pszBuffer;

    puts("IoThreadFunction() - return ");
    return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
    HANDLE hFile = ::CreateFile(_T("TestFile.txt"),
        GENERIC_WRITE, // 쓰기 모드
        0,  // 공유하지 않음
        NULL,
        CREATE_ALWAYS, // 무조건 생성
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // 중첩된 쓰기 -> 비동기 쓰기
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        _tprintf(_T("Error creating file. Error code: %d\n"), GetLastError());
        return 1;
    }

    HANDLE hThread = ::CreateThread(
        NULL,
        0,
        IoThreadFunction,
        hFile,
        0,
        NULL);

    if (hThread == NULL) {
        _tprintf(_T("Error creating thread. Error code: %d\n"), GetLastError());
        CloseHandle(hFile);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    // 스레드 종료 후 사용된 자원을 해제
    CloseHandle(hThread);
    CloseHandle(hFile);

    return 0;
}
