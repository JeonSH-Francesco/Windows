#include <iostream>
using namespace std;
#include <windows.h>

DWORD WINAPI ThreadFunction(LPVOID pParam)
{
    cout << "ThreadFunction() - Begin" <<endl;
    cout << (const char*)pParam << endl;
    cout << "ThreadFunction() - End" << endl;
    return 0;
}

//C-RunTime-Library함수를 쓰게 되는 경우에 전역 변수나 정적데이터를 가지게 되는 경우
//정적데이터를 VMS에서 처리하는 것이 생기게 되면, 스레드 간에 문제가 발생할 수 있다.
//->해결책 : TLS(Thread Local Storage)
//예)strtok함수같은 경우 문자열을 파싱을 하는 경우 T1 T2에서 호출하다가 충돌 날 수 있다. 
//따라서 _beginthreadex 함수는 스레드간의 간섭이 있는 경우에도 쓰면 상관 없다.
//CreateThread함수는 스레드 간의 간섭이 아예 없는 경우에 쓴다.

int main()
{
    cout << "main() - Begin" << endl;
    DWORD dwThreadId = 0;
    HANDLE hThread = ::CreateThread(
        NULL,
        0,
        ThreadFunction,
        (LPVOID)"PARAM",
        0,
        &dwThreadId);
    if (hThread == NULL) {
    }

    cout << "main() - WaitForSingleObject() - before" << endl;

    ::WaitForSingleObject(hThread, INFINITE);
    ::CloseHandle(hThread);

    cout << "main() - End" << endl;
    return 0;
}

