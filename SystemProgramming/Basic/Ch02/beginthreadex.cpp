#include <iostream>
#include <process.h>
#include <windows.h>

UINT WINAPI ThreadFunction(LPVOID pParam)
{
    std::cout << "ThreadFunction() - Begin" << std::endl;
    std::cout << (const char*)pParam << std::endl;
    std::cout << "ThreadFunction() - End" << std::endl;
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
    std::cout << "main() - Begin" << std::endl;
    UINT nThreadId = 0;
    HANDLE hThread = (HANDLE)::_beginthreadex(
        NULL,
        0,
        ThreadFunction,
        (LPVOID)"PARAM",
        0,
        &nThreadId);
    if (hThread == NULL) {
    }

    ::WaitForSingleObject(hThread, INFINITE);
    ::CloseHandle(hThread);

    std::cout << "main() - End" << std::endl;
    return 0;
}

