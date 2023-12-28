#include <iostream>
using namespace std;
#include <conio.h>
#include <process.h>
#include <windows.h>

UINT WINAPI ThreadFunction(LPVOID pParam) {
    HANDLE hEvent = pParam;
    cout << "ThreadFunction() - Begin" << endl;

    _getch();
    SetEvent(hEvent);
    cout << "ThreadFunction() - SetEvent()" << endl;
    cout << "ThreadFunction() - End" << endl;
    return 0;
}


int main()
{
    cout << "main() - Begin" << endl;
    HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

    UINT nThreadId = 0;
    HANDLE hThread = (HANDLE)::_beginthreadex(
        NULL,
        0,
        ThreadFunction,
        (LPVOID)hEvent,
        0,
        &nThreadId);
    if (hThread == NULL) {
        std::cout << "ERROR: Failed to begin thread." << std::endl;
        return 0;
    }

    if (::WaitForSingleObject(
        hEvent, INFINITE) == WAIT_OBJECT_0)
    {
        cout << "main() - WaitForSingleObject() return" << endl;
    }

    ::WaitForSingleObject(hThread, INFINITE);
    cout << "main() - Thread End" << endl;

    ::CloseHandle(hThread);
    cout << "main() - End" << endl;
    return 0;
}

/*
HANDLE은 POINTER다.
WaitForSingleObject 함수는 가리키고 있는 대상(핸들)이 들어가는데, 이벤트의 변화를 감지하는 함수다.(이벤트가 Set 되는 시점을 감지한다.)
의도적으로 시점을 만들어낼 때, Event를 사용한다.
Event 상태는 Set Reset 두가지가 있다.
->시점 동기화 : Event

결과 출력 경우 2가지
-------------------------------------
main() - Begin
ThreadFunction() - Begin
ThreadFunction() - SetEvent()
ThreadFunction() - End
main() - WaitForSingleObject() return
main() - Thread End
main() - End

--------------------------------------
main() - Begin
ThreadFunction() - Begin
ThreadFunction() - SetEvent()main() - WaitForSingleObject() return

ThreadFunction() - End
main() - Thread End
main() - End

*/
