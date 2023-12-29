#include <iostream>
#include <windows.h>
#include <tchar.h>

void CALLBACK FileIoComplete(
    DWORD dwError, //에러 코드
    DWORD dwTransfered, //입/출력이 완료된 데이터 크기
    LPOVERLAPPED pO1) //OVERLAPPED 구조체의 주소
{
    printf("FileIoComplete() Callback - [%d 바이트] 쓰기 완료 -%s\n", dwTransfered, (char*)pO1->hEvent);

    //hEvent 멤버를 포인터로 전용했으므로 가리키는 대상 메모리를 해제한다.
    //이 메모리는 IoThreadFunction() 함수에서 동적 할당된 것들이다!!!

    delete[] pO1->hEvent;
    delete pO1;
    puts("FileIoComplete() - return \n");
}

DWORD WINAPI IoThreadFunction(LPVOID pParam) {
    //메모리를 동적 할당하고 값을 채운다.
    //이 메모리는 완료 함수에서 해제한다.
    char* pszBuffer = new char[16];
    memset(pszBuffer, 0, sizeof(char) * 16);
    strcpy_s(pszBuffer, sizeof(char) * 16, "Hello IOCP");

    //OVERLAPPED 구조체의 hEvent 멤버를 포인터 변수로 전용한다.
    LPOVERLAPPED pOverlapped = NULL;
    pOverlapped = new OVERLAPPED;
    memset(pOverlapped, 0, sizeof(OVERLAPPED));

    pOverlapped->Offset = 1024 * 1024 * 5; //5MB만큼의 offset값을 채워놓고
    pOverlapped->hEvent = pszBuffer; //포인터로 활용하여 pOverlapped->hEvent = pszBuffer;

    puts("IoThreadFunction() - 중첩된 쓰기 시도");
    ::WriteFileEx((HANDLE)pParam,
        pszBuffer,
        sizeof(char) * 16,
        pOverlapped,
        FileIoComplete);//WriteFileEx함수는 함수의 주소를 맨 마지막 변수에 넣어줌으로써 callback할 수 있다.

    // 비동기 쓰기 시도에 대해 ALERTABLE_WAIT 상태로 대기
    for (; ::SleepEx(1, TRUE) != WAIT_IO_COMPLETION;); {
        //SleepEx에서 TRUE로 하면 SleepEx() 호출한 WorkerThread가 ALERTABLE_WAIT 상태가 된다!
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
    //비동기 쓰기를 위한 스레드를 생성한다.
    hThread = ::CreateThread(
        NULL,
        0,
        IoThreadFunction,
        hFile,
        0,
        &dwThreadID);
    //main 함수에서 WorkerThread를 생성하고 그냥 Wait하고 있다.
    //작업자 스레드가 종료될 때까지 기다린다.
    ::WaitForSingleObject(hThread,INFINITE);

    return 0;
}

// 정리
//SleepEx함수의 두 번째 매개변수를 True로 주게 되면서
//ALERTABLE_WAIT 상태(Wait 함수를 호출하면서 동시에 APC (Asynchronous Procedure Call)를 처리할 수 있는 상태)
//SleepEX를 call한 Workerthread는 ALERTABLE_WAIT가 되어 OS가 Callback 해줄 수 있는 상태가 된다.
//Read/WriteFileEX()의 마지막 매개변수로 넘겨준 Callback함수가 OS에 의해서 Callback이 되면서 메모리를 해제할 수 있다.
//(작업자 스레드에서 동적할당)
// IOCP(IO Completion Port)
// ->WorkerThread에서 메모리를 동적할당하는데, Callback되는 함수에서 메모리를 해제한다!
// 비동기 Callback 장점은 다음과 같다.
// 비동기 작업의 병렬 처리, 응답성 향상, 리소스 효율성, 
// 스케일링(IOCP를 사용하면 수천 개의 연결 또는 동시 작업을 효과적으로 처리할 수 있어, 대규모 시스템에서 확장성을 제공합니다.)
