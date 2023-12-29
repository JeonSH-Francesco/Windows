#include <iostream>
#include <windows.h>
#include <tchar.h>


void CALLBACK FileIoComplete(
    DWORD dwError,
    DWORD dwTransfered,
    LPOVERLAPPED pO1)
{
    printf("FileIoComplete() Callback - [%d 바이트] 쓰기 완료 -%s\n", dwTransfered, (char*)pO1->hEvent);

    //hEvent 멤버를 포인터로 전용했으므로 가리키는 대상 메모리를 해제한다.
    //이 메모리는 IoThreadFunction() 함수에서 동적 할당된 것들이다!
    delete[] pO1->hEvent;
    delete pO1;
    puts("FileIoComplete() - return \n");
}

DWORD WINAPI IoThreadFunction(LPVOID pParam) {
    char* pszBuffer = new char[16];
    memset(pszBuffer, 0, sizeof(char) * 16);
    strcpy_s(pszBuffer, sizeof(char) * 16, "Hello IOCP");

    LPOVERLAPPED pOverlapped = NULL;
    pOverlapped = new OVERLAPPED;
    memset(pOverlapped, 0, sizeof(OVERLAPPED));

    pOverlapped->Offset = 1024 * 1024 * 5;
    pOverlapped->hEvent = pszBuffer;

    puts("IoThreadFunction() - 중첩된 쓰기 시도");
    ::WriteFileEx((HANDLE)pParam,
        pszBuffer,
        sizeof(char) * 16,
        pOverlapped,
        FileIoComplete);

    // 비동기 쓰기 시도에 대해 ALERTABLE_WAIT 상태로 대기
    for (; ::SleepEx(1, TRUE) != WAIT_IO_COMPLETION;); {
        //SleepEx를 호출한 WorkerThread가 ALERTABLE_WAIT 상태가 된다/
        puts("IothreadFunction() - return ");
    }
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

    HANDLE hThread = NULL;
    DWORD dwThreadID = 0;
    dwThreadID=0;
    hThread = ::CreateThread(
        NULL,
        0,
        IoThreadFunction,
        hFile,
        0,
        &dwThreadID);
    //작업자 스레드가 종료될 때까지 기다린다.
    ::WaitForSingleObject(hThread,INFINITE);

    return 0;
}
